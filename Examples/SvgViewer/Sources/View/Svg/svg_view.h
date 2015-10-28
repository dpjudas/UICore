
#pragma once

class Svg;

class SvgView : public uicore::View
{
public:
	SvgView();

protected:
	void render_content(const uicore::CanvasPtr &canvas) override;

private:
	std::shared_ptr<Svg> svg;
};
