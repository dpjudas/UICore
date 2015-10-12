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

#pragma once

#include "UICore/Display/Render/graphic_context.h"
#include "UICore/Display/2D/render_batch_buffer.h"
#include "UICore/Display/2D/render_batch_triangle.h"
#include "UICore/Display/2D/render_batch_line.h"
#include "UICore/Display/2D/render_batch_line_texture.h"
#include "UICore/Display/2D/render_batch_point.h"
#include "UICore/Display/2D/canvas.h"
#include "UICore/Display/Window/display_window.h"
#include "canvas_batcher.h"

namespace uicore
{
	class RenderBatcher;
	class RenderBatchTriangle;

	enum MapMode
	{
		map_2d_upper_left,
		map_2d_lower_left
	};

	class CanvasImpl : public Canvas, public std::enable_shared_from_this<CanvasImpl>
	{
	public:
		CanvasImpl(const DisplayWindowPtr &window);

		const GraphicContextPtr &gc() const override { return _gc; }

		float width() const override { return gc()->dip_width(); }
		float height() const override { return gc()->dip_height(); }
		Sizef size() const override { return gc()->dip_size(); }
		float pixel_ratio() const override { return gc()->pixel_ratio(); }

		void clear(const Colorf &color) override;

		void begin() override;
		void end() override;

		Rectf clip() const override
		{
			if (!cliprects.empty())
				return cliprects.back();
			else
				return Rectf(0, 0, width(), height());
		}

		void set_clip(const Rectf &rect) override;
		void push_clip(const Rectf &rect)  override;
		void push_clip() override;
		void pop_clip() override;
		void reset_clip() override;

		void set_transform(const Mat4f &matrix) override;
		const Mat4f &transform() const override;
		const Mat4f &inverse_transform() const override;
		const Mat4f &projection() const override;

		Pointf grid_fit(const Pointf &pos) override;

		void set_batcher(RenderBatcher *batcher);

		void set_map_mode(MapMode map_mode);
		void update_viewport_size();
		void set_viewport(const Rectf &viewport);

		static Rectf triangles_bounding_box(const Vec2f *triangles, int num_vertex);
		static void texture_coords(const Vec2f *triangles, int num_vertex, const Texture2DPtr &texture, const Rect &texture_rect, std::vector<Vec2f> &out_texture_positions);

		std::vector<Rectf> cliprects;
		CanvasBatcher batcher;

	private:
		void calculate_map_mode_matrices();
		MapMode top_down_map_mode() const;
		void update_batcher_matrix();
		void write_clip(const Rectf &rect);

		GraphicContextPtr _gc;

		Mat4f canvas_transform;
		mutable bool canvas_inverse_transform_set = false;
		mutable Mat4f canvas_inverse_transform;
		Mat4f canvas_projection;
		MapMode canvas_map_mode = map_2d_upper_left;
		Rectf viewport_rect;

		DisplayWindowPtr current_window;

		RasterizerStatePtr rasterizer_state;
		BlendStatePtr opaque_blend;
		DepthStencilStatePtr depth_stencil_state;

		TextureImageYAxis canvas_y_axis;

		ClipZRange gc_clip_z_range;
	};
}
