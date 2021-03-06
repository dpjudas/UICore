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

#pragma once

#include "UICore/Display/Render/primitives_array.h"

namespace uicore
{
	class PrimitivesArrayImpl : public PrimitivesArray
	{
	public:
		class VertexData
		{
		public:
			VertexData(VertexArrayBuffer *array_provider, VertexAttributeDataType type, size_t offset, int size, int stride) :
				array_provider(array_provider), type(type), offset(offset), size(size), stride(stride) {}

			VertexData() : array_provider(), type(), offset(), size(), stride() {}

			VertexArrayBuffer *array_provider;
			VertexAttributeDataType type;
			size_t offset;
			int size;
			int stride;
		};

		virtual void set_attribute(int index, const VertexData &data, bool normalize = false) = 0;

		void set_attributes(int index, const std::shared_ptr<VertexArrayBuffer> &buffer, int size, VertexAttributeDataType type, size_t offset, int stride, bool normalize) override
		{
			VertexData data(buffer.get(), type, offset, size, stride);
			set_attribute(index, data, normalize);
		}
	};
}
