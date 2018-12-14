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
#include <algorithm>
#include <set>

namespace uicore
{
	View::View() : impl(new ViewImpl())
	{
	}

	View::~View()
	{
		for (auto &action : actions())
		{
			action->impl->view = nullptr;
		}
	}

	ViewTheme *View::theme() const
	{
		return impl->theme.get();
	}

	void View::set_theme(std::unique_ptr<ViewTheme> theme)
	{
		impl->theme = std::move(theme);
	}

	View *View::parent() const
	{
		return impl->_parent;
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

	void View::set_needs_layout()
	{
		View *super = parent();
		if (super)
			super->set_needs_layout();
		else
			set_needs_render();
	}

	std::shared_ptr<Canvas> View::canvas() const
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

#if 0
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
#endif

	bool View::has_ancestor(const View *ancestor_view) const
	{
		if (impl->_parent == ancestor_view)
			return true;
		else if (impl->_parent)
			return impl->_parent->has_ancestor(ancestor_view);
		else
			return false;
	}

	bool View::has_child(const View *child_view) const
	{
		return child_view->has_ancestor(this);
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

	void View::update_cursor(const std::shared_ptr<DisplayWindow> &window)
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

	void ViewImpl::render(View *self, const std::shared_ptr<Canvas> &canvas)
	{
		theme->render(canvas, _geometry);

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

		/*
		Rectf clip_box = canvas->clip();
		for (auto view = _first_child; view != nullptr; view = view->next_sibling())
		{
			if (!view->hidden())
			{
				// Note: this code isn't correct for rotated transforms (plus canvas cliprect can only clip AABB)
				Rectf border_box = view->geometry().border_box();
				Vec4f tl_point = canvas->transform() * Vec4f(border_box.left, border_box.top, 0.0f, 1.0f);
				Vec4f br_point = canvas->transform() * Vec4f(border_box.right, border_box.bottom, 0.0f, 1.0f);
				Rectf transformed_border_box(std::min(tl_point.x, br_point.x), std::min(tl_point.y, br_point.y), std::max(tl_point.x, br_point.x), std::max(tl_point.y, br_point.y));
				if (clip_box.is_overlapped(transformed_border_box))
				{
					view->impl->render(view.get(), canvas);
				}
			}
		}
		*/

		if (clipped)
			canvas->pop_clip();

		canvas->set_transform(old_transform);
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
}
