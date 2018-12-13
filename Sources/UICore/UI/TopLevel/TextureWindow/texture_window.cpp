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
#include "UICore/UI/TopLevel/texture_window.h"
#include "UICore/UI/Events/key_event.h"
#include "UICore/UI/Events/pointer_event.h"
#include "UICore/UI/Events/close_event.h"
#include "UICore/UI/Events/activation_change_event.h"
#include "UICore/Display/Window/input_event.h"
#include "UICore/Display/2D/canvas.h"
#include "texture_window_impl.h"

namespace uicore
{
	TextureWindow::TextureWindow(const std::shared_ptr<Canvas> &canvas) : impl(std::make_shared<TextureWindow_Impl>(this, canvas))
	{
	}

	void TextureWindow::update()
	{
		impl->update();
	}

	void TextureWindow::set_background_color(const Colorf &background_color)
	{
		impl->background_color = background_color;
	}

	void TextureWindow::set_clear_background(bool enable)
	{
		impl->clear_background_enable = enable;
	}

	std::shared_ptr<Canvas> TextureWindow::canvas() const
	{
		return impl->canvas;
	}

	std::shared_ptr<DisplayWindow> TextureWindow::display_window()
	{
		return impl->display_window;
	}

	void TextureWindow::set_needs_render()
	{
		impl->needs_render = true;
	}

	void TextureWindow::set_always_render(bool enable)
	{
		impl->always_render = enable;
	}

	Pointf TextureWindow::client_to_screen_pos(const Pointf &pos)
	{
		return pos;
	}

	Pointf TextureWindow::screen_to_client_pos(const Pointf &pos)
	{
		return pos;
	}

	void TextureWindow::set_viewport(const Rectf &rect)
	{
		if (rect != impl->canvas_rect)
		{
			impl->canvas_rect = rect;
			set_needs_render();
		}
	}

	void TextureWindow::set_window(const std::shared_ptr<DisplayWindow> &window, bool enable_automatic_events, const Mat4f &transform_mouse_matrix)
	{
		impl->set_window(window, enable_automatic_events, transform_mouse_matrix);
	}

	Rectf TextureWindow::viewport() const
	{
		return impl->canvas_rect;
	}

	void TextureWindow::on_window_close()
	{
		impl->on_window_close();
	}

	void TextureWindow::on_lost_focus()
	{
		impl->on_lost_focus();
	}

	void TextureWindow::on_got_focus()
	{
		impl->on_got_focus();
	}

	void TextureWindow::on_key_down(const uicore::InputEvent &e)
	{
		impl->on_key_down(e);
	}

	void TextureWindow::on_key_up(const uicore::InputEvent &e)
	{
		impl->on_key_up(e);
	}

	void TextureWindow::on_mouse_down(const uicore::InputEvent &e)
	{
		impl->on_mouse_down(e);
	}

	void TextureWindow::on_mouse_dblclk(const uicore::InputEvent &e)
	{
		impl->on_mouse_dblclk(e);
	}

	void TextureWindow::on_mouse_up(const uicore::InputEvent &e)
	{
		impl->on_mouse_up(e);
	}

	void TextureWindow::on_mouse_move(const uicore::InputEvent &e)
	{
		impl->on_mouse_move(e);
	}

	void TextureWindow::removing_view(View *view)
	{
		ViewTree::removing_view(view);

		if (impl->hot_view)
		{
			if (impl->hot_view.get() == view || view->has_child(impl->hot_view.get()))
				impl->hot_view.reset();
		}

		if (impl->captured_view)
		{
			if (impl->captured_view.get() == view || view->has_child(impl->captured_view.get()))
				impl->captured_view.reset();
		}
	}
}
