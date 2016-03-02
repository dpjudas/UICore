
#pragma once

class ThemeScrollView : public uicore::ScrollView
{
public:
	ThemeScrollView()
	{
		scrollbar_x_view()->style()->set("flex: 0 0 auto");
		scrollbar_x_view()->style()->set("background: rgb(232, 232, 236)");
		scrollbar_x_view()->track()->style()->set("border-image-slice: 4 0 3 0 fill;");
		scrollbar_x_view()->track()->style()->set("border-image-width:4px 0px 3px 0px;");
		scrollbar_x_view()->track()->style()->set("border-image-repeat:stretch;");
		scrollbar_x_view()->track()->style()->set("border-image-source:url('scrollbar_hori_track_normal.png');");
		scrollbar_x_view()->track()->style("hot")->set("border-image-source:url('scrollbar_hori_track_hot.png');");
		scrollbar_x_view()->track()->style("pressed")->set("border-image-source:url('scrollbar_hori_track_pressed.png');");
		scrollbar_x_view()->track()->style("disabled")->set("border-image-source:url('scrollbar_hori_track_disabled.png');");

		scrollbar_x_view()->thumb()->style()->set("border-image-slice: 5 5 5 5 fill;");
		scrollbar_x_view()->thumb()->style()->set("border-image-width:5px 5px 5px 5px;");
		scrollbar_x_view()->thumb()->style()->set("border-image-repeat:stretch;");
		scrollbar_x_view()->thumb()->style()->set("border-image-source:url('scrollbar_hori_thumb_normal.png');");
		scrollbar_x_view()->thumb()->style("hot")->set("border-image-source:url('scrollbar_hori_thumb_hot.png');");
		scrollbar_x_view()->thumb()->style("pressed")->set("border-image-source:url('scrollbar_hori_thumb_pressed.png');");
		scrollbar_x_view()->thumb()->style("disabled")->set("border-image-source:url('scrollbar_hori_thumb_disabled.png');");

		scrollbar_x_view()->thumb_grip()->style()->set("background-position:center center;");
		scrollbar_x_view()->thumb_grip()->style()->set("background-repeat:no-repeat;");
		scrollbar_x_view()->thumb_grip()->style()->set("background-attachment:scroll; ");
		scrollbar_x_view()->thumb_grip()->style()->set("background-image:url('scrollbar_hori_thumb_gripper_normal.png');");
		scrollbar_x_view()->thumb_grip()->style("hot")->set("background-image:url('scrollbar_hori_thumb_gripper_hot.png');");
		scrollbar_x_view()->thumb_grip()->style("pressed")->set("background-image:url('scrollbar_hori_thumb_gripper_pressed.png');");
		scrollbar_x_view()->thumb_grip()->style("disabled")->set("background-image:url('scrollbar_hori_thumb_gripper_disabled.png');");

		scrollbar_x_view()->button_decrement()->style()->set("width:17px; height:17px");
		scrollbar_x_view()->button_decrement()->style()->set("border-image-slice: 3 3 3 3 fill;");
		scrollbar_x_view()->button_decrement()->style()->set("border-image-width:3px 3px 3px 3px;");
		scrollbar_x_view()->button_decrement()->style()->set("border-image-repeat:stretch;");
		scrollbar_x_view()->button_decrement()->style()->set("border-image-source:url('scrollbar_hori_button_left_normal_withglyph.png');");
		scrollbar_x_view()->button_decrement()->style("hot")->set("border-image-source:url('scrollbar_hori_button_left_hot_withglyph.png');");
		scrollbar_x_view()->button_decrement()->style("pressed")->set("border-image-source:url('scrollbar_hori_button_left_pressed_withglyph.png');");
		scrollbar_x_view()->button_decrement()->style("disabled")->set("border-image-source:url('scrollbar_hori_button_left_disabled_withglyph.png');");

		scrollbar_x_view()->button_increment()->style()->set("width:17px; height:17px");
		scrollbar_x_view()->button_increment()->style()->set("border-image-slice: 3 3 3 3 fill;");
		scrollbar_x_view()->button_increment()->style()->set("border-image-width:3px 3px 3px 3px;");
		scrollbar_x_view()->button_increment()->style()->set("border-image-repeat:stretch;");
		scrollbar_x_view()->button_increment()->style()->set("border-image-source:url('scrollbar_hori_button_right_normal_withglyph.png');");
		scrollbar_x_view()->button_increment()->style("hot")->set("border-image-source:url('scrollbar_hori_button_right_hot_withglyph.png');");
		scrollbar_x_view()->button_increment()->style("pressed")->set("border-image-source:url('scrollbar_hori_button_right_pressed_withglyph.png');");
		scrollbar_x_view()->button_increment()->style("disabled")->set("border-image-source:url('scrollbar_hori_button_right_disabled_withglyph.png');");

		scrollbar_y_view()->style()->set("flex: 0 0 auto");
		scrollbar_y_view()->style()->set("background: rgb(232, 232, 236)");
		scrollbar_y_view()->track()->style()->set("border-image-slice: 4 0 3 0 fill;");
		scrollbar_y_view()->track()->style()->set("border-image-width:4px 0px 3px 0px;");
		scrollbar_y_view()->track()->style()->set("border-image-repeat:stretch;");
		scrollbar_y_view()->track()->style()->set("border-image-source:url('scrollbar_vert_track_normal.png');");
		scrollbar_y_view()->track()->style("hot")->set("border-image-source:url('scrollbar_vert_track_hot.png');");
		scrollbar_y_view()->track()->style("pressed")->set("border-image-source:url('scrollbar_vert_track_pressed.png');");
		scrollbar_y_view()->track()->style("disabled")->set("border-image-source:url('scrollbar_vert_track_disabled.png');");

		scrollbar_y_view()->thumb()->style()->set("border-image-slice: 5 5 5 5 fill;");
		scrollbar_y_view()->thumb()->style()->set("border-image-width:5px 5px 5px 5px;");
		scrollbar_y_view()->thumb()->style()->set("border-image-repeat:stretch;");
		scrollbar_y_view()->thumb()->style()->set("border-image-source:url('scrollbar_vert_thumb_normal.png');");
		scrollbar_y_view()->thumb()->style("hot")->set("border-image-source:url('scrollbar_vert_thumb_hot.png');");
		scrollbar_y_view()->thumb()->style("pressed")->set("border-image-source:url('scrollbar_vert_thumb_pressed.png');");
		scrollbar_y_view()->thumb()->style("disabled")->set("border-image-source:url('scrollbar_vert_thumb_disabled.png');");

		scrollbar_y_view()->thumb_grip()->style()->set("background-position:center center;");
		scrollbar_y_view()->thumb_grip()->style()->set("background-repeat:no-repeat;");
		scrollbar_y_view()->thumb_grip()->style()->set("background-attachment:scroll; ");
		scrollbar_y_view()->thumb_grip()->style()->set("background-image:url('scrollbar_vert_thumb_gripper_normal.png');");
		scrollbar_y_view()->thumb_grip()->style("hot")->set("background-image:url('scrollbar_vert_thumb_gripper_hot.png');");
		scrollbar_y_view()->thumb_grip()->style("pressed")->set("background-image:url('scrollbar_vert_thumb_gripper_pressed.png');");
		scrollbar_y_view()->thumb_grip()->style("disabled")->set("background-image:url('scrollbar_vert_thumb_gripper_disabled.png');");

		scrollbar_y_view()->button_decrement()->style()->set("width:17px; height:17px");
		scrollbar_y_view()->button_decrement()->style()->set("border-image-slice: 3 3 3 3 fill;");
		scrollbar_y_view()->button_decrement()->style()->set("border-image-width:3px 3px 3px 3px;");
		scrollbar_y_view()->button_decrement()->style()->set("border-image-repeat:stretch;");
		scrollbar_y_view()->button_decrement()->style()->set("border-image-source:url('scrollbar_vert_button_left_normal_withglyph.png');");
		scrollbar_y_view()->button_decrement()->style("hot")->set("border-image-source:url('scrollbar_vert_button_left_hot_withglyph.png');");
		scrollbar_y_view()->button_decrement()->style("pressed")->set("border-image-source:url('scrollbar_vert_button_left_pressed_withglyph.png');");
		scrollbar_y_view()->button_decrement()->style("disabled")->set("border-image-source:url('scrollbar_vert_button_left_disabled_withglyph.png');");

		scrollbar_y_view()->button_increment()->style()->set("width:17px; height:17px");
		scrollbar_y_view()->button_increment()->style()->set("border-image-slice: 3 3 3 3 fill;");
		scrollbar_y_view()->button_increment()->style()->set("border-image-width:3px 3px 3px 3px;");
		scrollbar_y_view()->button_increment()->style()->set("border-image-repeat:stretch;");
		scrollbar_y_view()->button_increment()->style()->set("border-image-source:url('scrollbar_vert_button_right_normal_withglyph.png');");
		scrollbar_y_view()->button_increment()->style("hot")->set("border-image-source:url('scrollbar_vert_button_right_hot_withglyph.png');");
		scrollbar_y_view()->button_increment()->style("pressed")->set("border-image-source:url('scrollbar_vert_button_right_pressed_withglyph.png');");
		scrollbar_y_view()->button_increment()->style("disabled")->set("border-image-source:url('scrollbar_vert_button_right_disabled_withglyph.png');");
	}
};

class ThemeVScrollBarView : public uicore::ScrollBarView
{
public:
	ThemeVScrollBarView()
	{
	}
};

class ThemeHScrollBarView : public uicore::ScrollBarView
{
public:
	ThemeHScrollBarView()
	{
		set_horizontal();
		style()->set("flex: 0 0 auto");
		style()->set("background: rgb(232, 232, 236)");
		track()->style()->set("border-image-slice: 4 0 3 0 fill;");
		track()->style()->set("border-image-width:4px 0px 3px 0px;");
		track()->style()->set("border-image-repeat:stretch;");
		track()->style()->set("border-image-source:url('scrollbar_hori_track_normal.png');");
		track()->style("hot")->set("border-image-source:url('scrollbar_hori_track_hot.png');");
		track()->style("pressed")->set("border-image-source:url('scrollbar_hori_track_pressed.png');");
		track()->style("disabled")->set("border-image-source:url('scrollbar_hori_track_disabled.png');");

		thumb()->style()->set("border-image-slice: 5 5 5 5 fill;");
		thumb()->style()->set("border-image-width:5px 5px 5px 5px;");
		thumb()->style()->set("border-image-repeat:stretch;");
		thumb()->style()->set("border-image-source:url('scrollbar_hori_thumb_normal.png');");
		thumb()->style("hot")->set("border-image-source:url('scrollbar_hori_thumb_hot.png');");
		thumb()->style("pressed")->set("border-image-source:url('scrollbar_hori_thumb_pressed.png');");
		thumb()->style("disabled")->set("border-image-source:url('scrollbar_hori_thumb_disabled.png');");

		thumb_grip()->style()->set("background-position:center center;");
		thumb_grip()->style()->set("background-repeat:no-repeat;");
		thumb_grip()->style()->set("background-attachment:scroll; ");
		thumb_grip()->style()->set("background-image:url('scrollbar_hori_thumb_gripper_normal.png');");
		thumb_grip()->style("hot")->set("background-image:url('scrollbar_hori_thumb_gripper_hot.png');");
		thumb_grip()->style("pressed")->set("background-image:url('scrollbar_hori_thumb_gripper_pressed.png');");
		thumb_grip()->style("disabled")->set("background-image:url('scrollbar_hori_thumb_gripper_disabled.png');");

		button_decrement()->style()->set("width:17px; height:17px");
		button_decrement()->style()->set("border-image-slice: 3 3 3 3 fill;");
		button_decrement()->style()->set("border-image-width:3px 3px 3px 3px;");
		button_decrement()->style()->set("border-image-repeat:stretch;");
		button_decrement()->style()->set("border-image-source:url('scrollbar_hori_button_left_normal_withglyph.png');");
		button_decrement()->style("hot")->set("border-image-source:url('scrollbar_hori_button_left_hot_withglyph.png');");
		button_decrement()->style("pressed")->set("border-image-source:url('scrollbar_hori_button_left_pressed_withglyph.png');");
		button_decrement()->style("disabled")->set("border-image-source:url('scrollbar_hori_button_left_disabled_withglyph.png');");

		button_increment()->style()->set("width:17px; height:17px");
		button_increment()->style()->set("border-image-slice: 3 3 3 3 fill;");
		button_increment()->style()->set("border-image-width:3px 3px 3px 3px;");
		button_increment()->style()->set("border-image-repeat:stretch;");
		button_increment()->style()->set("border-image-source:url('scrollbar_hori_button_right_normal_withglyph.png');");
		button_increment()->style("hot")->set("border-image-source:url('scrollbar_hori_button_right_hot_withglyph.png');");
		button_increment()->style("pressed")->set("border-image-source:url('scrollbar_hori_button_right_pressed_withglyph.png');");
		button_increment()->style("disabled")->set("border-image-source:url('scrollbar_hori_button_right_disabled_withglyph.png');");
	}
};
