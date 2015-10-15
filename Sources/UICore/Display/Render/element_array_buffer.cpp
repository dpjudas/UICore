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
#include "UICore/Display/Render/element_array_buffer.h"
#include "UICore/Display/Render/graphic_context.h"
#include "UICore/Display/Render/graphic_context_impl.h"
#include "UICore/Core/System/exception.h"

namespace uicore
{
	std::shared_ptr<ElementArrayBuffer> ElementArrayBuffer::create(const GraphicContextPtr &gc, int size, BufferUsage usage)
	{
		return static_cast<GraphicContextImpl*>(gc.get())->create_element_array_buffer(size, usage);
	}

	std::shared_ptr<ElementArrayBuffer> ElementArrayBuffer::create(const GraphicContextPtr &gc, const void *data, int size, BufferUsage usage)
	{
		return static_cast<GraphicContextImpl*>(gc.get())->create_element_array_buffer(data, size, usage);
	}
}
