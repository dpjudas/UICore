
#pragma once

class HeadlineView : public uicore::TextBlockBaseView
{
public:
	HeadlineView()
	{
		style()->set(R"(
			font-weight: bold;
			font-size: 24px;
			line-height: 30px;
			margin: 15px 0;
			)");
	}
};

class ParagraphView : public uicore::TextBlockBaseView
{
public:
	ParagraphView()
	{
		style()->set(R"(
			margin: 10px 0;
			)");
	}
};
