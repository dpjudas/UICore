
#include "precomp.h"
#include "main_window_controller.h"

using namespace uicore;

MainWindowController::MainWindowController()
{
	set_title("UICore Flexbox Cheat Sheet Example");
	set_frame_size({ 900.0f, 600.0f });
	set_root_view(view);
}
