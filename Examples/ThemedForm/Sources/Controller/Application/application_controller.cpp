
#include "precomp.h"
#include "application_controller.h"
#include "Controller/MainWindow/main_window_controller.h"

using namespace uicore;

ApplicationController::ApplicationController()
{
	// Force this example to always pick the OpenGL target
	OpenGLTarget::set_current();

	WindowManager::set_exit_on_last_close();
	WindowManager::present_main<MainWindowController>();
}

// Register ApplicationController as the application main class:
ApplicationInstance<ApplicationController> app;
