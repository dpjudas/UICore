
#pragma once

#include "View/MainWindow/main_window_view.h"

class MainWindowController : public uicore::WindowController
{
public:
	MainWindowController();

	std::shared_ptr<MainWindowView> view = std::make_shared<MainWindowView>();
};
