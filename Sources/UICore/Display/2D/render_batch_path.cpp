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
#include "render_batch_path.h"
#include "UICore/Display/Render/blend_state_description.h"
#include "UICore/Display/2D/canvas.h"
#include "UICore/Core/Math/quad.h"
#include "path_impl.h"
#include "render_batch_buffer.h"

namespace uicore
{
	RenderBatchPath::RenderBatchPath(const GraphicContextPtr &gc, RenderBatchBuffer *batch_buffer) : batch_buffer(batch_buffer), fill_renderer(gc, batch_buffer), stroke_renderer(gc)
	{
	}

	inline Pointf RenderBatchPath::to_position(const uicore::Pointf &point) const
	{
		return Pointf(
			modelview_matrix.matrix[0 * 4 + 0] * point.x + modelview_matrix.matrix[1 * 4 + 0] * point.y + modelview_matrix.matrix[3 * 4 + 0],
			modelview_matrix.matrix[0 * 4 + 1] * point.x + modelview_matrix.matrix[1 * 4 + 1] * point.y + modelview_matrix.matrix[3 * 4 + 1]);
	}

	void RenderBatchPath::fill(const CanvasPtr &canvas, const Path &path, const Brush &brush)
	{
		canvas->set_batcher(this);

		fill_renderer.set_size(canvas, canvas->gc()->width(), canvas->gc()->height());
		fill_renderer.clear();
		render(path, &fill_renderer);
		fill_renderer.fill(canvas, path.get_impl()->fill_mode, brush, modelview_matrix);
	}

	void RenderBatchPath::stroke(const CanvasPtr &canvas, const Path &path, const Pen &pen)
	{
		canvas->set_batcher(this);

		stroke_renderer.set_pen(canvas, pen);
		render(path, &stroke_renderer);
	}

	void RenderBatchPath::flush(const GraphicContextPtr &gc)
	{
		fill_renderer.flush(gc);
	}

	void RenderBatchPath::matrix_changed(const Mat4f &new_modelview, const Mat4f &new_projection, TextureImageYAxis image_yaxis, float pixel_ratio)
	{
		// We ignore the projection
		fill_renderer.set_yaxis(image_yaxis);
		modelview_matrix = Mat4f::scale(pixel_ratio, pixel_ratio, 1.0f) * new_modelview;
	}

	void RenderBatchPath::render(const Path &path, PathRenderer *path_renderer)
	{
		for (const auto &subpath : path.get_impl()->subpaths)
		{
			uicore::Pointf start_point = to_position(subpath.points[0]);
			path_renderer->begin(start_point.x, start_point.y);

			size_t i = 1;
			for (PathCommand command : subpath.commands)
			{
				if (command == PathCommand::line)
				{
					uicore::Pointf next_point = to_position(subpath.points[i]);
					i++;

					path_renderer->line(next_point.x, next_point.y);
				}
				else if (command == PathCommand::quadradic)
				{
					uicore::Pointf control = to_position(subpath.points[i]);
					uicore::Pointf next_point = to_position(subpath.points[i + 1]);
					i += 2;

					path_renderer->quadratic_bezier(control.x, control.y, next_point.x, next_point.y);
				}
				else if (command == PathCommand::cubic)
				{
					uicore::Pointf control1 = to_position(subpath.points[i]);
					uicore::Pointf control2 = to_position(subpath.points[i + 1]);
					uicore::Pointf next_point = to_position(subpath.points[i + 2]);
					i += 3;

					path_renderer->cubic_bezier(control1.x, control1.y, control2.x, control2.y, next_point.x, next_point.y);
				}
			}

			path_renderer->end(subpath.closed);
		}
	}
}
