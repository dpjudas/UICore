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
**  Note: Some of the libraries UICore may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
*/

#include "UICore/precomp.h"
#include "canvas_impl.h"
#include "UICore/Display/2D/render_batcher.h"
#include "UICore/Display/Render/graphic_context_impl.h"

namespace uicore
{
	CanvasImpl::CanvasImpl(const DisplayWindowPtr &window)
	{
		current_window = window;
		_gc = window->gc();

		rasterizer_state = _gc->create_rasterizer_state(RasterizerStateDescription());
		depth_stencil_state = _gc->create_depth_stencil_state(DepthStencilStateDescription());
		opaque_blend = _gc->create_blend_state(BlendStateDescription::opaque());

		gc_clip_z_range = _gc->clip_z_range();
		canvas_inverse_transform = canvas_transform = Mat4f::identity();
		canvas_inverse_transform_set = true;

		batcher = CanvasBatcher(_gc);

		if (!_gc->write_frame_buffer())	// No framebuffer attached to canvas
		{
			canvas_y_axis = y_axis_top_down;
		}
		else
		{
			if (_gc->texture_image_y_axis() == y_axis_bottom_up)
			{
				canvas_y_axis = y_axis_bottom_up;
			}
			else
			{
				canvas_y_axis = y_axis_top_down;
			}
		}

		update_viewport_size();

		calculate_map_mode_matrices();
	}

	void CanvasImpl::begin()
	{
		update_viewport_size();

		gc()->set_viewport(gc()->size(), gc()->texture_image_y_axis());
		gc()->set_rasterizer_state(rasterizer_state);
		gc()->set_depth_stencil_state(depth_stencil_state);
		gc()->set_blend_state(nullptr);
		if (!cliprects.empty())
			write_clip(cliprects.back());
	}

	void CanvasImpl::end()
	{
		batcher.flush();

		if (!cliprects.empty())
			gc()->reset_scissor();
		gc()->set_rasterizer_state(nullptr);
		gc()->set_depth_stencil_state(nullptr);
		gc()->set_blend_state(nullptr);
		gc()->set_program_object(nullptr);
		gc()->set_viewport(gc()->size(), gc()->texture_image_y_axis());
	}

	Pointf CanvasImpl::grid_fit(const Pointf &pos)
	{
		float pixel_ratio = gc()->pixel_ratio();
		Vec4f world_pos = transform() * Vec4f(pos.x, pos.y, 0.0f, 1.0f);
		world_pos.x = std::round(world_pos.x * pixel_ratio) / pixel_ratio;
		world_pos.y = std::round(world_pos.y * pixel_ratio) / pixel_ratio;
		Vec4f object_pos = inverse_transform() * world_pos;
		return Pointf(object_pos.x, object_pos.y);
	}

	void CanvasImpl::update_batcher_matrix()
	{
		batcher.update_batcher_matrix(_gc, canvas_transform, canvas_projection, canvas_y_axis);
	}

	void CanvasImpl::set_batcher(RenderBatcher *new_batcher)
	{
		if (batcher.set_batcher(gc(), new_batcher))
			update_batcher_matrix();
	}

	void CanvasImpl::calculate_map_mode_matrices()
	{
		Mat4f matrix;
		Mat4f pixel_scaling_matrix = Mat4f::scale(_gc->pixel_ratio(), _gc->pixel_ratio(), 1.0f);

		MapMode mode = (canvas_y_axis == y_axis_bottom_up) ? top_down_map_mode() : canvas_map_mode;
		switch (mode)
		{
		default:
		case map_2d_upper_left:
			matrix = Mat4f::ortho_2d(viewport_rect.left, viewport_rect.right, viewport_rect.bottom, viewport_rect.top, handed_right, gc_clip_z_range) * pixel_scaling_matrix;
			break;
		case map_2d_lower_left:
			matrix = Mat4f::ortho_2d(viewport_rect.left, viewport_rect.right, viewport_rect.top, viewport_rect.bottom, handed_right, gc_clip_z_range) * pixel_scaling_matrix;
			break;
		}

		if (matrix != canvas_projection)
		{
			canvas_projection = matrix;
			update_batcher_matrix();
		}
	}

	MapMode CanvasImpl::top_down_map_mode() const
	{
		switch (canvas_map_mode)
		{
		default:
		case map_2d_upper_left: return map_2d_lower_left;
		case map_2d_lower_left: return map_2d_upper_left;
		}
	}

	void CanvasImpl::set_transform(const Mat4f &matrix)
	{
		canvas_transform = matrix;
		canvas_inverse_transform_set = false;
		update_batcher_matrix();
	}

	const Mat4f &CanvasImpl::transform() const
	{
		return canvas_transform;
	}

	const Mat4f &CanvasImpl::inverse_transform() const
	{
		if (!canvas_inverse_transform_set)
		{
			canvas_inverse_transform_set = true;
			canvas_inverse_transform = uicore::Mat4f::inverse(canvas_transform);
		}
		return canvas_inverse_transform;
	}

	const Mat4f &CanvasImpl::projection() const
	{
		return canvas_projection;
	}

	void CanvasImpl::set_map_mode(MapMode map_mode)
	{
		canvas_map_mode = map_mode;
		calculate_map_mode_matrices();
	}

	void CanvasImpl::update_viewport_size()
	{
		Rectf size(gc()->size());
		if (size != viewport_rect)
		{
			viewport_rect = size;
			calculate_map_mode_matrices();
		}
	}

	void CanvasImpl::set_viewport(const Rectf &viewport)
	{
		viewport_rect = viewport * (1.0f * gc()->pixel_ratio());
		calculate_map_mode_matrices();
		gc()->set_viewport(viewport_rect, y_axis_top_down);
	}

	void CanvasImpl::clear(const Colorf &color)
	{
		if (!cliprects.empty()) // D3D target doesn't restrict clear to the scissor rect
		{
			batcher.flush();
			gc()->set_blend_state(opaque_blend);

			batcher.set_batcher(gc(), batcher.get_triangle_batcher());
			batcher.get_triangle_batcher()->fill(shared_from_this(), 0.0f, 0.0f, width(), height(), color);
			batcher.flush();

			gc()->set_blend_state(nullptr);
		}
		else
		{
			batcher.flush();
			gc()->clear(color);
		}
	}

	void CanvasImpl::write_clip(const Rectf &rect)
	{
		if ((rect.left > rect.right) || (rect.top > rect.bottom))
			throw Exception("Invalid cliprect");

		// Grid-fitted, display pixel ratio scaled clipping rect
		Rect recti{
			static_cast<int>(std::round(rect.left * gc()->pixel_ratio())),
			static_cast<int>(std::round(rect.top * gc()->pixel_ratio())),
			static_cast<int>(std::round(rect.right * gc()->pixel_ratio())),
			static_cast<int>(std::round(rect.bottom * gc()->pixel_ratio()))
		};

		gc()->set_scissor(recti, canvas_y_axis ? y_axis_top_down : y_axis_bottom_up);
	}

	void CanvasImpl::set_clip(const Rectf &rect)
	{
		batcher.flush();

		if (!cliprects.empty())
			cliprects.back() = rect;
		else
			cliprects.push_back(rect);
		write_clip(rect);
	}

	void CanvasImpl::push_clip(const Rectf &rect)
	{
		batcher.flush();

		if (!cliprects.empty())
		{
			Rectf r = cliprects.back();
			r.overlap(rect);
			cliprects.push_back(r);
		}
		else
		{
			cliprects.push_back(rect);
		}

		write_clip(cliprects.back());
	}

	void CanvasImpl::push_clip()
	{
		batcher.flush();

		if (cliprects.empty())
		{
			cliprects.push_back(gc()->size());
		}
		else
		{
			cliprects.push_back(cliprects.back());
		}

		write_clip(cliprects.back());
	}

	void CanvasImpl::pop_clip()
	{
		if (!cliprects.empty())
		{
			batcher.flush();

			cliprects.pop_back();

			if (cliprects.empty())
				gc()->reset_scissor();
			else
				write_clip(cliprects.back());
		}
	}

	void CanvasImpl::reset_clip()
	{
		if (!cliprects.empty())
		{
			batcher.flush();
			cliprects.clear();
			gc()->reset_scissor();
		}
	}

	void CanvasImpl::texture_coords(const Vec2f *triangles, int num_vertex, const Texture2DPtr &texture, const Rect &texture_rect, std::vector<Vec2f> &out_texture_positions)
	{
		out_texture_positions.clear();
		out_texture_positions.reserve(num_vertex);
		if (num_vertex)
		{
			Rectf bounding_box = triangles_bounding_box(triangles, num_vertex);
			Sizef bounding_box_size = bounding_box.size();
			if (bounding_box_size.width <= 0.0f)
				bounding_box_size.width = 1.0f;
			if (bounding_box_size.height <= 0.0f)
				bounding_box_size.height = 1.0f;

			Sizef texture_size = texture->size();
			Rectf texture_rect_scaled(texture_rect.left / texture_size.width, texture_rect.top / texture_size.height, texture_rect.right / texture_size.width, texture_rect.bottom / texture_size.height);

			Sizef bounding_box_invert_size(texture_rect_scaled.width() / bounding_box_size.width, texture_rect_scaled.height() / bounding_box_size.height);

			for (; num_vertex > 0; --num_vertex)
			{
				Vec2f point = *(triangles++);
				point.x -= bounding_box.left;
				point.y -= bounding_box.top;
				point.x *= bounding_box_invert_size.width;
				point.y *= bounding_box_invert_size.height;

				point.x += texture_rect_scaled.left;
				point.y += texture_rect_scaled.top;

				out_texture_positions.push_back(point);
			}
		}
	}

	Rectf CanvasImpl::triangles_bounding_box(const Vec2f *triangles, int num_vertex)
	{
		Rectf bounding_box;
		if (num_vertex)
			bounding_box = Rectf(triangles->x, triangles->y, triangles->x, triangles->y);

		for (--num_vertex; num_vertex > 0; --num_vertex)
		{
			triangles++;
			bounding_box.left = min(triangles->x, bounding_box.left);
			bounding_box.top = min(triangles->y, bounding_box.top);
			bounding_box.right = max(triangles->x, bounding_box.right);
			bounding_box.bottom = max(triangles->y, bounding_box.bottom);
		}
		return bounding_box;
	}
}
