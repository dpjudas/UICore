
#pragma once

class SvgNode;

class Svg
{
public:
	Svg(const std::string &filename);
	void render(const uicore::CanvasPtr &canvas, uicore::Rectf viewbox);

	static const std::string svg_ns;   // http://www.w3.org/2000/svg
	static const std::string xlink_ns; // http://www.w3.org/1999/xlink

private:
	uicore::XmlDocumentPtr xml;
	std::shared_ptr<SvgNode> root_node;
};

