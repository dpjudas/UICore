
#pragma once

#include "svg_element_visitor.h"

class SvgNode;

class SvgTreeBuilder : private SvgElementVisitor
{
public:
	SvgTreeBuilder(const uicore::CanvasPtr &canvas);
	void build(const uicore::XmlNodePtr &svg_element);

	std::shared_ptr<SvgNode> node;

protected:
	void g(const uicore::XmlNodePtr &e) override;
	void line(const uicore::XmlNodePtr &e) override;
	void polyline(const uicore::XmlNodePtr &e) override;
	void rect(const uicore::XmlNodePtr &e) override;
	void circle(const uicore::XmlNodePtr &e) override;
	void ellipse(const uicore::XmlNodePtr &e) override;
	void polygon(const uicore::XmlNodePtr &e) override;
	void path(const uicore::XmlNodePtr &e) override;
	void text(const uicore::XmlNodePtr &e) override;
	void image(const uicore::XmlNodePtr &e) override;

private:
	void render_path(const uicore::PathPtr &path, const uicore::XmlNodePtr &e);

	uicore::Mat4f get_transform(const uicore::XmlNodePtr &e);

	uicore::CanvasPtr canvas;
};

class SvgNode
{
public:
	void render(const uicore::CanvasPtr &canvas);

	uicore::Mat4f transform = uicore::Mat4f::identity();
	bool transform_active = false;

	std::vector<std::shared_ptr<SvgNode>> nodes;

	uicore::PathPtr path;
	uicore::Pen pen;
	uicore::Brush brush;
	bool fill = false;
	bool stroke = false;
};
