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
 **    James Lammlein
 **    Magnus Norddahl
 */

#pragma once

#include "UICore/Core/Math/rect.h"
#include "UICore/Display/Window/display_window_description.h"
#include "UICore/Display/display_target.h"
#include "UICore/Display/Window/display_window_provider.h"
#include "UICore/Display/Window/display_window.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Display/Window/input_device.h"
#include "UICore/GL/opengl.h"
#include "UICore/GL/opengl_context_description.h"
#include "UICore/GL/opengl_context_description_impl.h"
#include "UICore/GL/GL3/gl3_graphic_context.h"

#import <CoreFoundation/CoreFoundation.h>
#import <AppKit/AppKit.h>

@class CocoaWindow;

namespace uicore
{

	class OpenGLWindowProvider_Impl
	{
	public:
		OpenGLWindowProvider_Impl(OpenGLWindowProvider *self, OpenGLContextDescription &opengl_desc);

		NSOpenGLContext *get_share_context();
		void on_input_event(NSEvent *theEvent);

		OpenGLWindowProvider *self;
		GraphicContextPtr gc;
		DisplayWindowProvider *site;
		OpenGLContextDescription opengl_desc;

		CocoaWindow *window;
		NSOpenGLContext *opengl_context;

	private:
		void on_keyboard_event(NSEvent *theEvent);
		void on_mouse_event(NSEvent *theEvent);
	};

	inline NSRect to_cocoa_rect(const Rectf &rect, NSRect parent_frame)
	{
		return NSMakeRect(rect.left, parent_frame.size.height - rect.bottom, rect.width(), rect.height());
	}
	
	inline Rectf from_cocoa_rect(NSRect rect, NSRect parent_frame)
	{
		return Rectf::xywh(rect.origin.x, parent_frame.size.height - rect.origin.y - rect.size.height, rect.size.width, rect.size.height);
	}
	
}
