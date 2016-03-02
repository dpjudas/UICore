
#pragma once

class ThemeButtonView : public uicore::ButtonView
{
public:
	ThemeButtonView()
	{
		style()->set(R"(
			border-image-slice: 6 6 5 5 fill;
			border-image-width:6px 6px 5px 5px;
			border-image-repeat:stretch;
			border-image-source:url('button_normal.png');
			)");

		style("hot")->set("border-image-source:url('button_hot.png');");
		style("pressed")->set("border-image-source:url('button_pressed.png');");
		style("disabled")->set("border-image-source:url('button_disabled.png');");

		label()->style()->set(R"(
			margin: 5px auto;
			font: 13px/18px 'Segoe UI';
			padding: 0 10px;
			color: rgb(0,0,0);
			)");

		label()->style("disabled")->set("color: rgb(128,128,128);");
		label()->set_text_alignment(uicore::TextAlignment::center);
	}
};
