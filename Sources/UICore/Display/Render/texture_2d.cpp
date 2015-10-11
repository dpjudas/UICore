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
*/

#include "UICore/precomp.h"
#include "UICore/Display/Render/texture_2d.h"
#include "UICore/Display/Render/texture_impl.h"
#include "UICore/Display/Render/graphic_context_impl.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Display/ImageFormats/image_file.h"
#include "UICore/Core/Math/color.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Core/Text/string_format.h"

namespace uicore
{
	std::shared_ptr<Texture2D> Texture2D::create(const GraphicContextPtr &context, int width, int height, TextureFormat format, int levels)
	{
		return static_cast<GraphicContextImpl*>(context.get())->create_texture_2d(width, height, format, levels);
	}

	std::shared_ptr<Texture2D> Texture2D::create(const GraphicContextPtr &context, const Size &size, TextureFormat texture_format, int levels)
	{
		return create(context, size.width, size.height, texture_format, levels);
	}

	std::shared_ptr<Texture2D> Texture2D::create(const GraphicContextPtr &context, const std::string &filename, const ImageImportDescription &import_desc)
	{
		PixelBufferPtr pb = ImageFile::load(filename, std::string());
		pb = import_desc.process(pb);

		auto texture = create(context, pb->width(), pb->height(), import_desc.is_srgb() ? tf_srgb8_alpha8 : tf_rgba8);
		texture->set_subimage(context, Point(0, 0), pb, Rect(pb->size()), 0);
		return texture;
	}

	std::shared_ptr<Texture2D> Texture2D::create(const GraphicContextPtr &context, const IODevicePtr &file, const std::string &image_type, const ImageImportDescription &import_desc)
	{
		PixelBufferPtr pb = ImageFile::load(file, image_type);
		pb = import_desc.process(pb);

		auto texture = create(context, pb->width(), pb->height(), import_desc.is_srgb() ? tf_srgb8_alpha8 : tf_rgba8);
		texture->set_subimage(context, Point(0, 0), pb, Rect(pb->size()), 0);
		return texture;
	}

	std::shared_ptr<Texture2D> Texture2D::create(const GraphicContextPtr &context, const PixelBufferPtr &image, bool is_srgb)
	{
		return create(context, image, image->size(), is_srgb);
	}

	std::shared_ptr<Texture2D> Texture2D::create(const GraphicContextPtr &context, const PixelBufferPtr &image, const Rect &src_rect, bool is_srgb)
	{
		auto texture = create(context, src_rect.get_width(), src_rect.get_height(), is_srgb ? tf_srgb8_alpha8 : tf_rgba8);
		texture->set_subimage(context, Point(0, 0), image, src_rect, 0);
		return texture;
	}
}
