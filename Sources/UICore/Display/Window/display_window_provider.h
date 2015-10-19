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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#pragma once

#include "UICore/Core/Signals/signal.h"
#include "UICore/Display/Window/display_window.h"
#include "UICore/Core/Math/rect.h"
#include "UICore/Core/Math/size.h"
#include <memory>

namespace uicore
{
	class Size;
	class Point;
	class DisplayWindowDescription;
	class GraphicContext;
	class InputDevice;
	class Cursor;
	typedef std::shared_ptr<Cursor> CursorPtr;
	class CursorDescription;

	class DisplayWindowProvider : public DisplayWindow
	{
	public:
		virtual Rect backing_geometry() const = 0;
		virtual Rect backing_viewport() const = 0;
		virtual Size backing_minimum_size(bool client_area) const = 0;
		virtual Size backing_maximum_size(bool client_area) const = 0;
		virtual Point backing_client_to_screen(const Point &client) = 0;
		virtual Point backing_screen_to_client(const Point &screen) = 0;
		virtual void set_backing_position(const Rect &pos, bool client_area) = 0;
		virtual void set_backing_size(int width, int height, bool client_area) = 0;
		virtual void set_backing_minimum_size(int width, int height, bool client_area) = 0;
		virtual void set_backing_maximum_size(int width, int height, bool client_area) = 0;
		virtual void backing_enable_alpha_channel(const Rect &blur_rect) = 0;
		virtual void backing_extend_frame_into_client_area(int left, int top, int right, int bottom) = 0;
		virtual float pixel_ratio() const = 0;
		virtual void backing_flip(int interval) = 0;

		virtual CursorPtr create_cursor(const CursorDescription &cursor_description) = 0;

		Signal<void()> &sig_lost_focus() override { return _sig_lost_focus; }
		Signal<void()> &sig_got_focus() override { return _sig_got_focus; }
		Signal<void(float, float)> &sig_resize() override { return _sig_resize; }
		Signal<void()> &sig_paint() override { return _sig_paint; }
		Signal<void()> &sig_window_close() override { return _sig_window_close; }
		Signal<void()> &sig_window_destroy() override { return _sig_window_destroy; }
		Signal<void()> &sig_window_minimized() override { return _sig_window_minimized; }
		Signal<void()> &sig_window_maximized() override { return _sig_window_maximized; }
		Signal<void()> &sig_window_restored() override { return _sig_window_restored; }
		Signal<void()> &sig_window_moved() override { return _sig_window_moved; }
		Signal<void()> &sig_window_flip() override { return _sig_window_flip; }
		std::function<void(Rectf &)> &func_window_resize() override { return _func_window_resize; }
		std::function<bool()> &func_minimize_clicked() override { return _func_minimize_clicked; }
#ifdef WIN32
		std::function<bool(HWND, UINT, WPARAM, LPARAM)> &func_window_message() override { return _func_window_message; }
		Signal<void(HWND, UINT, WPARAM, LPARAM)> &sig_window_message() override { return _sig_window_message; }
#endif

		virtual void show_system_cursor() = 0;
		virtual void hide_system_cursor() = 0;

		void show_cursor() override { show_system_cursor(); }
		void hide_cursor() override { hide_system_cursor(); }

		Rectf geometry() const override
		{
			Rect geometryi = backing_geometry();
			Rectf geometry;
			geometry.left = geometryi.left / pixel_ratio();
			geometry.top = geometryi.top / pixel_ratio();
			geometry.right = geometryi.right / pixel_ratio();
			geometry.bottom = geometryi.bottom / pixel_ratio();
			return geometry;
		}

		Rectf viewport() const override
		{
			Rect viewporti = backing_viewport();
			Rectf viewport;
			viewport.left = viewporti.left / pixel_ratio();
			viewport.top = viewporti.top / pixel_ratio();
			viewport.right = viewporti.right / pixel_ratio();
			viewport.bottom = viewporti.bottom / pixel_ratio();
			return viewport;
		}

		Sizef minimum_size(bool client_area) override
		{
			Size sizei = backing_minimum_size(client_area);
			Sizef sizef;
			sizef.width = sizei.width / pixel_ratio();
			sizef.height = sizei.height / pixel_ratio();
			return sizef;
		}

		Sizef maximum_size(bool client_area) override
		{
			Size sizei = backing_maximum_size(client_area);
			Sizef sizef;
			sizef.width = sizei.width / pixel_ratio();
			sizef.height = sizei.height / pixel_ratio();
			return sizef;
		}

		Pointf client_to_screen(const Pointf &client) override
		{
			Point clienti;
			clienti.x = (int)std::round(client.x * pixel_ratio());
			clienti.y = (int)std::round(client.y * pixel_ratio());
			Point screeni = backing_client_to_screen(clienti);
			Pointf screen;
			screen.x = screeni.x / pixel_ratio();
			screen.y = screeni.y / pixel_ratio();
			return screen;
		}

		Pointf screen_to_client(const Pointf &screen) override
		{
			Point screeni;
			screeni.x = (int)std::round(screen.x * pixel_ratio());
			screeni.y = (int)std::round(screen.y * pixel_ratio());
			Point clienti = backing_screen_to_client(screeni);
			Pointf client;
			client.x = clienti.x / pixel_ratio();
			client.y = clienti.y / pixel_ratio();
			return client;
		}

		void set_position(const Rectf &rect, bool client_area) override
		{
			Rect recti;
			recti.left = (int)std::round(rect.left * pixel_ratio());
			recti.top = (int)std::round(rect.top * pixel_ratio());
			recti.right = (int)std::round(rect.right * pixel_ratio());
			recti.bottom = (int)std::round(rect.bottom * pixel_ratio());
			set_backing_position(recti, client_area);
		}

		void set_position(float x, float y) override
		{
			int xi = (int)std::round(x * pixel_ratio());
			int yi = (int)std::round(y * pixel_ratio());
			Rect geometry = backing_geometry();
			set_backing_position(Rect(xi, yi, xi + geometry.width(), yi + geometry.height()), false);
		}

		void set_size(float width, float height, bool client_area) override
		{
			int widthi = (int)std::round(width * pixel_ratio());
			int heighti = (int)std::round(height * pixel_ratio());
			set_backing_size(widthi, heighti, client_area);
		}

		void set_minimum_size(float width, float height, bool client_area) override
		{
			int widthi = (int)std::round(width * pixel_ratio());
			int heighti = (int)std::round(height * pixel_ratio());
			set_backing_minimum_size(widthi, heighti, client_area);
		}

		void set_maximum_size(float width, float height, bool client_area) override
		{
			int widthi = (int)std::round(width * pixel_ratio());
			int heighti = (int)std::round(height * pixel_ratio());
			set_backing_maximum_size(widthi, heighti, client_area);
		}

		void enable_alpha_channel(const Rectf &blur_rect) override
		{
			Rect blur_recti;
			blur_recti.left = (int)std::round(blur_rect.left * pixel_ratio());
			blur_recti.top = (int)std::round(blur_rect.top * pixel_ratio());
			blur_recti.right = (int)std::round(blur_rect.right * pixel_ratio());
			blur_recti.bottom = (int)std::round(blur_rect.bottom * pixel_ratio());
			backing_enable_alpha_channel(blur_recti);
		}

		void extend_frame_into_client_area(float left, float top, float right, float bottom) override
		{
			int lefti = (int)std::round(left * pixel_ratio());
			int topi = (int)std::round(top * pixel_ratio());
			int righti = (int)std::round(right * pixel_ratio());
			int bottomi = (int)std::round(bottom * pixel_ratio());
			backing_extend_frame_into_client_area(lefti, topi, righti, bottomi);
		}

		void flip(int interval) override
		{
			_sig_window_flip();
			backing_flip(interval);
		}

	private:
		Signal<void()> _sig_lost_focus;
		Signal<void()> _sig_got_focus;
		Signal<void(float, float)> _sig_resize;
		Signal<void()> _sig_paint;
		Signal<void()> _sig_window_close;
		Signal<void()> _sig_window_destroy;
		Signal<void()> _sig_window_minimized;
		Signal<void()> _sig_window_maximized;
		Signal<void()> _sig_window_restored;
		std::function<void(Rectf &)> _func_window_resize;
		std::function<bool()> _func_minimize_clicked;
		Signal<void()> _sig_window_moved;
		Signal<void()> _sig_window_flip;

#ifdef WIN32
		std::function<bool(HWND, UINT, WPARAM, LPARAM)> _func_window_message;
		Signal<void(HWND, UINT, WPARAM, LPARAM)> _sig_window_message;
#endif
	};
}
