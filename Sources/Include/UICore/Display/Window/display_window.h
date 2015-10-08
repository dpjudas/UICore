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
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#pragma once

#include "../../Core/Signals/signal.h"
#include "../display_target.h"
#include <memory>

#if !defined(WIN32) && !defined(__ANDROID__) && !defined(__APPLE__)
// We prefer not to include Xlib.h in uicorelib (to prevent namespace issues when "using namespace uicore")
struct _XDisplay;
typedef struct _XDisplay Display;
typedef unsigned long Window;
#endif

#if defined(__ANDROID__)
struct ANativeWindow;
#endif

namespace uicore
{
	class Sizef;
	class Rectf;
	class Display;
	class GraphicContext;
	typedef std::shared_ptr<GraphicContext> GraphicContextPtr;
	class DisplayWindowMode;
	class DisplayWindowDescription;
	struct DisplayWindowHandle;
	class InputDevice;
	typedef std::shared_ptr<InputDevice> InputDevicePtr;
	class Pointf;
	class Cursor;
	typedef std::shared_ptr<Cursor> CursorPtr;
	class DisplayWindowProvider;
	class DisplayWindow_Impl;
	class PixelBuffer;
	typedef std::shared_ptr<PixelBuffer> PixelBufferPtr;

	/// \brief Standard Cursor
	enum class StandardCursor
	{
		arrow,
		appstarting,
		cross,
		hand,
		ibeam,
		no,
		size_all,
		size_nesw,
		size_ns,
		size_nwse,
		size_we,
		uparrow,
		wait
	};

	struct DisplayWindowHandle
	{
#ifdef WIN32
		HWND hwnd = 0;
#elif defined(__ANDROID__)
		ANativeWindow *window = 0;
#elif defined(__APPLE__)
#else
		::Display *display = 0;
		::Window window = 0;
		int screen = -1;
#endif
	};

	/// \brief Top-level window class.
	class DisplayWindow
	{
	public:
		/// \brief Constructs a null instance.
		DisplayWindow();

		/// \brief Constructs a window.
		///
		/// \param title = Titlebar text.
		/// \param width = Width in pixels of window.
		/// \param height = Height in pixels of window.
		/// \param start_fullscreen = If true, window will go fullscreen.
		/// \param allow_resize = If true, window will have resize grabbers and can be resized.
		/// \param flipping_buffers = Number of flipping buffers in system. Default is that there is a front buffer and a back buffer.
		/// \param description = Structure that describes how to create the display window.
		static std::shared_ptr<DisplayWindow> create(const std::string &title, float width, float height, bool start_fullscreen = false, bool allow_resize = true, int flipping_buffers = 2);
		static std::shared_ptr<DisplayWindow> create(const DisplayWindowDescription &description);

		/// \brief Returns the position and size of the window frame.
		virtual Rectf geometry() const = 0;

		/// \brief Returns the drawable area of the window (excluding window frame).
		virtual Rectf viewport() const = 0;

		/// \brief Returns true if window is currently running fullscreen.
		virtual bool is_fullscreen() const = 0;

		/// \brief Returns true if window has focus.
		virtual bool has_focus() const = 0;

		/// \brief Return the graphic context for the window.
		virtual const GraphicContextPtr &gc() const = 0;

		/// \brief Returns the keyboard input device.
		virtual const InputDevicePtr &keyboard() const = 0;

		/// \brief Returns the mouse input device.
		virtual const InputDevicePtr &mouse() const = 0;

		/// \brief Returns the game controller input device.
		virtual const std::vector<InputDevicePtr> &game_controllers() const = 0;

		/// \brief Returns the input device with the given device name
		const InputDevicePtr &input_device(const std::string &device_name) const;

		/// \brief Signal emitted when window lost focus.
		virtual Signal<void()> &sig_lost_focus() = 0;

		/// \brief Signal emitted when window gain focus.
		virtual Signal<void()> &sig_got_focus() = 0;

		/// \brief Signal emitted when window is resized.
		virtual Signal<void(float, float)> &sig_resize() = 0;

		/// \brief Signal emitted when the window is invalidated.
		virtual Signal<void()> &sig_paint() = 0;

		/// \brief Signal emitted when window is closed.
		virtual Signal<void()> &sig_window_close() = 0;

		/// \brief Signal emitted when window is destroyed.
		virtual Signal<void()> &sig_window_destroy() = 0;

		/// \brief Signal emitted when window is minimized.
		virtual Signal<void()> &sig_window_minimized() = 0;

		/// \brief Signal emitted when window is maximized.
		virtual Signal<void()> &sig_window_maximized() = 0;

		/// \brief Signal emitted when window is restored.
		virtual Signal<void()> &sig_window_restored() = 0;

		/// \brief Signal emitted after a window has been moved.
		virtual Signal<void()> &sig_window_moved() = 0;

		/// \brief Signal emitted when window flip() was called
		virtual Signal<void()> &sig_window_flip() = 0;

		/// \brief Callback called when a window is being resized.
		virtual std::function<void(Rectf &)> &func_window_resize() = 0;

		/// \brief Callback called when a window is asked to minimize itself.
		virtual std::function<bool()> &func_minimize_clicked() = 0;

#ifdef WIN32
		/// \brief Callback called when a windows messages is received.
		virtual std::function<bool(HWND, UINT, WPARAM, LPARAM)> &func_window_message() = 0;

		/// \brief Signal called when a windows messages is received.
		virtual Signal<void(HWND, UINT, WPARAM, LPARAM)> &sig_window_message() = 0;
#endif

		/// \brief returns true if this display window is visible
		virtual bool is_visible() const = 0;

		/// \brief Returns true if the window is minimized.
		virtual bool is_minimized() const = 0;

		/// \brief Returns true if the window is maximized.
		virtual bool is_maximized() const = 0;

		/// \brief Returns true if text is available in the clipboard.
		virtual bool is_clipboard_text_available() const = 0;

		/// \brief Returns true if an image is available in the clipboard.
		virtual bool is_clipboard_image_available() const = 0;

		/// \brief Returns the text stored in the clipboard.
		virtual std::string clipboard_text() const = 0;

		/// \brief Returns an image stored in the clipboard.
		/// Returns a null pixelbuffer if no image is available.
		virtual PixelBufferPtr clipboard_image() const = 0;

		/// \brief Returns the minimum size the window can be resized to by the application user.
		virtual Sizef minimum_size(bool client_area = false) = 0;

		/// \brief Returns the maximum size the window can be resized to by the application user.
		virtual Sizef maximum_size(bool client_area = false) = 0;

		/// \brief Returns the window title.
		virtual std::string title() const = 0;

		/// Returns an platform-specific internal display window handle object.
		virtual DisplayWindowHandle handle() const = 0;

		/// \brief Convert from window client coordinates to screen coordinates.
		virtual Pointf client_to_screen(const Pointf &client) = 0;

		/// \brief Convert from screen coordinates to client coordinates.
		virtual Pointf screen_to_client(const Pointf &screen) = 0;

		/// \brief Capture/Release the mouse.
		virtual void capture_mouse(bool capture) = 0;

		/// \brief Invalidates the screen, causing a repaint.
		virtual void request_repaint() = 0;

		/// \brief Change window title.
		virtual void set_title(const std::string &title) = 0;

		/// \brief Set window position and size.
		///
		/// \param pos = Window position and size.
		/// \param client_area = true - Position relative to window client area
		virtual void set_position(const Rectf &pos, bool client_area) = 0;

		/// \brief Set window position.
		///
		/// \param x Window x position on desktop.
		/// \param y Window y position on desktop.
		virtual void set_position(float x, float y) = 0;

		/// \brief Set enabled
		virtual void set_enabled(bool enable) = 0;

		/// \brief Set visible
		void set_visible(bool visible, bool activate);

		/// \brief Resize window.
		///
		/// \param width = New width of window in pixels.
		/// \param height = New height of window in pixels.
		/// \param client_area = true - relative to the window client area
		virtual void set_size(float width, float height, bool client_area) = 0;

		/// \brief Minimum size a window can be resized to by the application user.
		virtual void set_minimum_size(float width, float height, bool client_area) = 0;

		/// \brief Maximum size a window can be resized to by the application user.
		virtual void set_maximum_size(float width, float height, bool client_area) = 0;

		/// \brief Minimizes the window.
		virtual void minimize() = 0;

		/// \brief Restores the window.
		virtual void restore() = 0;

		/// \brief Maximizes the window.
		virtual void maximize() = 0;

		/// Toggle fullscreen
		/// Only Win32 implementation for now
		virtual void toggle_fullscreen() = 0;

		/// \brief Displays the window in its current size and position.
		virtual void show(bool activate = true) = 0;

		/// \brief Hides the window.
		virtual void hide() = 0;

		/// \brief Raises the window on top of other windows.
		virtual void bring_to_front() = 0;

		/// \brief Flip back buffer to front, making changes visible on screen.
		///
		/// The parameter interval specifies the minimum number of video frames
		/// that are displayed before a buffer swap will occur.
		///
		/// If interval is set to a value of 0, buffer swaps are not synchronized
		/// to a video frame.
		///
		/// If interval is set to a value of -1 (the default), then it will use
		/// the buffer swap used for previous flip. If its the first flip, it will
		/// use the system default.
		///
		/// \param interval = See note
		virtual void flip(int interval = -1) = 0;

		/// \brief Shows the mouse cursor.
		virtual void show_cursor() = 0;

		/// \brief Sets the current cursor icon.
		virtual void set_cursor(const CursorPtr &cursor) = 0;

		/// \brief Set cursor
		///
		/// \param type = Standard Cursor
		virtual void set_cursor(StandardCursor type) = 0;

#ifdef WIN32
		/// \brief Sets the current cursor handle (win32 only)
		virtual void set_cursor_handle(HCURSOR cursor) = 0;
#endif

		/// \brief Hides the mouse cursor.
		virtual void hide_cursor() = 0;

		/// \brief Stores text in the clipboard.
		virtual void set_clipboard_text(const std::string &text) = 0;

		/// \brief Stores an image in the clipboard.
		virtual void set_clipboard_image(const PixelBufferPtr &buf) = 0;

		/// \brief Sets the large icon used for this window.
		virtual void set_large_icon(const PixelBufferPtr &image) = 0;

		/// \brief Sets the small icon used for this window.
		virtual void set_small_icon(const PixelBufferPtr &image) = 0;

		/// \brief Enable alpha channel for this window.
		///
		/// This is only supported on Windows Vista and above (Else use Layered windows instead)
		/// \param blur_rect = Blur rectangle. If size = 0, then the entire window is used
		virtual void enable_alpha_channel(const Rectf &blur_rect) = 0;

		/// \brief Exend the window frame into the client area
		///
		/// Only implemented on win32
		virtual void extend_frame_into_client_area(float left, float top, float right, float bottom) = 0;
	};

	typedef std::shared_ptr<DisplayWindow> DisplayWindowPtr;
}
