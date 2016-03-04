
#pragma once

class TextFieldView : public uicore::TextFieldBaseView
{
public:
	TextFieldView()
	{
		style()->set(R"(
			border: 1px solid #dadada;
			padding: 3px 9px 4px 10px;
			background: white;
			)");
	}
};
