
#include "precomp.h"
#include "svg_view.h"

using namespace uicore;

SvgView::SvgView()
{
	style()->set("flex: auto");
}

void SvgView::render_content(const CanvasPtr &canvas)
{
	float cx = geometry().content_width * 0.5f;
	float cy = geometry().content_height * 0.5f;

	Brush brush;
	brush.type = BrushType::radial;
	brush.center_point = { cx * 0.66f, cy * 0.66f };
	brush.radius_x = cx * 2.0f;
	brush.radius_y = cy * 2.0f;
	brush.stops = { { Colorf::aquamarine, 0.0f }, { Colorf::forestgreen, 0.5f }, { Colorf::darkkhaki, 1.0f } };

	Path::ellipse(cx, cy, cx, cy)->fill(canvas, brush);
}
