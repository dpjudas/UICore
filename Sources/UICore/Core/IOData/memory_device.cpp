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
#include "UICore/Core/IOData/memory_device.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/System/exception.h"
#include "UICore/Core/System/databuffer.h"
#include <algorithm>

#undef min
#undef max

namespace uicore
{
	class MemoryDeviceImpl : public MemoryDevice
	{
	public:
		MemoryDeviceImpl() : _buffer(DataBuffer::create(0)) { }
		MemoryDeviceImpl(const DataBufferPtr &buffer) : _buffer(buffer) { }

		const DataBufferPtr &buffer() const override { return _buffer; }
		void set_buffer(const DataBufferPtr &buffer) override { _buffer = buffer; _pos = 0; }

		long long size() const override { return _buffer->size(); }

		long long seek(long long pos) { if (pos >= 0 && pos < size()) _pos = pos; return _pos; }
		long long seek_from_current(long long offset) { return seek(_pos + offset); }
		long long seek_from_end(long long offset) { return seek(size() + offset); }

		int try_read(void *data, int size)
		{
			if (size < 0)
				throw Exception("Read failed");

			size = std::min(size, (int)(_buffer->size() - _pos));
			memcpy(data, _buffer->data() + _pos, size);
			return size;
		}

		void write(const void *data, int size)
		{
			if (size < 0)
				throw Exception("Write failed");

			if (_pos + size > 0x7ffffff)
				throw Exception("Memory buffer max size exceeeded");

			if (_pos + size > _buffer->capacity())
				_buffer->set_capacity(std::max(_pos + size, static_cast<long long>(_buffer->capacity()) * 2));

			if (_pos + size > _buffer->size())
				_buffer->set_size(_pos + size);

			memcpy(_buffer->data() + _pos, data, size);
			_pos += size;
		}

		MemoryDeviceImpl(const MemoryDeviceImpl &) = delete;
		MemoryDeviceImpl &operator=(const MemoryDeviceImpl &) = delete;

		DataBufferPtr _buffer;
		long long _pos = 0;
	};

	std::shared_ptr<MemoryDevice> MemoryDevice::create()
	{
		return std::make_shared<MemoryDeviceImpl>();
	}

	std::shared_ptr<MemoryDevice> MemoryDevice::open(const DataBufferPtr &buffer)
	{
		return std::make_shared<MemoryDeviceImpl>(buffer);
	}
}
