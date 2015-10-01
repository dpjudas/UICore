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
#include "UICore/Core/System/databuffer.h"
#include <string.h>

namespace uicore
{
	class DataBuffer_Impl : public DataBuffer
	{
	public:
		DataBuffer_Impl(size_t init_size) { set_size(init_size); }
		DataBuffer_Impl(const void *init_data, size_t init_size) { set_size(init_size); memcpy(data(), init_data, size()); }

		char *data() override { return buffer.data(); }
		const char *data() const override { return buffer.data(); }
		size_t size() const override { return buffer.size(); }
		size_t capacity() const override { return buffer.capacity(); }
		void set_size(size_t size) override { buffer.resize(size); }
		void set_capacity(size_t capacity) override { buffer.reserve(capacity); }

		std::shared_ptr<DataBuffer> copy(size_t pos, size_t size) override { return DataBuffer::create(data() + pos, size); }

	private:
		std::vector<char> buffer;
	};

	std::shared_ptr<DataBuffer> DataBuffer::create(size_t size)
	{
		return std::make_shared<DataBuffer_Impl>(size);
	}

	std::shared_ptr<DataBuffer> DataBuffer::create(const void *data, size_t size)
	{
		return std::make_shared<DataBuffer_Impl>(data, size);
	}
}
