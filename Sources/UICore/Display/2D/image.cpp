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
**    Kenneth Gangstoe
*/

#include "UICore/precomp.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Display/2D/image.h"
#include "UICore/Display/2D/canvas.h"
#include "UICore/Display/2D/subtexture.h"
#include "UICore/Display/Render/graphic_context.h"
#include "UICore/Display/Render/texture_2d.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Math/quad.h"
#include "render_batch_triangle.h"
#include "canvas_impl.h"

namespace uicore
{
	class Image_Impl
	{
	public:
		void calc_hotspot();

		Colorf color = Colorf::white;

		float scale_x = 1.0f, scale_y = 1.0f;

		Pointf translation_hotspot;
		Origin translation_origin = origin_top_left;

		Pointf translated_hotspot;	// Precalculated from calc_hotspot()

		Texture2DPtr texture;
		Rect texture_rect;
		float pixel_ratio = 1.0f;
	};

	void Image_Impl::calc_hotspot()
	{
		switch (translation_origin)
		{
		case origin_top_left:
		default:
			translated_hotspot = Pointf(translation_hotspot.x, translation_hotspot.y);
			break;
		case origin_top_center:
			translated_hotspot = Pointf(translation_hotspot.x - texture_rect.get_width() * scale_x * 0.5f, translation_hotspot.y);
			break;
		case origin_top_right:
			translated_hotspot = Pointf(translation_hotspot.x - texture_rect.get_width() * scale_x, translation_hotspot.y);
			break;
		case origin_center_left:
			translated_hotspot = Pointf(translation_hotspot.x, translation_hotspot.y - texture_rect.get_height() * scale_y * 0.5f);
			break;
		case origin_center:
			translated_hotspot = Pointf(translation_hotspot.x - texture_rect.get_width() * scale_x * 0.5f, translation_hotspot.y - texture_rect.get_height() * scale_y * 0.5f);
			break;
		case origin_center_right:
			translated_hotspot = Pointf(translation_hotspot.x - texture_rect.get_width() * scale_x, translation_hotspot.y - texture_rect.get_height() * scale_y * 0.5f);
			break;
		case origin_bottom_left:
			translated_hotspot = Pointf(translation_hotspot.x, translation_hotspot.y - texture_rect.get_height() * scale_y);
			break;
		case origin_bottom_center:
			translated_hotspot = Pointf(translation_hotspot.x - texture_rect.get_width() * scale_x * 0.5f, translation_hotspot.y - texture_rect.get_height() * scale_y);
			break;
		case origin_bottom_right:
			translated_hotspot = Pointf(translation_hotspot.x - texture_rect.get_width() * scale_x, translation_hotspot.y - texture_rect.get_height() * scale_y);
			break;
		}

		if (pixel_ratio != 0.0f)
		{
			translated_hotspot.x /= pixel_ratio;
			translated_hotspot.y /= pixel_ratio;
		}
	}

	Image::Image()
	{
	}

	Image::Image(const CanvasPtr &canvas, const PixelBufferPtr &pb, const Rect &rect, float pixel_ratio)
		: impl(std::make_shared<Image_Impl>())
	{
		impl->texture = Texture2D::create(canvas->gc(), pb->width(), pb->height(), pb->format());
		impl->texture->set_subimage(canvas->gc(), 0, 0, pb, rect);
		impl->texture_rect = Rect(0, 0, pb->width(), pb->height());
		impl->pixel_ratio = pixel_ratio;
	}

	Image::Image(Texture2DPtr texture, const Rect &rect, float pixel_ratio)
		: impl(std::make_shared<Image_Impl>())
	{
		impl->texture = texture;
		impl->texture_rect = rect;
		impl->pixel_ratio = pixel_ratio;
	}

	Image::Image(Subtexture &sub_texture, float pixel_ratio)
		: impl(std::make_shared<Image_Impl>())
	{
		impl->texture = sub_texture.get_texture();
		impl->texture_rect = sub_texture.get_geometry();
		impl->pixel_ratio = pixel_ratio;
	}

	Image::Image(const CanvasPtr &canvas, const std::string &filename, const ImageImportDescription &import_desc, float pixel_ratio)
		: impl(std::make_shared<Image_Impl>())
	{
		impl->texture = Texture2D::create(canvas->gc(), filename, import_desc);
		impl->texture_rect = impl->texture->size();
		impl->pixel_ratio = pixel_ratio;
	}

	Image::~Image()
	{
	}

	Image Image::clone() const
	{
		Image copy;
		copy.impl = std::shared_ptr<Image_Impl>(new Image_Impl());
		*(copy.impl) = *impl;
		return copy;
	}

	Subtexture Image::get_texture() const
	{
		return Subtexture(impl->texture, impl->texture_rect);
	}

	void Image::throw_if_null() const
	{
		if (!impl)
			throw Exception("Image is null");
	}

	float Image::get_scale_x() const
	{
		return impl->scale_x;
	}

	float Image::get_scale_y() const
	{
		return impl->scale_y;
	}

	float Image::get_alpha() const
	{
		return impl->color.a;
	}

	Colorf Image::get_color() const
	{
		return impl->color;
	}

	void Image::get_alignment(Origin &origin, float &x, float &y) const
	{
		origin = impl->translation_origin;
		x = impl->translation_hotspot.x;
		y = impl->translation_hotspot.y;
	}

	float Image::get_width() const
	{
		if (impl->pixel_ratio != 0.0f)
			return impl->texture_rect.get_width() / impl->pixel_ratio;
		else
			return impl->texture_rect.get_width();
	}

	float Image::get_height() const
	{
		if (impl->pixel_ratio != 0.0f)
			return impl->texture_rect.get_height() / impl->pixel_ratio;
		else
			return impl->texture_rect.get_height();
	}

	Sizef Image::get_size() const
	{
		return Sizef(get_width(), get_height());
	}

	void Image::draw(const CanvasPtr &canvas, float x, float y) const
	{
		Rectf dest(
			x + impl->translated_hotspot.x, y + impl->translated_hotspot.y,
			Sizef(get_width() * impl->scale_x, get_height() * impl->scale_y));

		RenderBatchTriangle *batcher = static_cast<CanvasImpl*>(canvas.get())->batcher.get_triangle_batcher();
		batcher->draw_image(canvas, impl->texture_rect, dest, impl->color, impl->texture);
	}

	void Image::draw(const CanvasPtr &canvas, const Rectf &src, const Rectf &dest) const
	{
		Rectf new_src = src;
		new_src.translate(impl->texture_rect.left, impl->texture_rect.top);

		Rectf new_dest = dest;
		new_dest.translate(impl->translated_hotspot);

		RenderBatchTriangle *batcher = static_cast<CanvasImpl*>(canvas.get())->batcher.get_triangle_batcher();
		batcher->draw_image(canvas, new_src, new_dest, impl->color, impl->texture);
	}

	void Image::draw(const CanvasPtr &canvas, const Rectf &dest) const
	{
		Rectf new_dest = dest;
		new_dest.translate(impl->translated_hotspot);

		RenderBatchTriangle *batcher = static_cast<CanvasImpl*>(canvas.get())->batcher.get_triangle_batcher();
		batcher->draw_image(canvas, impl->texture_rect, new_dest, impl->color, impl->texture);
	}

	void Image::draw(const CanvasPtr &canvas, const Rectf &src, const Quadf &dest) const
	{
		Rectf new_src = src;
		new_src.translate(impl->texture_rect.left, impl->texture_rect.top);

		Quadf new_dest = dest;
		new_dest.p += impl->translated_hotspot;
		new_dest.q += impl->translated_hotspot;
		new_dest.r += impl->translated_hotspot;
		new_dest.s += impl->translated_hotspot;

		RenderBatchTriangle *batcher = static_cast<CanvasImpl*>(canvas.get())->batcher.get_triangle_batcher();
		batcher->draw_image(canvas, new_src, new_dest, impl->color, impl->texture);
	}

	void Image::draw(const CanvasPtr &canvas, const Quadf &dest) const
	{
		Quadf new_dest = dest;
		new_dest.p += impl->translated_hotspot;
		new_dest.q += impl->translated_hotspot;
		new_dest.r += impl->translated_hotspot;
		new_dest.s += impl->translated_hotspot;

		RenderBatchTriangle *batcher = static_cast<CanvasImpl*>(canvas.get())->batcher.get_triangle_batcher();
		batcher->draw_image(canvas, impl->texture_rect, new_dest, impl->color, impl->texture);
	}

	void Image::set_scale(float x, float y)
	{
		impl->scale_x = x;
		impl->scale_y = y;
		impl->calc_hotspot();
	}

	void Image::set_alpha(float alpha)
	{
		impl->color.a = alpha;
	}

	void Image::set_color(const Colorf &color)
	{
		impl->color = color;
	}

	void Image::set_alignment(Origin origin, float x, float y)
	{
		impl->translation_origin = origin;
		impl->translation_hotspot.x = x;
		impl->translation_hotspot.y = y;
		impl->calc_hotspot();
	}

	void Image::set_wrap_mode(
		TextureWrapMode wrap_s,
		TextureWrapMode wrap_t)
	{
		impl->texture->set_wrap_mode(wrap_s, wrap_t);
	}

	void Image::set_linear_filter(bool linear_filter)
	{
		impl->texture->set_mag_filter(linear_filter ? filter_linear : filter_nearest);
		impl->texture->set_min_filter(linear_filter ? filter_linear : filter_nearest);
	}

	void Image::set_subimage(const CanvasPtr &canvas, int x, int y, const PixelBufferPtr &image, const Rect &src_rect, int level)
	{
		impl->texture->set_subimage(canvas->gc(), x, y, image, src_rect, level);
	}
}
