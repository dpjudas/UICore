
#pragma once

class ThemeFormView : public uicore::ColumnView
{
public:
	ThemeFormView()
	{
		style()->set(R"(
			background: rgb(250,250,250);
			font: 13px/20px 'Segoe UI'; color: black
			padding: 11px;
			)");
	}
};
