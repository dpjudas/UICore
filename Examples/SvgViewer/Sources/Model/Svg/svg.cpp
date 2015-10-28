
#include "precomp.h"
#include "svg.h"
#include "svg_tree.h"
#include "svg_attribute_reader.h"

using namespace uicore;

Svg::Svg(const std::string &filename)
{
	auto device = File::open_existing(filename);
	xml = XmlDocument::load(device, false);
}

void Svg::render(const CanvasPtr &canvas, Rectf render_viewbox)
{
	if (!root_node)
	{
		SvgTreeBuilder builder(canvas);
		auto element = xml->document_element();
		builder.build(element);
		root_node = builder.node;
	}

	SvgAttributeReader attr_viewbox(xml->document_element(), "viewBox");
	float x = (float)attr_viewbox.get_number();
	float y = (float)attr_viewbox.get_number();
	float w = (float)attr_viewbox.get_number();
	float h = (float)attr_viewbox.get_number();
	Rectf svg_viewport = Rectf::xywh(x, y, w, h);

	float aspect = svg_viewport.width() / svg_viewport.height();
	auto center = render_viewbox.center();
	render_viewbox.set_width(render_viewbox.height() * aspect);
	render_viewbox.set_position(center - Vec2f(render_viewbox.size() * 0.5f));

	auto offset_svg = Mat4f::translate(-svg_viewport.x(), -svg_viewport.y(), 0.0f);
	auto scale = Mat4f::scale(render_viewbox.width() / svg_viewport.width(), render_viewbox.height() / svg_viewport.height(), 1.0f);
	auto offset_render = Mat4f::translate(render_viewbox.x(), render_viewbox.y(), 0.0f);

	root_node->transform = offset_render * scale * offset_svg;
	root_node->transform_active = true;
	root_node->render(canvas);
}

const std::string Svg::svg_ns = "http://www.w3.org/2000/svg";
const std::string Svg::xlink_ns = "http://www.w3.org/1999/xlink";
