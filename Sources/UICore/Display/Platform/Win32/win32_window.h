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

#include <list>
#include <map>
#include "UICore/Display/Window/display_window.h"
#include "UICore/Display/Window/display_window_description.h"
#include "UICore/Display/Window/input_device.h"
#include "UICore/Display/Window/input_device_provider.h"
#include "UICore/Core/Math/point.h"
#include "UICore/Core/Math/rect.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include <memory>
#include <thread>
#include <condition_variable>
#include <cstdint>

namespace uicore
{
	class InputDeviceProvider_Win32Keyboard;
	class InputDeviceProvider_Win32Mouse;
	class DisplayMessageQueue_Win32;
	class DisplayWindowSite;
	class PixelBuffer;
	class CursorProvider_Win32;
	class InputEvent;
	class Colorf;

	class Win32Window
	{
	public:
		Win32Window();
		~Win32Window();

	public:
		HWND get_hwnd() const { return hwnd; }
		Rect get_geometry() const;
		Rect get_viewport() const;
		float get_pixel_ratio() const { return pixel_ratio; }
		bool has_focus() const;
		bool is_minimized() const;
		bool is_maximized() const;
		bool is_visible() const;
		Size get_minimum_size(bool client_area) const;
		Size get_maximum_size(bool client_area) const;
		std::string get_title() const;
		std::function<void()> &func_on_resized() { return callback_on_resized; }
		bool is_clipboard_text_available() const;
		bool is_clipboard_image_available() const;
		bool is_layered() const { return window_desc.is_layered(); }

		const std::shared_ptr<InputDevice> &get_keyboard() const { return keyboard; }
		const std::shared_ptr<InputDevice> &get_mouse() const { return mouse; }
		const std::vector<std::shared_ptr<InputDevice>> &get_game_controllers() const { return joysticks; }

	public:
		void create(DisplayWindowProvider *site, const DisplayWindowDescription &description, bool no_redirection_bitmap);

		Point client_to_screen(const Point &client);
		Point screen_to_client(const Point &screen);

		void show_system_cursor();
		void set_cursor(CursorProvider_Win32 *cursor);
		void set_cursor(StandardCursor type);
		void hide_system_cursor();
		void set_cursor_handle(HCURSOR cursor);

		void set_large_icon(const std::shared_ptr<PixelBuffer> &image);
		void set_small_icon(const std::shared_ptr<PixelBuffer> &image);

		void set_title(const std::string &new_title);
		void set_enabled(bool enable);

		void set_position(const Rect &pos, bool client_area);
		void set_size(int width, int height, bool client_area);
		void set_minimum_size(int width, int height, bool client_area);
		void set_maximum_size(int width, int height, bool client_area);

		void set_pixel_ratio(float ratio);

		void minimize();
		void restore();
		void maximize();
		void toggle_fullscreen();

		void show(bool activate);
		void hide();

		void bring_to_front();

		void capture_mouse(bool capture);

		void set_clipboard_text(const std::string &text);
		std::string get_clipboard_text() const;
		std::shared_ptr<PixelBuffer> get_clipboard_image() const;
		void set_clipboard_image(const std::shared_ptr<PixelBuffer> &image);
		static std::shared_ptr<PixelBuffer> create_bitmap_data(const std::shared_ptr<PixelBuffer> &image, const Rect &rect);

		static HBITMAP create_bitmap(HDC hdc, const std::shared_ptr<PixelBuffer> &image);
		HICON create_icon(const std::shared_ptr<PixelBuffer> &image) const;

		void request_repaint();

		void set_modifier_keys(InputEvent &key);

		void update_layered(std::shared_ptr<PixelBuffer> &image);

		void set_allow_drop_shadow(bool value) { allow_dropshadow = value; }

		void enable_alpha_channel(const Rect &blur_rect);
		void extend_frame_into_client_area(int left, int top, int right, int bottom);

	private:
		void update_layered_worker_thread();
		void update_layered_worker_thread_process();
		void update_layered_worker_thread_process_dwm();
		void update_dwm_settings();
		void set_alpha_channel();

		LRESULT wm_nc_hittest(WPARAM wparam, LPARAM lparam);
		LRESULT wm_nc_calcsize(WPARAM wparam, LPARAM lparam);
		LRESULT wm_create(WPARAM wparam, LPARAM lparam);
		void resend_nccalcsize();

		static LRESULT WINAPI static_window_proc(
			HWND hWnd,
			UINT uMsg,
			WPARAM wParam,
			LPARAM lParam);
		LRESULT window_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);

		/// \brief Creates the initial window based on the window description.
		void create_new_window(bool no_redirection_bitmap);

		void get_styles_from_description(const DisplayWindowDescription &desc, DWORD &out_style, DWORD &out_ex_style);
		RECT get_window_geometry_from_description(const DisplayWindowDescription &desc, DWORD style, DWORD ex_style);
		void connect_window_input(const DisplayWindowDescription &desc);
		void register_window_class();
		LRESULT wm_dwm_composition_changed(WPARAM wparam, LPARAM lparam);

		void received_keyboard_input(UINT msg, WPARAM wparam, LPARAM lparam);
		void received_mouse_input(UINT msg, WPARAM wparam, LPARAM lparam);
		void received_mouse_move(UINT msg, WPARAM wparam, LPARAM lparam);
		void received_joystick_input(UINT msg, WPARAM wparam, LPARAM lparam);

		void create_hid_devices();

		std::shared_ptr<PixelBuffer> get_argb8888_from_png(uint8_t *data, size_t size) const;
		std::shared_ptr<PixelBuffer> get_argb8888_from_rgb_dib(BITMAPV5HEADER *bitmapInfo, size_t size) const;
		std::shared_ptr<PixelBuffer> get_argb8888_from_bitfields_dib(BITMAPV5HEADER *bitmapInfo, size_t size) const;

		void flip_pixelbuffer_vertical(const std::shared_ptr<PixelBuffer> &pbuf) const;
		void add_png_to_clipboard(const std::shared_ptr<PixelBuffer> &image);
		void add_dib_to_clipboard(const std::shared_ptr<PixelBuffer> &image);
		void register_clipboard_formats();

		InputDeviceProvider_Win32Keyboard *get_keyboard_provider();
		InputDeviceProvider_Win32Mouse *get_mouse_provider();

		HWND hwnd;
		bool destroy_hwnd;
		HCURSOR current_cursor;
		HICON large_icon;
		HICON small_icon;
		bool cursor_set, cursor_hidden;
		DisplayWindowProvider *site;
		std::shared_ptr<InputDevice> keyboard, mouse;
		std::vector<std::shared_ptr<InputDevice>> joysticks;
		Point mouse_pos;
		std::map<int, int> repeat_count;
		std::function<void()> callback_on_resized;
		Size minimum_size;
		Size maximum_size;
		UINT png_clipboard_format;
		std::string class_name;
		bool allow_dropshadow;

		// Default frame dimensions
		static const int TOPEXTENDWIDTH = 27;
		static const int LEFTEXTENDWIDTH = 8;
		static const int RIGHTEXTENDWIDTH = 8;
		static const int BOTTOMEXTENDWIDTH = 8;

		DisplayWindowDescription window_desc;
		Rect window_blur_rect;

		std::thread update_window_worker_thread;
		bool update_window_worker_thread_started;
		std::shared_ptr<PixelBuffer> update_window_image;
		bool update_window_stop_flag = false;
		bool update_window_start_flag = false;
		bool update_window_completed_flag = false;
		std::mutex update_window_mutex;
		std::condition_variable update_window_main_event;
		std::condition_variable update_window_worker_event;
		HRGN update_window_region;
		unsigned int update_window_max_region_rects;

		float pixel_ratio = 1.0f;

		WINDOWPLACEMENT window_positon_before_fullscreen = {};
		DWORD window_style_before_fullscreen;
	};
}
