
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
			)");
	}
};

class ThemeParagraphView : public uicore::TextBlockView
{
public:
	ThemeParagraphView()
	{
	}
};
