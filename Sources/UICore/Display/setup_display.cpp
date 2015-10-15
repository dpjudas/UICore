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

#include "UICore/precomp.h"
#include "setup_display.h"
#include "UICore/Display/ImageFormats/image_file_type_register.h"
#include "UICore/Display/ImageFormats/targa_format.h"
#include "UICore/Display/ImageFormats/jpeg_format.h"
#include "UICore/Display/ImageFormats/png_format.h"
#include "../Core/System/setup_core.h"

#ifdef WIN32
#include "Platform/Win32/display_message_queue_win32.h"
#elif defined(__APPLE__)
#include "Platform/Cocoa/display_message_queue_cocoa.h"
#elif !defined(__ANDROID__)
#include "Platform/X11/display_message_queue_x11.h"
#endif

#if !defined __ANDROID__ && ! defined __APPLE__ && ! defined WIN32
#include <X11/Xlib.h>
#endif

namespace uicore
{
	class ImageFileType;
	class SetupDisplay_Impl : public SetupModule
	{
	public:
		SetupDisplay_Impl();
		virtual ~SetupDisplay_Impl();

		static SetupDisplay_Impl *instance;

		/// \brief Map of the class factories for each provider type.
		std::map<std::string, ImageFileType *> image_provider_factory_types;

		ImageFileType_Register<JPEGFormat> *jpeg_provider = nullptr;
		ImageFileType_Register<JPEGFormat> *jpg_provider = nullptr;
		ImageFileType_Register<PNGFormat> *png_provider = nullptr;
		ImageFileType_Register<TargaFormat> *targa_provider = nullptr;
		ImageFileType_Register<TargaFormat> *tga_provider = nullptr;

#if defined(WIN32)
		DisplayMessageQueue_Win32 message_queue;
#elif defined(__APPLE__)
		DisplayMessageQueue_Cocoa message_queue;
#elif !defined(__ANDROID__)
		DisplayMessageQueue_X11 message_queue;
#endif
	};
	SetupDisplay_Impl *SetupDisplay_Impl::instance = nullptr;

	void SetupDisplay::start()
	{
		std::lock_guard<std::recursive_mutex> lock(SetupCore::instance.mutex);

		if (SetupCore::instance.module_display)
			return;

		SetupCore::start();	// Display depends on core.
		SetupCore::instance.module_display = uicore::make_unique<SetupDisplay_Impl>();
	}

	SetupDisplay_Impl::SetupDisplay_Impl()
	{
		instance = this;
#ifdef WIN32
		SetProcessDPIAware();
#endif

#if !defined __ANDROID__ && ! defined __APPLE__ && ! defined WIN32
		// The XInitThreads() function initializes Xlib support for concurrent threads.
		// This function must be the first Xlib function a multi-threaded program calls, and it must complete before any other Xlib call is made.
		XInitThreads();
#endif
		jpeg_provider = new ImageFileType_Register<JPEGFormat>("jpeg");
		jpg_provider = new ImageFileType_Register<JPEGFormat>("jpg");
		png_provider = new ImageFileType_Register<PNGFormat>("png");
		targa_provider = new ImageFileType_Register<TargaFormat>("targa");
		tga_provider = new ImageFileType_Register<TargaFormat>("tga");
	}

	SetupDisplay_Impl::~SetupDisplay_Impl()
	{
		delete jpeg_provider;
		delete jpg_provider;
		delete png_provider;
		delete targa_provider;
		delete tga_provider;

		instance = nullptr;
	}

#ifdef WIN32
	DisplayMessageQueue_Win32 *SetupDisplay::get_message_queue()
	{
		if (!SetupDisplay_Impl::instance)
			start();
		return &SetupDisplay_Impl::instance->message_queue;
	}
#elif !defined(__APPLE__) && !defined(__ANDROID__)
	DisplayMessageQueue_X11* SetupDisplay::get_message_queue()
	{
		if (!SetupDisplay_Impl::instance)
			start();
		return &SetupDisplay_Impl::instance->message_queue;
	}
#endif
	std::map<std::string, ImageFileType *> *SetupDisplay::get_image_provider_factory_types()
	{
		if (!SetupDisplay_Impl::instance)
			start();
		return &SetupDisplay_Impl::instance->image_provider_factory_types;
	}
}
