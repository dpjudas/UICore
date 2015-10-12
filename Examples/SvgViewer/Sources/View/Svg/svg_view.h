
#pragma once

class SvgView : public uicore::View
{
public:
	SvgView();

protected:
	void render_content(const uicore::CanvasPtr &canvas) override;
};
