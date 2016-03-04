
#pragma once

class CheckBoxView : public uicore::CheckBoxBaseView
{
public:
	CheckBoxView()
	{
		style()->set(R"(
			background-position:center center;
			background-repeat:no-repeat;
			background-attachment:scroll;
			background-image:url('checkbox_unchecked_normal.png');
			width:13px;
			height:13px;
			)");

		style("unchecked_hot")->set("background-image:url('checkbox_unchecked_hot.png');");
		style("unchecked_pressed")->set("background-image:url('checkbox_unchecked_pressed.png');");
		style("unchecked_disabled")->set("background-image:url('checkbox_unchecked_disabled.png');");
		style("checked")->set("background-image:url('checkbox_checked_normal.png');");
		style("checked_hot")->set("background-image:url('checkbox_checked_hot.png');");
		style("checked_pressed")->set("background-image:url('checkbox_checked_pressed.png');");
		style("checked_disabled")->set("background-image:url('checkbox_checked_disabled.png');");
	}
};
