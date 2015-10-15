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
*/

#include "UICore/precomp.h"
#include "UICore/Display/Image/pixel_buffer_set.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include <algorithm>

namespace uicore
{
	class PixelBufferSetImpl : public PixelBufferSet
	{
	public:
		PixelBufferSetImpl(TextureDimensions dimensions, TextureFormat format, int width, int height, int slices)
			: _dimensions(dimensions), _format(format), _width(width), _height(height), _slices(slices)
		{
		}

		TextureDimensions dimensions() const override { return _dimensions; }
		TextureFormat format() const override { return _format; }
		int width() const override { return _width; }
		int height() const override { return _height; }
		int slice_count() const override { return (int)_slices.size(); }
		int base_level() const override { return _base_level; }
		int max_level() const override { return _max_level; }

		PixelBufferPtr image(int slice, int level) override;
		void set_image(int slice, int level, const PixelBufferPtr &image) override;

	private:
		TextureDimensions _dimensions;
		TextureFormat _format;
		int _width;
		int _height;
		std::vector<std::vector<PixelBufferPtr> > _slices;
		int _base_level = -1;
		int _max_level = -1;
	};

	std::shared_ptr<PixelBufferSet> PixelBufferSet::create(TextureDimensions dimensions, TextureFormat format, int width, int height, int slices)
	{
		return std::make_shared<PixelBufferSetImpl>(dimensions, format, width, height, slices);
	}

	std::shared_ptr<PixelBufferSet> PixelBufferSet::create(const PixelBufferPtr &image)
	{
		auto set = create(texture_2d, image->format(), image->width(), image->height(), 1);
		set->set_image(0, 0, image);
		return set;
	}

	PixelBufferPtr PixelBufferSetImpl::image(int slice, int level)
	{
		if (slice < 0 || slice >= (int)_slices.size() || level < 0)
			throw Exception("Out of bounds");

		if (_slices[slice].size() > (size_t)level)
		{
			return _slices[slice][level];
		}
		else
		{
			return PixelBufferPtr();
		}
	}

	void PixelBufferSetImpl::set_image(int slice, int level, const PixelBufferPtr &image)
	{
		if (slice < 0 || slice >= (int)_slices.size() || level < 0)
			throw Exception("Out of bounds");

		if (_slices[slice].size() <= (size_t)level)
			_slices[slice].resize(level + 1);
		_slices[slice][level] = image;

		if (_base_level == -1)
			_base_level = level;
		else
			_base_level = std::min(_base_level, level);

		if (_max_level == -1)
			_max_level = level;
		else
			_max_level = std::max(_max_level, level);
	}
}
