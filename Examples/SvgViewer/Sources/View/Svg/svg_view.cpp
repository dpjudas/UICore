
#include "precomp.h"
#include "svg_view.h"
#include "Model/app_model.h"
#include "Model/Svg/svg.h"

using namespace uicore;

SvgView::SvgView()
{
	style()->set("flex: auto");
}

void SvgView::render_content(const CanvasPtr &canvas)
{
	if (!svg)
		svg = std::make_shared<Svg>(AppModel::instance()->svg_filename);

	svg->render(canvas, geometry().content_size());
}
