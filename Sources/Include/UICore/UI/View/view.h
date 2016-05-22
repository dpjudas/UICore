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

#pragma once

#include "../../Core/Math/mat4.h"
#include "../../Core/Math/rect.h"
#include "../../Core/Math/easing.h"
#include "../../Core/Signals/signal.h"
#include "../../UI/Events/event.h"
#include "../Style/style.h"
#include "../Style/style_cascade.h"
#include "../Style/style_get_value.h"
#include "view_event_handler.h"
#include "view_geometry.h"
#include "focus_policy.h"
#include <vector>
#include <memory>
#include <functional>

namespace uicore
{
	class Style;
	class StyleCascade;
	class Canvas;
	typedef std::shared_ptr<Canvas> CanvasPtr;
	class EventUI;
	class ActivationChangeEvent;
	class CloseEvent;
	class FocusChangeEvent;
	class PointerEvent;
	class ResizeEvent;
	class KeyEvent;
	class ViewImpl;
	class CursorDescription;
	enum class StandardCursor;
	class DisplayWindow;
	typedef std::shared_ptr<DisplayWindow> DisplayWindowPtr;
	class ViewTree;
	class ViewAction;

	/// View for an area of the user interface
	class View : public std::enable_shared_from_this<View>, public ViewEventHandler
	{
	public:
		View();
		virtual ~View();
		
		/// Style cascade currently active for this view
		const StyleCascade &style_cascade() const;

		/// Style properties for the specified state
		const std::shared_ptr<Style> &style(const std::string &state = std::string()) const;
		
		/// Test if a style state is currently set
		bool state(const std::string &name) const;

		/// Set or clear style state
		void set_state(const std::string &name, bool value);

		/// Sets the state for this view and all siblings recursively, until a manually set state of the same name is found
		void set_state_cascade(const std::string &name, bool value);

		/// Parent view node or nullptr if the view is the current root node
		View *parent() const;
		
		/// Returns the first child view
		std::shared_ptr<View> first_child() const;
		
		/// Returns the last child ivew
		std::shared_ptr<View> last_child() const;
		
		/// Returns the previous sibling view
		std::shared_ptr<View> previous_sibling() const;
		
		/// Returns the next sibling view
		std::shared_ptr<View> next_sibling() const;
		
		/// Inserts a view before the specified reference child
		std::shared_ptr<View> insert_before(const std::shared_ptr<View> &new_child, const std::shared_ptr<View> &ref_child);
		
		/// Replaces a child view with another view
		std::shared_ptr<View> replace_child(const std::shared_ptr<View> &new_child, const std::shared_ptr<View> &old_child);

		struct Children
		{
			Children(const View *v) : view(v) { }
			
			struct iterator
			{
				iterator() { }
				iterator(std::shared_ptr<View> v) : _view(v) { }
				const std::shared_ptr<View> &operator*() const { return _view; }
				iterator &operator++() { _view = _view->next_sibling(); return *this; }
				bool operator==(const iterator &other) const { return _view == other._view; }
				bool operator!=(const iterator &other) const { return _view != other._view; }
			private:
				std::shared_ptr<View> _view;
			};
			typedef iterator const_iterator;
			
			iterator begin() { return view->first_child(); }
			const_iterator begin() const { return view->first_child(); }
			iterator end() { return iterator(); }
			const_iterator end() const { return const_iterator(); }

		private:
			const View *view;
		};
		
		/// List of all immediate child views
		Children children() const { return this; }

		/// Add a child view
		std::shared_ptr<View> add_child(const std::shared_ptr<View> &view);

		template<typename T, typename... Types>
		std::shared_ptr<T> add_child(Types &&... args)
		{
			auto child = std::make_shared<T>(std::forward<Types>(args)...);
			add_child(child);
			return child;
		}

		std::shared_ptr<View> add_child()
		{
			return add_child<View>();
		}

		/// Remove view from parent
		void remove_from_parent();

		/// Add an action recognizer
		void add_action(const std::shared_ptr<ViewAction> &action);

		template<typename T, typename... Types>
		std::shared_ptr<T> add_action(Types &&... args)
		{
			auto action = std::make_shared<T>(std::forward<Types>(args)...);
			add_action(action);
			return action;
		}

		/// List of all action recognizers
		const std::vector<std::shared_ptr<ViewAction>> &actions() const;

		/// Test if view is set to hidden
		bool hidden() const;

		/// Hides a view from layout and rendering
		void set_hidden(bool value = true);

		/// Test if view should participate in static layout calculations (layout_children)
		bool is_static_position_and_visible() const;

		/// Test if view geometry needs to be recalculated
		bool needs_layout() const;

		/// Forces recalculation of view geometry before next rendering
		void set_needs_layout();

		/// Actual view position and size after layout
		const ViewGeometry &geometry() const;

		/// Sets the view position and size
		///
		/// This function should only be called by layout_children.
		void set_geometry(const ViewGeometry &geometry);

		/// Gets the current canvas used to render this view
		///
		/// This function may return a null canvas if the view does not have a canvas attached to it yet.
		CanvasPtr canvas() const;

		/// Signals this view needs to be rendered again
		void set_needs_render();

		/// Test if this view generated an exception during rendering
		bool render_exception_encountered() const;

		/// Clears exception encountered flag
		///
		/// If a view generates an exception during rendering the view's render_content function will not be called again until this function is called.
		void clear_exception_encountered();

		/// Current view transform
		const Mat4f &view_transform() const;

		/// Specifies the view transform to be applied before its contents and children are rendered
		void set_view_transform(const Mat4f &transform);

		/// Content clipping flag
		bool content_clipped() const;

		/// Specifies if content should be clipped during rendering
		void set_content_clipped(bool clipped);

		/// Calculates the preferred width of this view
		float preferred_width(const CanvasPtr &canvas);

		/// Calculates the preferred height of this view
		float preferred_height(const CanvasPtr &canvas, float width);

		/// Calculates the offset to the first baseline
		float first_baseline_offset(const CanvasPtr &canvas, float width);

		/// Calculates the offset to the last baseline
		float last_baseline_offset(const CanvasPtr &canvas, float width);

		/// Sets the view geometry for all children of this view
		virtual void layout_children(const CanvasPtr &canvas);

		/// Tree in view hierachy
		const ViewTree *view_tree() const;
		ViewTree *view_tree();

		/// The view receiving keyboard events or nullptr if no view has the focus
		View *focus_view() const;

		/// Find descendant view at the specified content relative position
		std::shared_ptr<View> find_view_at(const Pointf &pos) const;

		/// Focus policy active for this view
		FocusPolicy focus_policy() const;

		/// Set if this view automatically can gain focus
		void set_focus_policy(FocusPolicy policy);

		/// Tab index for keyboard focus changes
		unsigned int tab_index() const;

		/// Sets the tab index used for keyboard focus changes
		void set_tab_index(unsigned int index);

		/// Set this view as the focused view
		void set_focus();

		/// Remove focus from this view
		void remove_focus();

		/// Test if this view is receiving keyboard input
		bool has_focus() const { return focus_view() == this; }

		/// Give focus to the previous view in the keyboard tab index order
		void prev_focus();

		/// Give focus to the next view in the keyboard tab index order
		void next_focus();

		/// Continously call an animation function for the specified duration
		void animate(float from, float to, const std::function<void(float)> &setter, int duration_ms = 400, const std::function<float(float)> &easing = Easing::linear, std::function<void()> animation_end = std::function<void()>());

		/// Stop all activate animation functions
		void stop_animations();

		/// Set the cursor icon used when cursor is above this view
		void set_cursor(const CursorDescription &cursor);
		void set_cursor(StandardCursor type);

		/// Specify that the cursor icon is inherited from the parent view
		void set_inherit_cursor();

		Signal<void(PointerEvent *)> &sig_pointer_press();
		Signal<void(PointerEvent *)> &sig_pointer_double_click();
		Signal<void(PointerEvent *)> &sig_pointer_release();
		Signal<void(PointerEvent *)> &sig_pointer_move();
		Signal<void(PointerEvent *)> &sig_pointer_enter();
		Signal<void(PointerEvent *)> &sig_pointer_leave();
		Signal<void(PointerEvent *)> &sig_pointer_proximity_change();
		Signal<void(ActivationChangeEvent *)> &sig_activated();
		Signal<void(ActivationChangeEvent *)> &sig_deactivated();
		Signal<void(FocusChangeEvent *)> &sig_focus_gained();
		Signal<void(FocusChangeEvent *)> &sig_focus_lost();
		Signal<void(KeyEvent *)> &sig_key_press();
		Signal<void(KeyEvent *)> &sig_key_release();
		Signal<void(CloseEvent *)> &sig_close();

		void pointer_press(PointerEvent *e) override { sig_pointer_press()(e); }
		void pointer_double_click(PointerEvent *e) override { sig_pointer_double_click()(e); }
		void pointer_release(PointerEvent *e) override { sig_pointer_release()(e); }
		void pointer_move(PointerEvent *e) override { sig_pointer_move()(e); }
		void pointer_enter(PointerEvent *e) override { sig_pointer_enter()(e); }
		void pointer_leave(PointerEvent *e) override { sig_pointer_leave()(e); }
		void pointer_proximity_change(PointerEvent *e) override { sig_pointer_proximity_change()(e); }
		void activated(ActivationChangeEvent *e) override { sig_activated()(e); }
		void deactivated(ActivationChangeEvent *e) override { sig_deactivated()(e); }
		void focus_gained(FocusChangeEvent *e) override { sig_focus_gained()(e); }
		void focus_lost(FocusChangeEvent *e) override { sig_focus_lost()(e); }
		void key_press(KeyEvent *e) override { sig_key_press()(e); }
		void key_release(KeyEvent *e) override { sig_key_release()(e); }
		void close(CloseEvent *e) override { sig_close()(e); }

		/// Dispatch view event
		///
		/// \param until_parent_view = Dispatch until current target is this view, then propagation stops 
		void dispatch_event(EventUI *e, bool no_propagation = false);
		void dispatch_event(EventUI *e, const View *until_parent_view);

		/// Find the common parent view for the specified views
		static View *common_parent(View *view1, View *view2);

		/// Update window cursor to the cursor used by this view
		void update_cursor(const DisplayWindowPtr &window);

		/// Map from local content to screen coordinates
		Pointf to_screen_pos(const Pointf &pos);

		/// Map from screen to local content coordinates
		Pointf from_screen_pos(const Pointf &pos);

		/// Map from local content to root content coordinates
		Pointf to_root_pos(const Pointf &pos);

		/// Map from root content to local content coordinates
		Pointf from_root_pos(const Pointf &pos);

		/// Slot container helping with automatic disconnection of connected slots when the view is destroyed
		SlotContainer slots;

	protected:
		/// Renders the content of a view
		virtual void render_content(const CanvasPtr &canvas) { }

		/// Child view was added to this view
		virtual void child_added(const std::shared_ptr<View> &view) { }

		/// Child view was removed from this view
		virtual void child_removed(const std::shared_ptr<View> &view) { }

		/// Calculates the preferred width of this view
		virtual float calculate_preferred_width(const CanvasPtr &canvas);

		/// Calculates the preferred height of this view
		virtual float calculate_preferred_height(const CanvasPtr &canvas, float width);

		/// Calculates the offset to the first baseline
		virtual float calculate_first_baseline_offset(const CanvasPtr &canvas, float width);

		/// Calculates the offset to the last baseline
		virtual float calculate_last_baseline_offset(const CanvasPtr &canvas, float width);

	private:
		View(const View &) = delete;
		View &operator=(const View &) = delete;

		std::unique_ptr<ViewImpl> impl;

		friend class ViewTree;
		friend class ViewImpl;
		friend class ViewAction;
	};
}
