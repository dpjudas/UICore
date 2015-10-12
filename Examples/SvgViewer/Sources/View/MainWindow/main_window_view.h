
#pragma once

#include "View/Svg/svg_view.h"

class MainWindowView : public uicore::ColumnView
{
public:
	MainWindowView()
	{
		style()->set("background: rgb(240,240,240)");
		style()->set("padding: 11px");
		style()->set("font: 11px/15px 'Segoe UI'; color: black");

		svg = add_subview<SvgView>();
	}

	std::shared_ptr<SvgView> svg;
};
