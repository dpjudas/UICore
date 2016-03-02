
#pragma once

class ThemeSliderView : public uicore::SliderView
{
public:
	ThemeSliderView()
	{
		set_horizontal();

		style()->set("flex-direction: row;");

		track()->style()->set(R"(
			flex: 1 1 auto;
			height: 4px;
			margin: 7px 0px;
			border-image-slice: 1 2 1 1 fill;
			border-image-width: 1px 2px 1px 1px;
			border-image-repeat: stretch;
			border-image-source: url('slider_track.png');
			)");

		thumb()->style()->set(R"(
			position: absolute;
			width: 11px;
			height: 19px;
			border-image-slice: 9 3 9 2 fill;
			border-image-width: 9px 3px 9px 2px;
			border-image-repeat: stretch;
			border-image-source: url('slider_horizontal_thumb_normal.png');
			)");

		thumb()->style("hot")->set("border-image-source:url('slider_horizontal_thumb_hot.png');");
		thumb()->style("pressed")->set("border-image-source:url('slider_horizontal_thumb_pressed.png');");
		thumb()->style("disabled")->set("border-image-source:url('slider_horizontal_thumb_disabled.png');");
	}
};
