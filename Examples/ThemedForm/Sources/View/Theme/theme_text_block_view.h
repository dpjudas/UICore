
#pragma once

class ThemeHeadlineView : public uicore::TextBlockView
{
public:
	ThemeHeadlineView()
	{
		style()->set(R"(
			font-weight: bold;
			font-size: 24px;
			line-height: 30px;
			margin: 15px 0;
			)");
	}
};

class ThemeParagraphView : public uicore::TextBlockView
{
public:
	ThemeParagraphView()
	{
		style()->set(R"(
			margin: 10px 0;
			)");
	}
};
