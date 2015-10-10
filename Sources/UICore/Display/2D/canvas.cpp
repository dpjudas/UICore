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
**    Harry Storbacka
**    Kenneth Gangstoe
**    Mark Page
*/

#include "UICore/precomp.h"
#include "UICore/Display/Render/graphic_context.h"
#include "UICore/Display/2D/canvas.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Display/Render/primitives_array.h"
#include "UICore/Display/Window/display_window.h"
#include "UICore/Core/Math/line_segment.h"
#include "UICore/Core/Math/quad.h"
#include "UICore/Core/Math/triangle_math.h"
#include "render_batch_triangle.h"
#include "canvas_impl.h"
#include "UICore/Display/Font/font.h"

namespace uicore
{
	Canvas::Canvas()
	{
	}

	Canvas::Canvas(const DisplayWindowPtr &window) : impl(std::make_shared<Canvas_Impl>())
	{
		impl->init(window);
		impl->set_map_mode(map_2d_upper_left);
	}

	Canvas::~Canvas()
	{
	}

	void Canvas::throw_if_null() const
	{
		if (!impl)
			throw Exception("Canvas is null");
	}

	const GraphicContextPtr &Canvas::get_gc() const
	{
		return impl->get_gc();
	}

	const Mat4f &Canvas::get_transform() const
	{
		return impl->get_transform();
	}

	Mat4f &Canvas::get_inverse_transform()
	{
		return impl->get_inverse_transform();
	}

	const Mat4f &Canvas::get_projection() const
	{
		return impl->get_projection();
	}

	Rectf Canvas::get_cliprect() const
	{
		if (!impl->cliprects.empty())
			return impl->cliprects.back();

		return Rectf(0, 0, get_width(), get_height());
	}

	PixelBufferPtr Canvas::get_pixeldata(const Rect &rect2, TextureFormat texture_format, bool clamp)
	{
		flush();
		return get_gc()->pixeldata(rect2, texture_format, clamp);
	}

	PixelBufferPtr Canvas::get_pixeldata(TextureFormat texture_format, bool clamp)
	{
		flush();
		return get_gc()->pixeldata(texture_format, clamp);
	}

	void Canvas::set_rasterizer_state(const RasterizerStatePtr &state)
	{
		flush();
		get_gc()->set_rasterizer_state(state);
	}

	void Canvas::set_blend_state(const BlendStatePtr &state, const Colorf &blend_color, unsigned int sample_mask)
	{
		flush();
		get_gc()->set_blend_state(state, blend_color, sample_mask);
	}

	void Canvas::set_depth_stencil_state(const DepthStencilStatePtr &state, int stencil_ref)
	{
		flush();
		get_gc()->set_depth_stencil_state(state, stencil_ref);
	}

	void Canvas::reset_rasterizer_state()
	{
		flush();
		get_gc()->reset_rasterizer_state();
	}

	void Canvas::reset_blend_state()
	{
		flush();
		get_gc()->reset_blend_state();
	}

	void Canvas::reset_depth_stencil_state()
	{
		flush();
		get_gc()->reset_depth_stencil_state();
	}

	void Canvas::set_cliprect(const Rectf &rect)
	{
		flush();
		impl->set_cliprect(rect);
	}

	void Canvas::push_cliprect(const Rectf &rect)
	{
		flush();
		impl->push_cliprect(rect);
	}

	void Canvas::push_cliprect()
	{
		flush();
		impl->push_cliprect();
	}

	void Canvas::pop_cliprect()
	{
		flush();
		impl->pop_cliprect();
	}

	void Canvas::reset_cliprect()
	{
		flush();
		impl->reset_cliprect();
	}

	void Canvas::clear(const Colorf &color)
	{
		flush();
		impl->clear(color);
	}

	void Canvas::set_program_object(StandardProgram standard_program)
	{
		flush();
		get_gc()->set_program_object(standard_program);
	}

	void Canvas::set_batcher(RenderBatcher *batcher)
	{
		impl->set_batcher(*this, batcher);
	}

	void Canvas::flush()
	{
		impl->flush();
	}

	void Canvas::set_transform(const Mat4f &matrix)
	{
		impl->set_transform(matrix);
	}

	void Canvas::mult_transform(const Mat4f &matrix)
	{
		impl->set_transform(get_transform() * matrix);
	}

	Pointf Canvas::grid_fit(const Pointf &pos)
	{
		float pixel_ratio = get_gc()->pixel_ratio();
		Vec4f world_pos = get_transform() * Vec4f(pos.x, pos.y, 0.0f, 1.0f);
		world_pos.x = std::round(world_pos.x * pixel_ratio) / pixel_ratio;
		world_pos.y = std::round(world_pos.y * pixel_ratio) / pixel_ratio;
		Vec4f object_pos = get_inverse_transform() * world_pos;
		return Pointf(object_pos.x, object_pos.y);
	}
}
