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
#include "UICore/Display/Render/staging_texture.h"
#include "UICore/Display/Render/graphic_context.h"
#include "UICore/Display/Render/graphic_context_impl.h"

namespace uicore
{
	std::shared_ptr<StagingTexture> StagingTexture::create(const std::shared_ptr<GraphicContext> &gc, int width, int height, StagingDirection direction, TextureFormat texture_format, const void *data, BufferUsage usage)
	{
		return static_cast<GraphicContextImpl*>(gc.get())->create_staging_texture(data, Size(width, height), direction, texture_format, usage);
	}

	std::shared_ptr<StagingTexture> StagingTexture::create(const std::shared_ptr<GraphicContext> &gc, const std::shared_ptr<PixelBuffer> &pbuff, StagingDirection direction, BufferUsage usage)
	{
		return static_cast<GraphicContextImpl*>(gc.get())->create_staging_texture(pbuff->data(), pbuff->size(), direction, pbuff->format(), usage);
	}
}
