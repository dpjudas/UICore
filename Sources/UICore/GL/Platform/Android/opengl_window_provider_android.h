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
**    Mark Page
*/

#pragma once

#include "UICore/Display/Window/display_window_provider.h"
#include "UICore/Display/Render/graphic_context.h"
#include <memory>
#include "UICore/GL/opengl_context_description.h"
#include "UICore/GL/opengl_wrap.h"

#include <EGL/egl.h>
#include <GLES/gl.h>

namespace uicore
{

	class OpenGLContextDescription;

	class OpenGLWindowProvider : public DisplayWindowProvider
	{
/// \name Construction
/// \{

	public:
		OpenGLWindowProvider(OpenGLContextDescription &opengl_desc, const DisplayWindowDescription &description);
		~OpenGLWindowProvider();


/// \}
/// \name Attributes
/// \{

	public:
		Rect get_geometry() const;
		Rect get_viewport() const;
		bool is_fullscreen() const;
		bool has_focus() const;
		bool is_minimized() const;
		bool is_maximized() const;
		bool is_visible() const;
		std::string get_title() const;
		Size get_minimum_size(bool client_area) const;
		Size get_maximum_size(bool client_area) const;
		DisplayWindowHandle get_handle() const { return DisplayWindowHandle(); }
		const std::shared_ptr<GraphicContext> &gc() const { return _gc; }
		bool is_clipboard_text_available() const;
		bool is_clipboard_image_available() const;
		std::string get_clipboard_text() const;
		PixelBuffer get_clipboard_image() const;
		float get_pixel_ratio() const;
		ProcAddress *get_proc_address(const std::string& function_name) const;
		bool is_double_buffered() const { return double_buffered; }

		EGLDisplay get_display() const { return display; }
		EGLContext get_context() const { return context; }

/// \}
/// \name Operations
/// \{

	public:
		void make_current() const;
		Pointf client_to_screen(const Pointf &client);
		Pointf screen_to_client(const Pointf &screen);

		void show_system_cursor();
		std::shared_ptr<Cursor> create_cursor(const CursorDescription &cursor_description);
		void set_cursor(const std::shared_ptr<Cursor> &cursor);
		void set_cursor(StandardCursor type);
		void hide_system_cursor();
		void set_title(const std::string &new_title);
		void set_position(const Rectf &pos, bool client_area);
		void set_size(float width, float height, bool client_area);
		void set_minimum_size(float width, float height, bool client_area);
		void set_maximum_size(float width, float height, bool client_area);
		void set_enabled(bool enable);
		void minimize();
		void restore();
		void maximize();
		void toggle_fullscreen();
		void show(bool activate);
		void hide();
		void bring_to_front();

		/// \brief Flip OpenGL buffers.
		void flip(int interval);

		/// \brief Capture/Release the mouse.
		void capture_mouse(bool capture);

		/// \brief Stores text in the clipboard.
		void set_clipboard_text(const std::string &text);

		void set_clipboard_image(const std::shared_ptr<PixelBuffer> &buf);

		/// \brief Invalidates a region of a screen, causing a repaint.
		void request_repaint();

		void set_large_icon(const std::shared_ptr<PixelBuffer> &image);
		void set_small_icon(const std::shared_ptr<PixelBuffer> &image);

		void enable_alpha_channel(const Rectf &blur_rect);
		void extend_frame_into_client_area(float left, float top, float right, float bottom);

		void set_pixel_ratio(float ratio);

		InputDevice &get_keyboard();
		InputDevice &get_mouse();
		std::vector<InputDevice> &get_game_controllers();

/// \}
/// \name Implementation
/// \{

	private:
		void get_opengl_version(int &version_major, int &version_minor);

		std::shared_ptr<GraphicContext> _gc;
		bool using_gl3 = false;
		bool double_buffered = false;
		int swap_interval = 0;

		OpenGLContextDescription opengl_desc;
		DisplayWindowHandle window_handle;
		EGLDisplay display = EGL_NO_DISPLAY;
		EGLSurface surface = EGL_NO_SURFACE;
		EGLContext context = EGL_NO_CONTEXT;
/// \}
	};

}
