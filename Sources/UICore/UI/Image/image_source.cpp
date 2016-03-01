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
#include "UICore/UI/Image/image_source.h"
#include "UICore/UI/UIThread/ui_thread.h"
#include "UICore/Display/2D/image.h"
#include "UICore/Display/2D/canvas.h"

namespace uicore
{
	class ImageSourceCallback : public ImageSource
	{
	public:
		ImageSourceCallback(const std::function<ImagePtr(const CanvasPtr &)> &cb_get_image) : cb_get_image(cb_get_image) { }
		ImagePtr image(const CanvasPtr &canvas) override { return cb_get_image(canvas); }

		std::function<ImagePtr(const CanvasPtr &)> cb_get_image;
	};

	std::shared_ptr<ImageSource> ImageSource::from_callback(const std::function<ImagePtr(const CanvasPtr &)> &get_image_callback)
	{
		return std::make_shared<ImageSourceCallback>(get_image_callback);
	}

	std::shared_ptr<ImageSource> ImageSource::from_resource(const std::string &resource_name)
	{
		return ImageSource::from_callback([=](const CanvasPtr &canvas)
		{
			return UIThread::image(canvas, resource_name);
		});
	}

	std::shared_ptr<ImageSource> ImageSource::from_image(const ImagePtr &image)
	{
		return ImageSource::from_callback([=](const CanvasPtr &canvas)
		{
			return image;
		});
	}
}
