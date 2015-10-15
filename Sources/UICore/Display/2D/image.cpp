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
**    Kenneth Gangstoe
*/

#include "UICore/precomp.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Display/2D/image.h"
#include "UICore/Display/2D/canvas.h"
#include "UICore/Display/2D/texture_group.h"
#include "UICore/Display/Render/graphic_context.h"
#include "UICore/Display/Render/texture_2d.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Math/quad.h"
#include "render_batch_triangle.h"
#include "canvas_impl.h"

namespace uicore
{
	class ImageImpl : public Image
	{
	public:
		ImageImpl(const ImageImpl &other) { *this = other; }
		ImageImpl(const CanvasPtr &canvas, const PixelBufferPtr &pb, const Rect &rect, float _pixel_ratio);
		ImageImpl(Texture2DPtr texture, const Rect &rect, float _pixel_ratio);
		ImageImpl(TextureGroupImage &sub_texture, float _pixel_ratio);
		ImageImpl(const CanvasPtr &canvas, const std::string &filename, const ImageImportDescription &import_desc, float _pixel_ratio);

		float scale_x() const override { return _scale_x; }
		float scale_y() const override { return _scale_y; }
		Colorf color() const override { return _color; }
		void get_alignment(Origin &origin, float &x, float &y) const override;
		TextureGroupImage texture() const override;
		Sizef size() const override;
		float width() const override;
		float height() const override;
		std::shared_ptr<Image> clone() const override;
		void draw(const CanvasPtr &canvas, float x, float y) const override;
		void draw(const CanvasPtr &canvas, const Rectf &src, const Rectf &dest) const override;
		void draw(const CanvasPtr &canvas, const Rectf &dest) const override;
		void draw(const CanvasPtr &canvas, const Rectf &src, const Quadf &dest) const override;
		void draw(const CanvasPtr &canvas, const Quadf &dest) const override;
		void set_scale(float x, float y) override;
		void set_color(const Colorf &color) override;
		void set_alignment(Origin origin, float x, float y) override;
		void set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t) override;
		void set_linear_filter(bool linear_filter = true) override;

	private:
		void calc_hotspot();

		Colorf _color = Colorf::white;

		float _scale_x = 1.0f;
		float _scale_y = 1.0f;

		Pointf _translation_hotspot;
		Origin _translation_origin = origin_top_left;

		Pointf _translated_hotspot;	// Precalculated from calc_hotspot()

		Texture2DPtr _texture;
		Rect _texture_rect;
		float _pixel_ratio = 1.0f;
	};

	std::shared_ptr<Image> Image::create(Texture2DPtr texture, const Rect &rect, float pixel_ratio)
	{
		return std::make_shared<ImageImpl>(texture, rect, pixel_ratio);
	}

	std::shared_ptr<Image> Image::create(TextureGroupImage &sub_texture, float pixel_ratio)
	{
		return std::make_shared<ImageImpl>(sub_texture, pixel_ratio);
	}

	std::shared_ptr<Image> Image::create(const CanvasPtr &canvas, const PixelBufferPtr &pixelbuffer, const Rect &rect, float pixel_ratio)
	{
		return std::make_shared<ImageImpl>(canvas, pixelbuffer, rect, pixel_ratio);
	}

	std::shared_ptr<Image> Image::create(const CanvasPtr &canvas, const std::string &filename, const ImageImportDescription &import_desc, float pixel_ratio)
	{
		return std::make_shared<ImageImpl>(canvas, filename, import_desc, pixel_ratio);
	}

	ImageImpl::ImageImpl(const CanvasPtr &canvas, const PixelBufferPtr &pb, const Rect &rect, float pixel_ratio)
	{
		_texture = Texture2D::create(canvas->gc(), pb->width(), pb->height(), pb->format());
		_texture->set_subimage(canvas->gc(), 0, 0, pb, rect);
		_texture_rect = Rect(0, 0, pb->width(), pb->height());
		_pixel_ratio = pixel_ratio;
	}

	ImageImpl::ImageImpl(Texture2DPtr texture, const Rect &rect, float pixel_ratio)
	{
		_texture = texture;
		_texture_rect = rect;
		_pixel_ratio = pixel_ratio;
	}

	ImageImpl::ImageImpl(TextureGroupImage &sub_texture, float pixel_ratio)
	{
		_texture = sub_texture.texture();
		_texture_rect = sub_texture.geometry();
		_pixel_ratio = pixel_ratio;
	}

	ImageImpl::ImageImpl(const CanvasPtr &canvas, const std::string &filename, const ImageImportDescription &import_desc, float pixel_ratio)
	{
		_texture = Texture2D::create(canvas->gc(), filename, import_desc);
		_texture_rect = _texture->size();
		_pixel_ratio = pixel_ratio;
	}

	std::shared_ptr<Image> ImageImpl::clone() const
	{
		return std::make_shared<ImageImpl>(*this);
	}

	TextureGroupImage ImageImpl::texture() const
	{
		return TextureGroupImage(_texture, _texture_rect);
	}

	void ImageImpl::get_alignment(Origin &origin, float &x, float &y) const
	{
		origin = _translation_origin;
		x = _translation_hotspot.x;
		y = _translation_hotspot.y;
	}

	float ImageImpl::width() const
	{
		if (_pixel_ratio != 0.0f)
			return _texture_rect.width() / _pixel_ratio;
		else
			return _texture_rect.width();
	}

	float ImageImpl::height() const
	{
		if (_pixel_ratio != 0.0f)
			return _texture_rect.height() / _pixel_ratio;
		else
			return _texture_rect.height();
	}

	Sizef ImageImpl::size() const
	{
		return Sizef(width(), height());
	}

	void ImageImpl::draw(const CanvasPtr &canvas, float x, float y) const
	{
		Rectf dest(
			x + _translated_hotspot.x, y + _translated_hotspot.y,
			Sizef(width() * _scale_x, height() * _scale_y));

		RenderBatchTriangle *batcher = static_cast<CanvasImpl*>(canvas.get())->batcher.get_triangle_batcher();
		batcher->draw_image(canvas, _texture_rect, dest, _color, _texture);
	}

	void ImageImpl::draw(const CanvasPtr &canvas, const Rectf &src, const Rectf &dest) const
	{
		Rectf new_src = src;
		new_src.translate(_texture_rect.left, _texture_rect.top);

		Rectf new_dest = dest;
		new_dest.translate(_translated_hotspot);

		RenderBatchTriangle *batcher = static_cast<CanvasImpl*>(canvas.get())->batcher.get_triangle_batcher();
		batcher->draw_image(canvas, new_src, new_dest, _color, _texture);
	}

	void ImageImpl::draw(const CanvasPtr &canvas, const Rectf &dest) const
	{
		Rectf new_dest = dest;
		new_dest.translate(_translated_hotspot);

		RenderBatchTriangle *batcher = static_cast<CanvasImpl*>(canvas.get())->batcher.get_triangle_batcher();
		batcher->draw_image(canvas, _texture_rect, new_dest, _color, _texture);
	}

	void ImageImpl::draw(const CanvasPtr &canvas, const Rectf &src, const Quadf &dest) const
	{
		Rectf new_src = src;
		new_src.translate(_texture_rect.left, _texture_rect.top);

		Quadf new_dest = dest;
		new_dest.p += _translated_hotspot;
		new_dest.q += _translated_hotspot;
		new_dest.r += _translated_hotspot;
		new_dest.s += _translated_hotspot;

		RenderBatchTriangle *batcher = static_cast<CanvasImpl*>(canvas.get())->batcher.get_triangle_batcher();
		batcher->draw_image(canvas, new_src, new_dest, _color, _texture);
	}

	void ImageImpl::draw(const CanvasPtr &canvas, const Quadf &dest) const
	{
		Quadf new_dest = dest;
		new_dest.p += _translated_hotspot;
		new_dest.q += _translated_hotspot;
		new_dest.r += _translated_hotspot;
		new_dest.s += _translated_hotspot;

		RenderBatchTriangle *batcher = static_cast<CanvasImpl*>(canvas.get())->batcher.get_triangle_batcher();
		batcher->draw_image(canvas, _texture_rect, new_dest, _color, _texture);
	}

	void ImageImpl::set_scale(float x, float y)
	{
		_scale_x = x;
		_scale_y = y;
		calc_hotspot();
	}

	void ImageImpl::set_color(const Colorf &color)
	{
		_color = color;
	}

	void ImageImpl::set_alignment(Origin origin, float x, float y)
	{
		_translation_origin = origin;
		_translation_hotspot.x = x;
		_translation_hotspot.y = y;
		calc_hotspot();
	}

	void ImageImpl::set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t)
	{
		_texture->set_wrap_mode(wrap_s, wrap_t);
	}

	void ImageImpl::set_linear_filter(bool linear_filter)
	{
		_texture->set_mag_filter(linear_filter ? filter_linear : filter_nearest);
		_texture->set_min_filter(linear_filter ? filter_linear : filter_nearest);
	}

	void ImageImpl::calc_hotspot()
	{
		switch (_translation_origin)
		{
		case origin_top_left:
		default:
			_translated_hotspot = Pointf(_translation_hotspot.x, _translation_hotspot.y);
			break;
		case origin_top_center:
			_translated_hotspot = Pointf(_translation_hotspot.x - _texture_rect.width() * _scale_x * 0.5f, _translation_hotspot.y);
			break;
		case origin_top_right:
			_translated_hotspot = Pointf(_translation_hotspot.x - _texture_rect.width() * _scale_x, _translation_hotspot.y);
			break;
		case origin_center_left:
			_translated_hotspot = Pointf(_translation_hotspot.x, _translation_hotspot.y - _texture_rect.height() * _scale_y * 0.5f);
			break;
		case origin_center:
			_translated_hotspot = Pointf(_translation_hotspot.x - _texture_rect.width() * _scale_x * 0.5f, _translation_hotspot.y - _texture_rect.height() * _scale_y * 0.5f);
			break;
		case origin_center_right:
			_translated_hotspot = Pointf(_translation_hotspot.x - _texture_rect.width() * _scale_x, _translation_hotspot.y - _texture_rect.height() * _scale_y * 0.5f);
			break;
		case origin_bottom_left:
			_translated_hotspot = Pointf(_translation_hotspot.x, _translation_hotspot.y - _texture_rect.height() * _scale_y);
			break;
		case origin_bottom_center:
			_translated_hotspot = Pointf(_translation_hotspot.x - _texture_rect.width() * _scale_x * 0.5f, _translation_hotspot.y - _texture_rect.height() * _scale_y);
			break;
		case origin_bottom_right:
			_translated_hotspot = Pointf(_translation_hotspot.x - _texture_rect.width() * _scale_x, _translation_hotspot.y - _texture_rect.height() * _scale_y);
			break;
		}

		if (_pixel_ratio != 0.0f)
		{
			_translated_hotspot.x /= _pixel_ratio;
			_translated_hotspot.y /= _pixel_ratio;
		}
	}
}
