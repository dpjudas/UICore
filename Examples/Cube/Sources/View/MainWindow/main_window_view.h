
#pragma once

#include "View/Cube/cube_view.h"

class MainWindowView : public uicore::ColumnView
{
public:
	MainWindowView()
	{
		style()->set("background: rgb(100,100,100)");
		style()->set("padding: 11px");
		style()->set("font: 11px/15px 'Segoe UI'; color: black");

		cube = add_subview<CubeView>();
	}

	std::shared_ptr<CubeView> cube;
};
