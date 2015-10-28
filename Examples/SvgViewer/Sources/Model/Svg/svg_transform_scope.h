
#pragma once

// Applies to: 'a', 'circle', 'clipPath', 'defs', 'ellipse', 'foreignObject', 'g', 'image', 'line', 'path', 'polygon', 'polyline', 'rect', 'switch', 'text', 'use'
class SvgTransformScope
{
public:
	SvgTransformScope(const uicore::CanvasPtr &canvas, const uicore::XmlNodePtr &e);
	SvgTransformScope(const uicore::CanvasPtr &canvas, const uicore::Mat4f &transform, bool transform_active);
	~SvgTransformScope();

	static uicore::Mat4f parse_transform(const uicore::XmlNodePtr &e, bool &transform_active);

private:
	uicore::CanvasPtr canvas;
	uicore::Mat4f old_transform;
	bool transform_active = false;
};
