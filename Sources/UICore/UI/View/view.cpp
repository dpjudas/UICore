/*
**  UICore
**  Copyright (c) 1997-2015 The UICore Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries UICore may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "UICore/precomp.h"
#include "UICore/UI/View/view.h"
#include "UICore/UI/View/view_action.h"
#include "UICore/UI/TopLevel/view_tree.h"
#include "UICore/UI/Events/event.h"
#include "UICore/UI/Events/activation_change_event.h"
#include "UICore/UI/Events/close_event.h"
#include "UICore/UI/Events/focus_change_event.h"
#include "UICore/UI/Events/key_event.h"
#include "UICore/UI/Events/pointer_event.h"
#include "UICore/UI/Events/resize_event.h"
#include "UICore/UI/UIThread/ui_thread.h"
#include "UICore/Display/2D/canvas.h"
#include "UICore/Display/2D/path.h"
#include "UICore/Display/2D/pen.h"
#include "UICore/Display/2D/brush.h"
#include "UICore/Core/Text/text.h"
#include "view_impl.h"
#include "view_action_impl.h"
#include "flex_layout.h"
#include "custom_layout.h"
#include <algorithm>
#include <set>

namespace uicore
{
	View::View() : impl(new ViewImpl())
	{
		//box_style.set_style_changed(bind_member(this, &View::set_needs_layout));
	}

	View::~View()
	{
		for (auto &child : children())
		{
			child->impl->_parent = nullptr;
			child->impl->update_style_cascade();
		}

		for (auto &action : actions())
		{
			action->impl->view = nullptr;
		}
	}

	const StyleCascade &View::style_cascade() const
	{
		return impl->style_cascade;
	}

	const std::shared_ptr<Style> &View::style(const std::string &state) const
	{
		const auto it = impl->styles.find(state);
		if (it != impl->styles.end())
			return it->second;

		auto &style = impl->styles[state];
		style = std::make_shared<Style>();
		impl->update_style_cascade();
		return style;
	}

	bool View::state(const std::string &name) const
	{
		const auto it = impl->states.find(name);
		if (it != impl->states.end())
			return it->second.enabled;
		else
			return false;
	}
	
	void View::set_state(const std::string &name, bool value)
	{
		if (impl->states[name].enabled != value)
		{
			impl->states[name] = ViewImpl::StyleState(false, value);
			impl->update_style_cascade();
			set_needs_layout();
		}
	}
	void View::set_state_cascade(const std::string &name, bool value)
	{
		if (impl->states[name].enabled != value)
		{
			impl->states[name] = ViewImpl::StyleState(false, value);
			impl->update_style_cascade();
			set_needs_layout();
			impl->set_state_cascade_siblings(name, value);
		}
	}

	void ViewImpl::set_state_cascade_siblings(const std::string &name, bool value)
	{
		for (auto view = _first_child; view != nullptr; view = view->next_sibling())
		{
			ViewImpl *impl = view->impl.get();
			if (impl->states[name].inherited)
			{
				impl->states[name] = ViewImpl::StyleState(true, value);
				impl->update_style_cascade();
				view->set_needs_layout();
				impl->set_state_cascade_siblings(name, value);
			}
		}
	}

	View *View::parent() const
	{
		return impl->_parent;
	}

	std::shared_ptr<View> View::first_child() const
	{
		return impl->_first_child;
	}
	
	std::shared_ptr<View> View::last_child() const
	{
		return impl->_last_child;
	}
	
	std::shared_ptr<View> View::previous_sibling() const
	{
		return impl->_prev_sibling.lock();
	}
	
	std::shared_ptr<View> View::next_sibling() const
	{
		return impl->_next_sibling;
	}
	
	std::shared_ptr<View> View::insert_before(const std::shared_ptr<View> &new_child, const std::shared_ptr<View> &ref_child)
	{
		if (!ref_child)
			return add_child(new_child);
		
		if (!new_child)
			throw Exception("instance not set to an object");
		
		if (ref_child->parent() != this)
			throw Exception("not parent of reference child view");
		
		new_child->remove_from_parent();
		
		auto prev = ref_child->previous_sibling();
		if (prev)
		{
			new_child->impl->_prev_sibling = prev;
			prev->impl->_next_sibling = new_child;
		}
		
		ref_child->impl->_prev_sibling = new_child;
		
		if (impl->_first_child == ref_child)
			impl->_first_child = new_child;
		
		new_child->impl->_parent = this;
		new_child->impl->update_style_cascade();
		new_child->set_needs_layout();
		set_needs_layout();
		
		child_added(new_child);

		return new_child;
	}
	
	std::shared_ptr<View> View::replace_child(const std::shared_ptr<View> &new_child, const std::shared_ptr<View> &old_child)
	{
		if (!new_child || !old_child)
			throw Exception("instance not set to an object");
		
		if (old_child->parent() != this)
			throw Exception("not parent of old child node");
		
		insert_before(new_child, old_child);
		old_child->remove_from_parent();
		
		return old_child;
	}
	
	std::shared_ptr<View> View::add_child(const std::shared_ptr<View> &new_child)
	{
		if (!new_child)
			throw Exception("instance not set to an object");
		
		if (new_child->parent())
			new_child->remove_from_parent();
		
		if (impl->_first_child)
		{
			auto last = last_child();
			last->impl->_next_sibling = new_child;
			new_child->impl->_prev_sibling = last;
			impl->_last_child = new_child;
		}
		else
		{
			impl->_first_child = new_child;
			impl->_last_child = new_child;
		}
		
		new_child->impl->_parent = this;
		new_child->impl->update_style_cascade();
		new_child->set_needs_layout();
		set_needs_layout();
		
		child_added(new_child);

		return new_child;
	}

	void View::remove_from_parent()
	{
		if (!impl->_parent)
			return;
		
		// To do: clear owner_view, focus_view, if it is this view or a child
		
		impl->_parent->set_needs_layout();
		
		auto old_child = shared_from_this();
		
		if (impl->_parent->impl->_first_child == old_child)
			impl->_parent->impl->_first_child = old_child->next_sibling();
		if (impl->_parent->impl->_last_child == old_child)
			impl->_parent->impl->_last_child = old_child->previous_sibling();
		
		auto prev = old_child->previous_sibling();
		if (prev)
			prev->impl->_next_sibling = old_child->impl->_next_sibling;
		if (old_child->impl->_next_sibling)
			old_child->impl->_next_sibling->impl->_prev_sibling = prev;
		
		old_child->impl->_prev_sibling.reset();
		old_child->impl->_next_sibling.reset();
		
		old_child->impl->_parent = nullptr;
		
		child_removed(old_child);
	}

	const std::vector<std::shared_ptr<ViewAction>> &View::actions() const
	{
		return impl->_actions;
	}

	void View::add_action(const std::shared_ptr<ViewAction> &action)
	{
		if (action)
		{
			action->remove_from_view();
			impl->_actions.push_back(action);
			action->impl->view = this;
		}
	}

	bool View::hidden() const
	{
		return impl->hidden;
	}

	void View::set_hidden(bool value)
	{
		if (value != impl->hidden)
		{
			impl->hidden = value;
			set_needs_layout();
		}
	}

	bool View::is_static_position_and_visible() const
	{
		return style_cascade().computed_value("position").is_keyword("static") && !hidden();
	}

	bool View::needs_layout() const
	{
		return impl->needs_layout;
	}

	void View::set_needs_layout()
	{
		impl->needs_layout = true;
		impl->layout_cache.clear();

		View *super = parent();
		if (super)
			super->set_needs_layout();
		else
			set_needs_render();
	}

	CanvasPtr View::canvas() const
	{
		const ViewTree *tree = view_tree();
		if (tree)
			return tree->canvas();
		else
			return nullptr;
	}

	void View::set_needs_render()
	{
		ViewTree *tree = view_tree();
		if (tree)
			tree->set_needs_render();
	}

	const ViewGeometry &View::geometry() const
	{
		return impl->_geometry;
	}

	void View::set_geometry(const ViewGeometry &geometry)
	{
		if (impl->_geometry.content_box() != geometry.content_box())
		{
			impl->_geometry = geometry;
			set_needs_layout();
		}
	}

	bool View::render_exception_encountered() const
	{
		return impl->exception_encountered;
	}

	void View::clear_exception_encountered()
	{
		if (impl->exception_encountered)
		{
			impl->exception_encountered = false;
			set_needs_render();
		}
	}

	const Mat4f &View::view_transform() const
	{
		return impl->view_transform;
	}

	void View::set_view_transform(const Mat4f &transform)
	{
		impl->view_transform = transform;
		set_needs_render();
	}

	bool View::content_clipped() const
	{
		return impl->content_clipped;
	}

	void View::set_content_clipped(bool clipped)
	{
		if (impl->content_clipped != clipped)
		{
			impl->content_clipped = clipped;
			set_needs_render();
		}
	}

	float View::preferred_width(const CanvasPtr &canvas)
	{
		if (!impl->layout_cache.preferred_width_calculated)
		{
			impl->layout_cache.preferred_width = calculate_preferred_width(canvas);
			impl->layout_cache.preferred_width_calculated = true;
		}
		return impl->layout_cache.preferred_width;
	}

	float View::preferred_height(const CanvasPtr &canvas, float width)
	{
		auto it = impl->layout_cache.preferred_height.find(width);
		if (it != impl->layout_cache.preferred_height.end())
			return it->second;

		float height = calculate_preferred_height(canvas, width);
		impl->layout_cache.preferred_height[width] = height;
		return height;
	}

	float View::first_baseline_offset(const CanvasPtr &canvas, float width)
	{
		auto it = impl->layout_cache.first_baseline_offset.find(width);
		if (it != impl->layout_cache.first_baseline_offset.end())
			return it->second;

		float baseline_offset = calculate_first_baseline_offset(canvas, width);
		impl->layout_cache.first_baseline_offset[width] = baseline_offset;
		return baseline_offset;
	}

	float View::last_baseline_offset(const CanvasPtr &canvas, float width)
	{
		auto it = impl->layout_cache.last_baseline_offset.find(width);
		if (it != impl->layout_cache.last_baseline_offset.end())
			return it->second;

		float baseline_offset = calculate_last_baseline_offset(canvas, width);
		impl->layout_cache.last_baseline_offset[width] = baseline_offset;
		return baseline_offset;
	}

	float View::calculate_preferred_width(const CanvasPtr &canvas)
	{
		return impl->active_layout(this)->preferred_width(canvas, this);
	}

	float View::calculate_preferred_height(const CanvasPtr &canvas, float width)
	{
		return impl->active_layout(this)->preferred_height(canvas, this, width);
	}

	float View::calculate_first_baseline_offset(const CanvasPtr &canvas, float width)
	{
		return impl->active_layout(this)->first_baseline_offset(canvas, this, width);
	}

	float View::calculate_last_baseline_offset(const CanvasPtr &canvas, float width)
	{
		return impl->active_layout(this)->last_baseline_offset(canvas, this, width);
	}

	void View::layout_children(const CanvasPtr &canvas)
	{
		return impl->active_layout(this)->layout_children(canvas, this);
	}

	ViewTree *View::view_tree()
	{
		View *super = parent();
		if (super)
			return super->view_tree();
		else
			return impl->view_tree;
	}

	const ViewTree *View::view_tree() const
	{
		View *super = parent();
		if (super)
			return super->view_tree();
		else
			return impl->view_tree;
	}

	View *View::focus_view() const
	{
		const ViewTree *tree = view_tree();
		if (tree)
			return tree->focus_view();
		else
			return nullptr;
	}

	std::shared_ptr<View> View::find_view_at(const Pointf &pos) const
	{
		// Search the children in reverse order, as we want to search the view that was "last drawn" first
		for (auto child = last_child(); child != nullptr; child = child->previous_sibling())
		{
			if (child->geometry().border_box().contains(pos) && !child->hidden())
			{
				Pointf child_content_pos(pos.x - child->geometry().content_x, pos.y - child->geometry().content_y);
				child_content_pos = Vec2f(Mat4f::inverse(child->view_transform()) * Vec4f(child_content_pos, 0.0f, 1.0f));
				std::shared_ptr<View> view = child->find_view_at(child_content_pos);
				if (view)
					return view;
				else
					return child;
			}
		}

		return std::shared_ptr<View>();
	}

	FocusPolicy View::focus_policy() const
	{
		return impl->focus_policy;
	}

	void View::set_focus_policy(FocusPolicy policy)
	{
		impl->focus_policy = policy;
	}

	unsigned int View::tab_index() const
	{
		return impl->tab_index;
	}

	void View::set_tab_index(unsigned int index)
	{
		impl->tab_index = index;
	}

	void View::set_focus()
	{
		ViewTree *tree = view_tree();
		if (!tree)
			return;

		tree->set_focus_view(this);
	}

	void View::remove_focus()
	{
		if (focus_view() != nullptr)
			return;

		ViewTree *tree = view_tree();
		if (!tree)
			return;

		tree->set_focus_view(nullptr);
	}

	void View::prev_focus()
	{
		ViewTree *tree = view_tree();
		if (!tree)
			return;

		View *cur_focus = tree->focus_view();
		View *prev_focus = nullptr;

		if (cur_focus)
		{
			prev_focus = cur_focus->impl->find_prev_with_tab_index(cur_focus->tab_index());
			if (prev_focus == nullptr)
				prev_focus = cur_focus->impl->find_prev_with_tab_index(tree->root_view()->impl->find_prev_tab_index(cur_focus->tab_index()));
		}
		else
		{
			prev_focus = tree->root_view()->impl->find_prev_with_tab_index(tree->root_view()->impl->find_prev_tab_index(0));
		}

		if (prev_focus)
			prev_focus->set_focus();
	}

	void View::next_focus()
	{
		ViewTree *tree = view_tree();
		if (!tree)
			return;

		View *cur_focus = tree->focus_view();
		View *next_focus = nullptr;

		if (cur_focus)
		{
			next_focus = cur_focus->impl->find_next_with_tab_index(cur_focus->tab_index());
			if (next_focus == nullptr)
				next_focus = cur_focus->impl->find_next_with_tab_index(tree->root_view()->impl->find_next_tab_index(cur_focus->tab_index()));
		}
		else
		{
			next_focus = tree->root_view()->impl->find_next_with_tab_index(tree->root_view()->impl->find_next_tab_index(0));
		}

		if (next_focus)
			next_focus->set_focus();
	}

	void View::animate(float from, float to, const std::function<void(float)> &setter, int duration, const std::function<float(float)> &easing, std::function<void()> animation_end)
	{
		impl->animation_group.start(Animation(from, to, setter, duration, easing, animation_end));
	}

	void View::stop_animations()
	{
		impl->animation_group.stop();
	}

	void View::set_cursor(const CursorDescription &cursor)
	{
		if (impl->is_cursor_inherited || impl->cursor_desc != cursor)
		{
			impl->cursor_desc = cursor;
			impl->cursor.reset();
			impl->is_custom_cursor = true;
			impl->is_cursor_inherited = false;
		}
	}

	void View::set_cursor(StandardCursor type)
	{
		if (impl->is_cursor_inherited || impl->is_custom_cursor || impl->cursor_type != type)
		{
			impl->cursor_type = type;
			impl->cursor_desc = CursorDescription();
			impl->cursor.reset();
			impl->is_custom_cursor = false;
			impl->is_cursor_inherited = false;
		}
	}

	void View::set_inherit_cursor()
	{
		if (!impl->is_cursor_inherited)
		{
			impl->cursor_desc = CursorDescription();
			impl->cursor.reset();
			impl->is_custom_cursor = false;
			impl->is_cursor_inherited = true;
		}
	}

	void View::update_cursor(const DisplayWindowPtr &window)
	{
		if (impl->is_cursor_inherited)
		{
			View *super = parent();
			if (super)
				super->update_cursor(window);
			else
				window->set_cursor(StandardCursor::arrow);
		}
		else if (impl->is_custom_cursor)
		{
			if (!impl->cursor)
			{
				impl->cursor = Cursor::create(window, impl->cursor_desc);
			}
			window->set_cursor(impl->cursor);
		}
		else
		{
			window->set_cursor(impl->cursor_type);
		}
	}

	Pointf View::to_screen_pos(const Pointf &pos)
	{
		ViewTree *tree = view_tree();
		Pointf root_content_pos = to_root_pos(pos);
		Pointf root_pos = root_content_pos + tree->root_view()->geometry().content_box().position();
		return tree ? tree->client_to_screen_pos(root_pos) : root_pos;
	}

	Pointf View::from_screen_pos(const Pointf &pos)
	{
		ViewTree *tree = view_tree();
		Pointf root_pos = tree ? tree->screen_to_client_pos(pos) : pos;
		Pointf root_content_pos = root_pos - tree->root_view()->geometry().content_box().position();
		return from_root_pos(root_content_pos);
	}

	Pointf View::to_root_pos(const Pointf &pos)
	{
		if (parent())
			return parent()->to_root_pos(geometry().content_box().position() + Vec2f(view_transform() * Vec4f(pos, 0.0f, 1.0f)));
		else
			return pos;
	}

	Pointf View::from_root_pos(const Pointf &pos)
	{
		if (parent())
			return parent()->from_root_pos(Vec2f(Mat4f::inverse(view_transform()) * Vec4f(pos, 0.0f, 1.0f)) - geometry().content_box().position());
		else
			return pos;
	}

	void View::dispatch_event(EventUI *e, bool no_propagation)
	{
		EventUI simple_event;
		if (e == nullptr)
			e = &simple_event;

		View *target = this;

		if (!target->view_tree())
			return;

		// Make sure root view is not destroyed during event dispatching (needed for dismiss_popup)
		auto pin_root = target->view_tree()->root_view();

		e->_target = target->shared_from_this();

		if (no_propagation)
		{
			e->_phase = EventUIPhase::at_target;
			e->_current_target = e->_target;
			e->_current_target->impl->process_event(e->_current_target.get(), e, true);
			if (!e->propagation_stopped())
				e->_current_target->impl->process_event(e->_current_target.get(), e, false);
		}
		else
		{
			target->impl->inverse_bubble(e, nullptr);

			if (!e->propagation_stopped())
			{
				e->_phase = EventUIPhase::at_target;
				e->_current_target = e->_target;
				e->_current_target->impl->process_event(e->_current_target.get(), e, true);
				if (!e->propagation_stopped())
					e->_current_target->impl->process_event(e->_current_target.get(), e, false);

				while (e->_current_target->parent() && !e->propagation_stopped())
				{
					e->_phase = EventUIPhase::bubbling;
					e->_current_target = e->_current_target->parent()->shared_from_this();
					e->_current_target->impl->process_event(e->_current_target.get(), e, false);
				}
			}
		}

		e->_current_target.reset();
		e->_phase = EventUIPhase::none;
	}

	void View::dispatch_event(EventUI *e, const View *until_parent_view)
	{
		EventUI simple_event;
		if (e == nullptr)
			e = &simple_event;

		View *target = this;

		if (target == until_parent_view)
			return;

		if (!target->view_tree())
			return;

		// Make sure root view is not destroyed during event dispatching (needed for dismiss_popup)
		auto pin_root = target->view_tree()->root_view();

		e->_target = target->shared_from_this();

		target->impl->inverse_bubble(e, until_parent_view);

		if (!e->propagation_stopped())
		{
			e->_phase = EventUIPhase::at_target;
			e->_current_target = e->_target;
			e->_current_target->impl->process_event(e->_current_target.get(), e, true);
			if (!e->propagation_stopped())
				e->_current_target->impl->process_event(e->_current_target.get(), e, false);

			while (e->_current_target->parent() && e->_current_target->parent() != until_parent_view && !e->propagation_stopped())
			{
				e->_phase = EventUIPhase::bubbling;
				e->_current_target = e->_current_target->parent()->shared_from_this();
				e->_current_target->impl->process_event(e->_current_target.get(), e, false);
			}
		}

		e->_current_target.reset();
		e->_phase = EventUIPhase::none;
	}

	View *View::common_parent(View *view1, View *view2)
	{
		std::set<View *> parents;
		if ((view1 == nullptr) || (view2 == nullptr))
			return nullptr;

		for (; view1; view1 = view1->parent())
		{
			parents.insert(view1);
		}
		for (; view2; view2 = view2->parent())
		{
			if (parents.find(view2) != parents.end())
				return view2;
		}
		return nullptr;
	}

	Signal<void(PointerEvent *)> &View::sig_pointer_press()
	{
		return impl->sig_pointer_press;
	}

	Signal<void(PointerEvent *)> &View::sig_pointer_double_click()
	{
		return impl->sig_pointer_double_click;
	}

	Signal<void(PointerEvent *)> &View::sig_pointer_release()
	{
		return impl->sig_pointer_release;
	}

	Signal<void(PointerEvent *)> &View::sig_pointer_move()
	{
		return impl->sig_pointer_move;
	}

	Signal<void(PointerEvent *)> &View::sig_pointer_enter()
	{
		return impl->sig_pointer_enter;
	}

	Signal<void(PointerEvent *)> &View::sig_pointer_leave()
	{
		return impl->sig_pointer_leave;
	}

	Signal<void(PointerEvent *)> &View::sig_pointer_proximity_change()
	{
		return impl->sig_pointer_proximity_change;
	}

	Signal<void(ActivationChangeEvent *)> &View::sig_activated()
	{
		return impl->sig_activated;
	}

	Signal<void(ActivationChangeEvent *)> &View::sig_deactivated()
	{
		return impl->sig_deactivated;
	}

	Signal<void(FocusChangeEvent *)> &View::sig_focus_gained()
	{
		return impl->sig_focus_gained;
	}

	Signal<void(FocusChangeEvent *)> &View::sig_focus_lost()
	{
		return impl->sig_focus_lost;
	}

	Signal<void(KeyEvent *)> &View::sig_key_press()
	{
		return impl->sig_key_press;
	}

	Signal<void(KeyEvent *)> &View::sig_key_release()
	{
		return impl->sig_key_release;
	}

	Signal<void(CloseEvent *)> &View::sig_close()
	{
		return impl->sig_close;
	}

	/////////////////////////////////////////////////////////////////////////

	ViewLayout *ViewImpl::active_layout(View *self)
	{
		if (self->style_cascade().computed_value("layout").is_keyword("flex"))
		{
			return &flex;
		}
		else
		{
			static CustomLayout custom;
			return &custom;
		}
	}

	void ViewImpl::render(View *self, const CanvasPtr &canvas, ViewRenderLayer layer)
	{
		if (layer == ViewRenderLayer::background)
			style_cascade.render_background(canvas, _geometry);
		else if (layer == ViewRenderLayer::border)
			style_cascade.render_border(canvas, _geometry);

		Mat4f old_transform = canvas->transform();
		Pointf translate = _geometry.content_pos();
		canvas->set_transform(old_transform * Mat4f::translate(translate.x, translate.y, 0) * view_transform);

		bool clipped = content_clipped;
		if (clipped)
		{
			// Seems canvas cliprects are always in absolute coordinates - should this be changed?
			// Note: this code isn't correct for rotated transforms (plus canvas cliprect can only clip AABB)
			Vec4f tl_point = canvas->transform() * Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
			Vec4f br_point = canvas->transform() * Vec4f(_geometry.content_width, _geometry.content_height, 0.0f, 1.0f);
			canvas->push_clip(Rectf(std::min(tl_point.x, br_point.x), std::min(tl_point.y, br_point.y), std::max(tl_point.x, br_point.x), std::max(tl_point.y, br_point.y)));
		}

		if (layer == ViewRenderLayer::content)
		{
			if (!self->render_exception_encountered())
			{
				bool success = UIThread::try_catch([&]
				{
					self->render_content(canvas);
				});

				if (!success)
				{
					exception_encountered = true;
				}
			}

			if (self->render_exception_encountered())
			{
				canvas->set_transform(old_transform * Mat4f::translate(translate.x, translate.y, 0));
				Path::rect(0.0f, 0.0f, _geometry.content_width, _geometry.content_height)->fill(canvas, Colorf(1.0f, 0.2f, 0.2f, 0.5f));
				Path::line(0.0f, 0.0f, _geometry.content_width, _geometry.content_height)->stroke(canvas, StandardColorf::black());
				Path::line(_geometry.content_width, 0.0f, 0.0f, _geometry.content_height)->stroke(canvas, StandardColorf::black());
			}
		}

		Rectf clip_box = canvas->clip();
		for (auto view = _first_child; view != nullptr; view = view->next_sibling())
		{
			if (!view->hidden())
			{
				// Note: this code isn't correct for rotated transforms (plus canvas cliprect can only clip AABB)
				Rect border_box = view->geometry().border_box();
				Vec4f tl_point = canvas->transform() * Vec4f(border_box.left, border_box.top, 0.0f, 1.0f);
				Vec4f br_point = canvas->transform() * Vec4f(border_box.right, border_box.bottom, 0.0f, 1.0f);
				Rectf transformed_border_box(std::min(tl_point.x, br_point.x), std::min(tl_point.y, br_point.y), std::max(tl_point.x, br_point.x), std::max(tl_point.y, br_point.y));
				if (clip_box.is_overlapped(transformed_border_box))
				{
					view->impl->render(view.get(), canvas, layer);
				}
			}
		}

		if (clipped)
			canvas->pop_clip();

		canvas->set_transform(old_transform);
	}

	void ViewImpl::update_style_cascade() const
	{
		std::vector<std::pair<Style *, size_t>> matches;

		for (auto it : styles)
		{
			auto &style_list = it.first;
			auto &style = it.second;

			auto style_classes = Text::split(style_list, " ");

			bool match = true;
			for (const auto &state : style_classes)
			{
				auto search_it = states.find(state);
				if (search_it == states.end() || !search_it->second.enabled)
					match = false;
			}

			if (match)
				matches.push_back({ style.get(), style_classes.size() });
		}

		std::stable_sort(matches.begin(), matches.end(), [](const std::pair<Style *, size_t> &a, const std::pair<Style *, size_t> &b) { return a.second != b.second ? a.second > b.second : a.first > b.first; });

		if (_parent)
			style_cascade.parent = &_parent->style_cascade();
		else
			style_cascade.parent = nullptr;

		style_cascade.cascade.clear();
		for (auto &match : matches)
			style_cascade.cascade.push_back(match.first);
	}

	void ViewImpl::process_event_handler(ViewEventHandler *handler, EventUI *e)
	{
		ActivationChangeEvent *activation_change = dynamic_cast<ActivationChangeEvent*>(e);
		CloseEvent *close = dynamic_cast<CloseEvent*>(e);
		ResizeEvent *resize = dynamic_cast<ResizeEvent*>(e);
		FocusChangeEvent *focus_change = dynamic_cast<FocusChangeEvent*>(e);
		PointerEvent *pointer = dynamic_cast<PointerEvent*>(e);
		KeyEvent *key = dynamic_cast<KeyEvent*>(e);

		if (activation_change)
		{
			switch (activation_change->type())
			{
			case ActivationChangeType::activated: handler->activated(activation_change); break;
			case ActivationChangeType::deactivated: handler->deactivated(activation_change); break;
			}
		}
		else if (focus_change)
		{
			switch (focus_change->type())
			{
			case FocusChangeType::gained: handler->focus_gained(focus_change); break;
			case FocusChangeType::lost: handler->focus_lost(focus_change); break;
			}
		}
		else if (pointer)
		{
			switch (pointer->type())
			{
			case PointerEventType::enter: handler->pointer_enter(pointer); break;
			case PointerEventType::leave: handler->pointer_leave(pointer); break;
			case PointerEventType::move: handler->pointer_move(pointer); break;
			case PointerEventType::press: handler->pointer_press(pointer); break;
			case PointerEventType::release: handler->pointer_release(pointer); break;
			case PointerEventType::double_click: handler->pointer_double_click(pointer); break;
			case PointerEventType::promixity_change: handler->pointer_proximity_change(pointer); break;
			case PointerEventType::none: break;
			}
		}
		else if (key)
		{
			switch (key->type())
			{
			case KeyEventType::none: break;
			case KeyEventType::press: handler->key_press(key); break;
			case KeyEventType::release: handler->key_release(key); break;
			}
		}
	}

	void ViewImpl::process_event(View *self, EventUI *e, bool use_capture)
	{
		if (!use_capture)
		{
			if (_active_action)
			{
				process_event_handler(_active_action, e);
			}
			else
			{
				for (auto &action : _actions)
				{
					process_event_handler(action.get(), e);
					if (_active_action || e->propagation_stopped())
						break;
				}
			}

			if (e->propagation_stopped())
				return;
		}

		if (!use_capture)
			process_event_handler(self, e);
	}

	void ViewImpl::inverse_bubble(EventUI *e, const View *until_parent_view)
	{
		if (_parent && _parent != until_parent_view)
		{
			std::shared_ptr<View> super = _parent->shared_from_this();
			super->impl->inverse_bubble(e, until_parent_view);
			if (!e->propagation_stopped())
			{
				e->_phase = EventUIPhase::capturing;
				e->_current_target = super;
				if (e->_current_target)
					e->_current_target->impl->process_event(e->_current_target.get(), e, true);
			}
		}
	}

	unsigned int ViewImpl::find_next_tab_index(unsigned int start_index) const
	{
		unsigned int next_index = tab_index > start_index ? tab_index : 0;
		for (auto child = _first_child; child != nullptr; child = child->next_sibling())
		{
			if (child->hidden()) continue;

			unsigned int next_child_index = child->impl->find_next_tab_index(start_index);
			if (next_child_index != 0)
			{
				if (next_index != 0)
					next_index = uicore::min(next_index, next_child_index);
				else
					next_index = next_child_index;
			}
		}
		return next_index;
	}

	unsigned int ViewImpl::find_prev_tab_index(unsigned int start_index) const
	{
		unsigned int index = find_prev_tab_index_helper(start_index);
		if (index == 0 && start_index > 0)
			index = find_highest_tab_index();
		return index;
	}

	unsigned int ViewImpl::find_prev_tab_index_helper(unsigned int start_index) const
	{
		unsigned int prev_index = tab_index < start_index ? tab_index : 0;
		for (auto child = _first_child; child != nullptr; child = child->next_sibling())
		{
			if (child->hidden()) continue;

			unsigned int prev_child_index = child->impl->find_prev_tab_index_helper(start_index);
			if (prev_child_index != 0)
			{
				if (prev_index != 0)
					prev_index = uicore::max(prev_index, prev_child_index);
				else
					prev_index = prev_child_index;
			}
		}
		return prev_index;
	}

	unsigned int ViewImpl::find_highest_tab_index() const
	{
		unsigned int index = tab_index;
		for (auto child = _first_child; child != nullptr; child = child->next_sibling())
		{
			if (!child->hidden())
				index = uicore::max(child->impl->find_highest_tab_index(), index);
		}
		return index;
	}

	View *ViewImpl::find_next_with_tab_index(unsigned int search_index, const ViewImpl *search_from, bool also_search_ancestors) const
	{
		bool search_from_found = search_from ? false : true;
		for (auto child = _first_child; child != nullptr; child = child->next_sibling())
		{
			if (child->hidden())
				continue;

			if (search_from_found)
			{
				if (child->tab_index() == search_index && child->focus_policy() == FocusPolicy::accept)
				{
					return child.get();
				}
				else
				{
					View *next = child->impl->find_next_with_tab_index(search_index, nullptr, false);
					if (next)
						return next;
				}
			}
			else if (child->impl.get() == search_from)
			{
				search_from_found = true;
			}
		}

		if (!also_search_ancestors || !_parent)
			return nullptr;

		return _parent->impl->find_next_with_tab_index(search_index, this, true);
	}

	View *ViewImpl::find_prev_with_tab_index(unsigned int search_index, const ViewImpl *search_from, bool also_search_ancestors) const
	{
		bool search_from_found = search_from ? false : true;
		for (auto child = _last_child; child != nullptr; child = child->previous_sibling())
		{
			if (child->hidden()) continue;

			if (search_from_found)
			{
				if (child->tab_index() == search_index && child->focus_policy() == FocusPolicy::accept)
				{
					return child.get();
				}
				else
				{
					View *next = child->impl->find_prev_with_tab_index(search_index, nullptr, false);
					if (next)
						return next;
				}
			}
			else if (child->impl.get() == search_from)
			{
				search_from_found = true;
			}
		}

		if (!also_search_ancestors || !_parent)
			return nullptr;

		return _parent->impl->find_prev_with_tab_index(search_index, this, true);
	}

}
