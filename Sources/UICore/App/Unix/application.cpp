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

#include <cstdlib>
#include <iostream>
#include <vector>
#include "UICore/Application/application.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/System/exception.h"
#include "UICore/Core/ErrorReporting/exception_dialog.h"
#include "UICore/display.h"

namespace uicore
{
	static ApplicationInstancePrivate *app_instance = 0;
	static bool enable_catch_exceptions = false;

	static std::vector<std::string> command_line_args;

	ApplicationInstancePrivate::ApplicationInstancePrivate(bool catch_exceptions)
	{
		app_instance = this;
		enable_catch_exceptions = catch_exceptions;
	}

	const std::vector<std::string> &Application::main_args()
	{
		return command_line_args;
	}
}

int main(int argc, char **argv)
{
	if (uicore::app_instance == nullptr)
	{
		std::cout << "UICore: No global Application instance!" << std::endl;
		return 255;
	}

	std::vector<std::string> args;
	for (int i = 0; i < argc; i++)
		args.push_back(argv[i]);
	uicore::command_line_args = args;

#ifdef DEBUG
	clan::ConsoleLogger console_logger;
	console_logger.enable();
#endif

	int retval = 0;

	if (uicore::enable_catch_exceptions)
	{
		try
		{
			std::unique_ptr<uicore::Application> app = uicore::app_instance->create();
			uicore::RunLoop::run();
		}
		catch (uicore::Exception &exception)
		{
			std::string console_name("Console");
			if (!args.empty())
				console_name = args[0];

			std::cout << "Exception caught: " + exception.get_message_and_stack_trace() << std::endl;
			retval = -1;
		}
	}
	else
	{
		std::unique_ptr<uicore::Application> app = uicore::app_instance->create();
		uicore::RunLoop::run();
	}

	return retval;
}
