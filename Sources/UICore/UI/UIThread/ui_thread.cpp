/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
*/

#include "UICore/precomp.h"
#include "UICore/Display/2D/canvas.h"
#include "UICore/Display/2D/image.h"
#include "UICore/Display/Font/font.h"
#include "UICore/Display/System/run_loop.h"
#include "UICore/UI/UIThread/ui_thread.h"
#include "UICore/Core/ErrorReporting/exception_dialog.h"
#include "UICore/Core/IOData/path_help.h"

namespace clan
{
	static UIThreadImpl *ui_thread_instance = nullptr;

	class UIThreadImpl
	{
	public:
		UIThreadImpl()
		{
			if (ui_thread_instance != nullptr) throw Exception("Only one UIThread is allowed");
			ui_thread_instance = this;
		}

		~UIThreadImpl()
		{
			ui_thread_instance = nullptr;
		}

		std::string resource_path;
		std::function<void(const std::exception_ptr &)> exception_handler;

		static UIThreadImpl *get_instance()
		{
			if (ui_thread_instance == nullptr) throw Exception("No UIThread created");
			return ui_thread_instance;
		}
	};

	UIThread::UIThread()
	{
	}

	UIThread::UIThread(const std::string &resource_path, const std::function<void(const std::exception_ptr &)> &exception_handler) : impl(std::make_shared<UIThreadImpl>())
	{
		impl->resource_path = resource_path;
		impl->exception_handler = exception_handler;

		if (!exception_handler)
		{
			impl->exception_handler = ExceptionDialog::show;
		}
	}

	UIThread::~UIThread()
	{
	}

	Image UIThread::get_image(Canvas &canvas, const std::string &name)
	{
		return Image(canvas, PathHelp::combine(UIThreadImpl::get_instance()->resource_path, name));
	}

	Font UIThread::get_font(const std::string &family, const FontDescription &desc)
	{
		return Font(family, desc);
	}

	bool UIThread::try_catch(const std::function<void()> &block)
	{
		try
		{
			block();
			return true;
		}
		catch (...)
		{
			std::exception_ptr exception = std::current_exception();
			RunLoop::main_thread_async([=]()
			{
				UIThreadImpl::get_instance()->exception_handler(exception);
			});
			return false;
		}
	}
}
