
#include "precomp.h"
#include "main_window_controller.h"

using namespace uicore;

MainWindowController::MainWindowController()
{
	set_title("UICore SVG Viewer Example");
	set_frame_size({ 1200.0f, 750.0f });
	set_root_view(view);
}
