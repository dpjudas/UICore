
#pragma once

class ListBoxView : public uicore::ListBoxBaseView
{
public:
	ListBoxView()
	{
		style()->set(R"(
			margin: 7px 0;
			border: 1px solid black;
			padding: 5px;
			background: #f0f0f0;
			)");
	}
};

class ListBoxLabelView : public uicore::LabelBaseView
{
public:
	ListBoxLabelView(const std::string &text = std::string())
	{
		style()->set(R"(
			font: 13px/17px 'Segoe UI';
			color: black;
			margin: 1px 0;
			padding: 0 2px
			)");

		style("selected")->set("background: #7777f0; color: white;");
		style("hot")->set("background: #ccccf0; color: black");

		set_text(text);
	}
};
