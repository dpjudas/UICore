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
**    Kevin J Bluck
**    Mark Page
*/

#include "UICore/precomp.h"
#include "opengl_window_provider_wgl.h"
#include "UICore/Core/Math/rect.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Text/string_format.h"
#include "UICore/Display/Window/display_window_description.h"
#include "UICore/Display/display_target.h"
#include "UICore/Display/Window/display_window_provider.h"
#include "UICore/Display/Window/display_window.h"
#include "UICore/GL/gl_share_list.h"
#include "UICore/GL/opengl.h"
#include "UICore/GL/opengl_wrap.h"
#include "UICore/GL/opengl_context_description.h"
#include "UICore/Display/Platform/Win32/cursor_provider_win32.h"
#include "UICore/Display/Platform/Win32/dwm_functions.h"
#include "../../opengl_context_description_impl.h"
#include "../../GL3/gl3_graphic_context.h"
#include "../../GL1/gl1_graphic_context.h"
#include "opengl_creation_helper.h"
#include <commctrl.h>

namespace uicore
{
	OpenGLWindowProvider::OpenGLWindowProvider(OpenGLContextDescription &opengl_desc, const DisplayWindowDescription &desc) : opengl_desc(opengl_desc)
	{
		win32_window.func_on_resized() = bind_member(this, &OpenGLWindowProvider::on_window_resized);

		fullscreen = desc.is_fullscreen();

		win32_window.create(this, desc);

		if (!opengl_context)
		{
			HWND handle = win32_window.get_hwnd();
			dwm_layered = false;

			if (desc.is_layered() && !DwmFunctions::is_composition_enabled())
			{
				create_shadow_window(handle);
			}
			else
			{
				if (desc.is_layered())
					dwm_layered = true;
			}

			desc.is_layered() ? double_buffered = false : double_buffered = true;	// Only can use Layered windows that are single buffered with OpenGL (via shadow window) ( PFD_DOUBLEBUFFER_DONTCARE set in OpenGLCreationHelper::set_multisampling_pixel_format)

			device_context = GetDC(handle);

			HGLRC share_context = get_share_context();

			OpenGLCreationHelper helper(handle, device_context);
			helper.set_multisampling_pixel_format(desc);

			int gl_major = opengl_desc.get_version_major();
			int gl_minor = opengl_desc.get_version_minor();
			if (opengl_desc.get_allow_lower_versions() == false)
			{
				opengl_context = helper.create_opengl3_context(share_context, gl_major, gl_minor, opengl_desc);
				if (!opengl_context)
					throw Exception(string_format("This application requires OpenGL %1.%2 or above. Try updating your drivers, or upgrade to a newer graphics card.", gl_major, gl_minor));
			}
			else
			{
				static const char opengl_version_list[] =
				{
					// Clanlib supported version pairs
					4, 4,
					4, 3,
					4, 2,
					4, 1,
					4, 0,
					3, 3,
					3, 2,
					3, 1,
					3, 0,
					0, 0,	// End of list
				};

				const char *opengl_version_list_ptr = opengl_version_list;
				do
				{
					int major = *(opengl_version_list_ptr++);
					if (major == 0)
						break;

					int minor = *(opengl_version_list_ptr++);

					// Find the appropriate version in the list
					if (major > gl_major)
						continue;

					if (major == gl_major)
					{
						if (minor > gl_minor)
							continue;
					}

					opengl_context = helper.create_opengl3_context(share_context, major, minor, opengl_desc);
				} while (!opengl_context);

				if (!opengl_context)
					opengl_context = helper.create_opengl2_context(share_context);

				if (!opengl_context)
					throw Exception("This application requires OpenGL. Try updating your drivers, or upgrade to a newer graphics card.");

			}

			bool use_gl3;
			int desc_version_major = opengl_desc.get_version_major();
			int desc_version_minor = opengl_desc.get_version_minor();

			// Do not attempt GL3, if not requested that version
			if (desc_version_major < 3)
			{
				use_gl3 = false;
			}
			else if (!opengl_desc.get_allow_lower_versions())	// Else, if we do not allow lower versions, only attempt GL3
			{
				use_gl3 = true;
			}
			else
			{
				// Choose the target depending on the current opengl version
				int gl_version_major;
				int gl_version_minor;
				get_opengl_version(gl_version_major, gl_version_minor);
				if (gl_version_major < 3)
				{
					use_gl3 = false;
				}
				else
				{
					use_gl3 = true;
				}

			}

			if (use_gl3)
			{
				using_gl3 = true;
				_gc = std::make_shared<GL3GraphicContext>(this);
			}
			else
			{
				using_gl3 = false;
				_gc = std::make_shared<GL1GraphicContext>(this);
			}
		}

		wglSwapIntervalEXT = (ptr_wglSwapIntervalEXT)OpenGL::get_proc_address("wglSwapIntervalEXT");
		swap_interval = desc.swap_interval();
		if (wglSwapIntervalEXT && swap_interval != -1)
			wglSwapIntervalEXT(swap_interval);
	}

	OpenGLWindowProvider::~OpenGLWindowProvider()
	{
		if (opengl_context)
		{
			if (_gc)
			{
				if (using_gl3)
				{
					GL3GraphicContext *gl_provider = dynamic_cast<GL3GraphicContext*>(_gc.get());
					if (gl_provider)
						gl_provider->dispose();
				}
				else
				{
					GL1GraphicContext *gl_provider = dynamic_cast<GL1GraphicContext*>(_gc.get());
					if (gl_provider)
						gl_provider->dispose();
				}
			}

			// Delete the context
			if (wglGetCurrentContext() == opengl_context)
				OpenGL::set_active(0);
			wglDeleteContext(opengl_context);

			opengl_context = 0;
		}
		if (device_context)
		{
			ReleaseDC(win32_window.get_hwnd(), device_context);
			device_context = 0;
			shadow_hwnd = 0;
		}
		if (shadow_window)
		{
			if (shadow_hwnd)
				DestroyWindow(shadow_hwnd);
			shadow_window = false;
		}

		shadow_hwnd = 0;
	}

	// std::list<HGLRC> OpenGLWindowProvider::opengl_contexts;

	ProcAddress *OpenGLWindowProvider::get_proc_address(const std::string& function_name) const
	{
		return (void(*)())wglGetProcAddress(function_name.c_str());
	}

	Rect OpenGLWindowProvider::backing_geometry() const
	{
		return win32_window.get_geometry();
	}

	Rect OpenGLWindowProvider::backing_viewport() const
	{
		return win32_window.get_viewport();
	}

	float OpenGLWindowProvider::pixel_ratio() const
	{
		return win32_window.get_pixel_ratio();
	}

	bool OpenGLWindowProvider::is_fullscreen() const
	{
		return fullscreen;
	}

	bool OpenGLWindowProvider::has_focus() const
	{
		return win32_window.has_focus();
	}

	bool OpenGLWindowProvider::is_minimized() const
	{
		return win32_window.is_minimized();
	}

	bool OpenGLWindowProvider::is_maximized() const
	{
		return win32_window.is_maximized();
	}

	bool OpenGLWindowProvider::is_visible() const
	{
		return win32_window.is_visible();
	}

	Size OpenGLWindowProvider::backing_minimum_size(bool client_area) const
	{
		return win32_window.get_minimum_size(client_area);
	}

	Size OpenGLWindowProvider::backing_maximum_size(bool client_area) const
	{
		return win32_window.get_maximum_size(client_area);
	}

	std::string OpenGLWindowProvider::title() const
	{
		return win32_window.get_title();
	}

	bool OpenGLWindowProvider::is_clipboard_text_available() const
	{
		return win32_window.is_clipboard_text_available();
	}

	bool OpenGLWindowProvider::is_clipboard_image_available() const
	{
		return win32_window.is_clipboard_image_available();
	}

	void OpenGLWindowProvider::make_current() const
	{
		wglMakeCurrent(device_context, opengl_context);
	}

	Point OpenGLWindowProvider::backing_client_to_screen(const Point &client)
	{
		return win32_window.client_to_screen(client);
	}

	Point OpenGLWindowProvider::backing_screen_to_client(const Point &screen)
	{
		return win32_window.screen_to_client(screen);
	}

	void OpenGLWindowProvider::create_shadow_window(HWND wnd)
	{
		WINDOWINFO window_info;
		memset(&window_info, 0, sizeof(WINDOWINFO));
		window_info.cbSize = sizeof(WINDOWINFO);
		GetWindowInfo(wnd, &window_info);
		if (window_info.dwStyle & WS_VISIBLE)
			window_info.dwStyle -= WS_VISIBLE;

		shadow_hwnd = CreateWindowEx(
			0, //window_info.dwExStyle,
			WC_STATIC, //(LPCTSTR) window_info.atomWindowType,
			TEXT(""),
			window_info.dwStyle,
			0, // window_info.rcWindow.left,
			0, // window_info.rcWindow.top,
			window_info.rcWindow.right - window_info.rcWindow.left,
			window_info.rcWindow.bottom - window_info.rcWindow.top,
			GetParent(wnd), 0, GetModuleHandle(0), 0);
		if (shadow_hwnd == 0)
			throw Exception("Unable to create display window (opengl offscreen window)");

		shadow_window = true;
	}

	void OpenGLWindowProvider::get_opengl_version(int &version_major, int &version_minor)
	{
		make_current();
#undef glGetString
		std::string version = (char*)::glGetString(GL_VERSION);

		version_major = 0;
		version_minor = 0;

		std::vector<std::string> split_version = Text::split(version, ".");
		if (split_version.size() > 0)
			version_major = Text::parse_int32(split_version[0]);
		if (split_version.size() > 1)
			version_minor = Text::parse_int32(split_version[1]);

	}

	void OpenGLWindowProvider::on_window_resized()
	{
		if (shadow_window)
		{
			RECT rect = { 0, 0, 0, 0 };
			GetClientRect(win32_window.get_hwnd(), &rect);
			SetWindowPos(shadow_hwnd, 0, 0, 0, rect.right, rect.bottom, SWP_NOREPOSITION | SWP_NOZORDER);
		}

		if (using_gl3)
		{
			GL3GraphicContext *gl_provider = dynamic_cast<GL3GraphicContext*>(_gc.get());
			if (gl_provider)
				gl_provider->on_window_resized();
		}
		else
		{
			GL1GraphicContext *gl_provider = dynamic_cast<GL1GraphicContext*>(_gc.get());
			if (gl_provider)
				gl_provider->on_window_resized();
		}

	}

	void OpenGLWindowProvider::show_system_cursor()
	{
		win32_window.show_system_cursor();
	}

	CursorPtr OpenGLWindowProvider::create_cursor(const CursorDescription &cursor_description)
	{
		return std::make_shared<CursorProvider_Win32>(cursor_description);
	}

	void OpenGLWindowProvider::set_cursor(const CursorPtr &cursor)
	{
		win32_window.set_cursor(static_cast<CursorProvider_Win32 *>(cursor.get()));
	}

	void OpenGLWindowProvider::set_cursor(StandardCursor type)
	{
		win32_window.set_cursor(type);
	}

	void OpenGLWindowProvider::set_cursor_handle(HCURSOR cursor)
	{
		win32_window.set_cursor_handle(cursor);
	}

	void OpenGLWindowProvider::hide_system_cursor()
	{
		win32_window.hide_system_cursor();
	}

	void OpenGLWindowProvider::set_title(const std::string &new_title)
	{
		win32_window.set_title(new_title);
	}

	void OpenGLWindowProvider::set_backing_position(const Rect &pos, bool client_area)
	{
		win32_window.set_position(pos, client_area);
	}

	void OpenGLWindowProvider::set_backing_size(int width, int height, bool client_area)
	{
		win32_window.set_size(width, height, client_area);
	}

	void OpenGLWindowProvider::set_backing_minimum_size(int width, int height, bool client_area)
	{
		win32_window.set_minimum_size(width, height, client_area);
	}

	void OpenGLWindowProvider::set_backing_maximum_size(int width, int height, bool client_area)
	{
		win32_window.set_maximum_size(width, height, client_area);
	}

	void OpenGLWindowProvider::set_enabled(bool enable)
	{
		win32_window.set_enabled(enable);
	}

	void OpenGLWindowProvider::minimize()
	{
		win32_window.minimize();
	}

	void OpenGLWindowProvider::restore()
	{
		win32_window.restore();
	}

	void OpenGLWindowProvider::maximize()
	{
		win32_window.maximize();
	}

	void OpenGLWindowProvider::toggle_fullscreen()
	{
		win32_window.toggle_fullscreen();
	}

	void OpenGLWindowProvider::show(bool activate)
	{
		win32_window.show(activate);
	}

	void OpenGLWindowProvider::hide()
	{
		win32_window.hide();
	}

	void OpenGLWindowProvider::bring_to_front()
	{
		win32_window.bring_to_front();
	}

	void OpenGLWindowProvider::backing_flip(int interval)
	{
		OpenGL::set_active(gc());
		glFlush();
		if (shadow_window)
		{
			int width = viewport().width();
			int height = viewport().height();

			if (using_gl3)
			{
				if (double_buffered)
				{
					glDrawBuffer(GL_BACK);
					glReadBuffer(GL_FRONT);
				}

				auto pixelbuffer = PixelBuffer::create(width, height, tf_bgra8);
				glPixelStorei(GL_PACK_ALIGNMENT, 1);
				glPixelStorei(GL_PACK_ROW_LENGTH, pixelbuffer->pitch() / pixelbuffer->bytes_per_pixel());
				glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
				glPixelStorei(GL_PACK_SKIP_ROWS, 0);
				glReadPixels(
					0, 0,
					width, height,
					GL_BGRA,
					GL_UNSIGNED_BYTE,
					pixelbuffer->data());

				win32_window.update_layered(pixelbuffer);
			}
			else
			{
				GLint old_viewport[4], old_matrix_mode;
				GLfloat old_matrix_projection[16], old_matrix_modelview[16];
				glGetIntegerv(GL_VIEWPORT, old_viewport);
				glGetIntegerv(GL_MATRIX_MODE, &old_matrix_mode);
				glGetFloatv(GL_PROJECTION_MATRIX, old_matrix_projection);
				glGetFloatv(GL_MODELVIEW_MATRIX, old_matrix_modelview);
				GLboolean blending = glIsEnabled(GL_BLEND);
				glDisable(GL_BLEND);

				glViewport(0, 0, width, height);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glMultMatrixf(Mat4f::ortho_2d(0.0f, (float)width, 0.0f, (float)height, handed_right, clip_negative_positive_w));
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();

				if (double_buffered)
				{
					glReadBuffer(GL_BACK);
				}
				glRasterPos2i(0, 0);
				glPixelZoom(1.0f, 1.0f);

				auto pixelbuffer = PixelBuffer::create(width, height, tf_rgba8);
				glPixelStorei(GL_PACK_ALIGNMENT, 1);
				glPixelStorei(GL_PACK_ROW_LENGTH, pixelbuffer->pitch() / pixelbuffer->bytes_per_pixel());
				glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
				glPixelStorei(GL_PACK_SKIP_ROWS, 0);
				glReadPixels(
					0, 0,
					width, height,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					pixelbuffer->data());

				win32_window.update_layered(pixelbuffer);

				if (blending)
					glEnable(GL_BLEND);
				glViewport(old_viewport[0], old_viewport[1], old_viewport[2], old_viewport[3]);
				glMatrixMode(GL_PROJECTION);
				glLoadMatrixf(old_matrix_projection);
				glMatrixMode(GL_MODELVIEW);
				glLoadMatrixf(old_matrix_modelview);
				glMatrixMode(old_matrix_mode);
			}
		}
		else
		{
			if (interval != -1 && interval != swap_interval)
			{
				swap_interval = interval;
				if (wglSwapIntervalEXT)
					wglSwapIntervalEXT(swap_interval);
			}

			BOOL retval = SwapBuffers(get_device_context());

			if (dwm_layered)
			{
				int width = viewport().width();
				int height = viewport().height();

				glReadBuffer(GL_FRONT);

				auto pixelbuffer = PixelBuffer::create(width, height, tf_r8);
				glPixelStorei(GL_PACK_ALIGNMENT, 1);
				glPixelStorei(GL_PACK_ROW_LENGTH, pixelbuffer->pitch() / pixelbuffer->bytes_per_pixel());
				glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
				glPixelStorei(GL_PACK_SKIP_ROWS, 0);
				glReadPixels(
					0, 0,
					width, height,
					GL_ALPHA,
					GL_BYTE, // use GL_BITMAP here for even less transfer?
					pixelbuffer->data());

				win32_window.update_layered(pixelbuffer);
			}
		}
		OpenGL::check_error();
	}

	void OpenGLWindowProvider::capture_mouse(bool capture)
	{
		win32_window.capture_mouse(capture);
	}

	void OpenGLWindowProvider::set_clipboard_text(const std::string &text)
	{
		win32_window.set_clipboard_text(text);
	}

	std::string OpenGLWindowProvider::clipboard_text() const
	{
		return win32_window.get_clipboard_text();
	}

	void OpenGLWindowProvider::request_repaint()
	{
		win32_window.request_repaint();
	}

	void OpenGLWindowProvider::set_large_icon(const PixelBufferPtr &image)
	{
		win32_window.set_large_icon(image);
	}

	void OpenGLWindowProvider::set_small_icon(const PixelBufferPtr &image)
	{
		win32_window.set_small_icon(image);
	}

	void OpenGLWindowProvider::backing_enable_alpha_channel(const Rect &blur_rect)
	{
		win32_window.enable_alpha_channel(blur_rect);
	}

	void OpenGLWindowProvider::backing_extend_frame_into_client_area(int left, int top, int right, int bottom)
	{
		win32_window.extend_frame_into_client_area(left, top, right, bottom);
	}

	HGLRC OpenGLWindowProvider::get_share_context()
	{
		HGLRC share_context = 0;
		GraphicContextImpl* gc = GLShareList::any_context();
		if (gc)
		{
			const DisplayWindowProvider *rwp = NULL;

			GL3GraphicContext *gl3_provider = dynamic_cast<GL3GraphicContext*>(gc);
			if (gl3_provider)
				rwp = &gl3_provider->get_render_window();

			if (!rwp)
			{
				GL1GraphicContext *gl1_provider = dynamic_cast<GL1GraphicContext*>(gc);
				if (gl1_provider)
					rwp = &gl1_provider->get_render_window();
			}

			if (rwp)
			{
				const OpenGLWindowProvider *render_window_wgl = dynamic_cast<const OpenGLWindowProvider*>(rwp);
				if (render_window_wgl)
					share_context = render_window_wgl->opengl_context;
			}
		}
		return share_context;
	}

	void OpenGLWindowProvider::set_clipboard_image(const PixelBufferPtr &buf)
	{
		win32_window.set_clipboard_image(buf);
	}

	PixelBufferPtr OpenGLWindowProvider::clipboard_image() const
	{
		return win32_window.get_clipboard_image();
	}
}
