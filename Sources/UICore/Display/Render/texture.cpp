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
#include "UICore/Display/Render/texture.h"
#include "UICore/Display/Render/texture_1d.h"
#include "UICore/Display/Render/texture_1d_array.h"
#include "UICore/Display/Render/texture_2d.h"
#include "UICore/Display/Render/texture_2d_array.h"
#include "UICore/Display/Render/texture_3d.h"
#include "UICore/Display/Render/texture_cube.h"
#include "UICore/Display/Render/texture_cube_array.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Display/Image/pixel_buffer_set.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Core/Text/string_format.h"

namespace uicore
{
	std::shared_ptr<Texture> Texture::create(GraphicContext &gc, PixelBufferSet pixelbuffer_set)
	{
		if (pixelbuffer_set.get_dimensions() == texture_1d)
		{
			auto texture = Texture1D::create(gc, pixelbuffer_set.get_width(), pixelbuffer_set.get_format(), pixelbuffer_set.get_max_level() + 1);
			for (int level = 0; level <= pixelbuffer_set.get_max_level(); level++)
			{
				PixelBuffer buffer = pixelbuffer_set.get_image(0, level);
				texture->set_image(gc, buffer, level);
			}
			return texture;
		}
		else if (pixelbuffer_set.get_dimensions() == texture_1d_array)
		{
			auto texture = Texture1DArray::create(gc, pixelbuffer_set.get_width(), pixelbuffer_set.get_slice_count(), pixelbuffer_set.get_format(), pixelbuffer_set.get_max_level() + 1);
			for (int slice = 0; slice < pixelbuffer_set.get_slice_count(); slice++)
			{
				for (int level = 0; level <= pixelbuffer_set.get_max_level(); level++)
				{
					PixelBuffer buffer = pixelbuffer_set.get_image(slice, level);
					texture->set_image(gc, slice, buffer, level);
				}
			}
			return texture;
		}
		else if (pixelbuffer_set.get_dimensions() == texture_2d)
		{
			auto texture = Texture2D::create(gc, pixelbuffer_set.get_width(), pixelbuffer_set.get_height(), pixelbuffer_set.get_format(), pixelbuffer_set.get_max_level() + 1);
			for (int level = 0; level <= pixelbuffer_set.get_max_level(); level++)
			{
				PixelBuffer buffer = pixelbuffer_set.get_image(0, level);
				texture->set_image(gc, buffer, level);
			}
			return texture;
		}
		else if (pixelbuffer_set.get_dimensions() == texture_2d_array)
		{
			auto texture = Texture2DArray::create(gc, pixelbuffer_set.get_width(), pixelbuffer_set.get_height(), pixelbuffer_set.get_slice_count(), pixelbuffer_set.get_format(), pixelbuffer_set.get_max_level() + 1);
			for (int slice = 0; slice < pixelbuffer_set.get_slice_count(); slice++)
			{
				for (int level = 0; level <= pixelbuffer_set.get_max_level(); level++)
				{
					PixelBuffer buffer = pixelbuffer_set.get_image(slice, level);
					texture->set_image(gc, slice, buffer, level);
				}
			}
			return texture;
		}
		else if (pixelbuffer_set.get_dimensions() == texture_3d)
		{
			auto texture = Texture3D::create(gc, pixelbuffer_set.get_width(), pixelbuffer_set.get_height(), pixelbuffer_set.get_slice_count(), pixelbuffer_set.get_format(), pixelbuffer_set.get_max_level() + 1);
			for (int slice = 0; slice < pixelbuffer_set.get_slice_count(); slice++)
			{
				for (int level = 0; level <= pixelbuffer_set.get_max_level(); level++)
				{
					PixelBuffer buffer = pixelbuffer_set.get_image(slice, level);
					texture->set_image(gc, buffer, slice, level);
				}
			}
			return texture;
		}
		else if (pixelbuffer_set.get_dimensions() == texture_cube)
		{
			auto texture = TextureCube::create(gc, pixelbuffer_set.get_width(), pixelbuffer_set.get_height(), pixelbuffer_set.get_format(), pixelbuffer_set.get_max_level() + 1);
			for (int slice = 0; slice < pixelbuffer_set.get_slice_count(); slice++)
			{
				for (int level = 0; level <= pixelbuffer_set.get_max_level(); level++)
				{
					PixelBuffer buffer = pixelbuffer_set.get_image(slice, level);
					texture->set_image(gc, (TextureCubeDirection)slice, buffer, level);
				}
			}
			return texture;
		}
		else if (pixelbuffer_set.get_dimensions() == texture_cube_array)
		{
			auto texture = TextureCubeArray::create(gc, pixelbuffer_set.get_width(), pixelbuffer_set.get_height(), pixelbuffer_set.get_slice_count(), pixelbuffer_set.get_format(), pixelbuffer_set.get_max_level() + 1);
			for (int slice = 0; slice < pixelbuffer_set.get_slice_count(); slice++)
			{
				for (int level = 0; level <= pixelbuffer_set.get_max_level(); level++)
				{
					PixelBuffer buffer = pixelbuffer_set.get_image(slice, level);
					texture->set_image(gc, slice / 6, (TextureCubeDirection)(slice % 6), buffer, level);
				}
			}
			return texture;
		}
		else
		{
			throw Exception("Unsupported texture dimensions");
		}
	}
}
