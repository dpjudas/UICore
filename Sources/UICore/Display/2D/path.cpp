/*
**  UICore
**  Copyright (c) 1997-2015 The UICore Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
*/

#include "UICore/precomp.h"
#include "UICore/Display/2D/path.h"
#include "path_impl.h"
#include "canvas_impl.h"
#include "render_batch_path.h"
#include "../Font/font_impl.h"

namespace uicore
{
	std::shared_ptr<Path> Path::create()
	{
		return std::make_shared<PathImpl>();
	}

	std::shared_ptr<Path> Path::glyph(const CanvasPtr &canvas, Font &font, unsigned int glyph, GlyphMetrics &out_metrics)
	{
		auto path = Path::create();
		if (font.impl)
		{
			font.impl->get_glyph_path(canvas, glyph, path, out_metrics);
		}
		return path;
	}

	PathImpl::PathImpl()
	{
		_subpaths.resize(1);
	}

	void PathImpl::move_to(const Pointf &point)
	{
		if (!_subpaths.back().commands.empty())
			_subpaths.push_back(PathSubpath());

		_subpaths.back().points.front() = point;
	}

	void PathImpl::line_to(const Pointf &point)
	{
		_subpaths.back().points.push_back(point);
		_subpaths.back().commands.push_back(PathCommand::line);
	}

	void PathImpl::bezier_to(const Pointf &control, const Pointf &point)
	{
		_subpaths.back().points.push_back(control);
		_subpaths.back().points.push_back(point);
		_subpaths.back().commands.push_back(PathCommand::quadradic);
	}

	void PathImpl::bezier_to(const Pointf &control1, const Pointf &control2, const Pointf &point)
	{
		_subpaths.back().points.push_back(control1);
		_subpaths.back().points.push_back(control2);
		_subpaths.back().points.push_back(point);
		_subpaths.back().commands.push_back(PathCommand::cubic);
	}

	void PathImpl::close()
	{
		if (!_subpaths.back().commands.empty())
		{
			_subpaths.back().closed = true;
			_subpaths.push_back(PathSubpath());
		}
	}

	void PathImpl::add_rect(const Rectf &box)
	{
		move_to(box.top_left());
		line_to(Pointf(box.top_right()));
		line_to(Pointf(box.bottom_right()));
		line_to(Pointf(box.bottom_left()));
		close();
	}

	void PathImpl::add_line(const Pointf &start, const Pointf &end)
	{
		move_to(start);
		line_to(end);
	}

	void PathImpl::add_ellipse(const Pointf &center, const Sizef &radius)
	{
		uicore::Sizef diameter = radius * 2.0f;

		float x = center.x - radius.width;
		float y = center.y - radius.height;

		float kappa = 0.5522848f;		// Kappa is 4*((SqrRoot2) - 1) / 3
		float control_horiz = (diameter.width / 2) * kappa;
		float control_vert = (diameter.height / 2) * kappa;
		float x_end = x + diameter.width;
		float y_end = y + diameter.height;
		float x_middle = x + diameter.width / 2;
		float y_middle = y + diameter.height / 2;

		move_to(x, y_middle);
		bezier_to(Pointf(x, y_middle - control_vert), Pointf(x_middle - control_horiz, y), Pointf(x_middle, y));
		bezier_to(Pointf(x_middle + control_horiz, y), Pointf(x_end, y_middle - control_vert), Pointf(x_end, y_middle));
		bezier_to(Pointf(x_end, y_middle + control_vert), Pointf(x_middle + control_horiz, y_end), Pointf(x_middle, y_end));
		bezier_to(Pointf(x_middle - control_horiz, y_end), Pointf(x, y_middle + control_vert), Pointf(x, y_middle));
	}

	void PathImpl::add_rect(const Rectf &box, const uicore::Sizef &corner)
	{
		float x = box.left;
		float y = box.top;

		float kappa = 0.5522848f;		// Kappa is 4*((SqrRoot2) - 1) / 3
		float control_horiz = (corner.width) * kappa;
		float control_vert = (corner.height) * kappa;

		float x_middle_a = box.left + corner.width;
		float y_middle_a = box.top + corner.height;

		float x_middle_b = box.right - corner.width;
		float y_middle_b = box.bottom - corner.height;

		move_to(x, y_middle_a);

		bezier_to(Pointf(x, y_middle_a - control_vert), Pointf(x_middle_a - control_horiz, y), Pointf(x_middle_a, y));
		line_to(Pointf(x_middle_b, y));
		bezier_to(Pointf(x_middle_b + control_horiz, y), Pointf(box.right, y_middle_a - control_vert), Pointf(box.right, y_middle_a));
		line_to(Pointf(box.right, y_middle_b));
		bezier_to(Pointf(box.right, y_middle_b + control_vert), Pointf(x_middle_b + control_horiz, box.bottom), Pointf(x_middle_b, box.bottom));
		line_to(Pointf(x_middle_a, box.bottom));
		bezier_to(Pointf(x_middle_a - control_horiz, box.bottom), Pointf(x, y_middle_b + control_vert), Pointf(x, y_middle_b));
		close();
	}

	void PathImpl::add(const std::shared_ptr<Path> &path)
	{
		PathImpl *other = static_cast<PathImpl*>(path.get());
		if (other != this && !other->_subpaths.empty())
		{
			_subpaths.reserve(_subpaths.size() + other->_subpaths.size());
			_subpaths.insert(_subpaths.end(), other->_subpaths.begin(), other->_subpaths.end());
		}
	}

	void PathImpl::apply_transform(const Mat3f &transform)
	{
		for (auto & elem : _subpaths)
		{
			std::vector<Pointf> &points = elem.points;
			for (auto & point : points)
			{
				point = transform * point;
			}
		}
	}

	std::shared_ptr<Path> PathImpl::clone() const
	{
		return std::make_shared<PathImpl>(*this);
	}

	void PathImpl::stroke(const CanvasPtr &canvas, const Pen &pen)
	{
		RenderBatchPath *batcher = static_cast<CanvasImpl*>(canvas.get())->batcher.get_path_batcher();
		batcher->stroke(canvas, *this, pen);
	}

	void PathImpl::fill(const CanvasPtr &canvas, const Brush &brush)
	{
		RenderBatchPath *batcher = static_cast<CanvasImpl*>(canvas.get())->batcher.get_path_batcher();
		batcher->fill(canvas, *this, brush);
	}

	void PathImpl::fill_and_stroke(const CanvasPtr &canvas, const Pen &pen, const Brush &brush)
	{
		RenderBatchPath *batcher = static_cast<CanvasImpl*>(canvas.get())->batcher.get_path_batcher();
		batcher->fill(canvas, *this, brush);
		batcher->stroke(canvas, *this, pen);
	}
}
