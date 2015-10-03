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
#include "UICore/Core/Math/base64_encoder.h"
#include "UICore/Core/System/databuffer.h"

namespace uicore
{
	namespace
	{
		static unsigned char base64char[64] =
		{
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
			'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
			'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
			'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
			'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
		};

		class Base64EncoderImpl : public Base64Encoder
		{
		public:
			DataBufferPtr _result = DataBuffer::create(0);
			unsigned char chunk[3];
			int chunk_filled = 0;

			static void encode_data(unsigned char *output, const unsigned char *input, int size_input)
			{
				int i, o;
				for (i = 0, o = 0; i < size_input; i += 3, o += 4)
				{
					unsigned int v1 = input[i + 0];
					unsigned int v2 = input[i + 1];
					unsigned int v3 = input[i + 2];
					unsigned int value = (v1 << 16) + (v2 << 8) + v3;

					output[o + 0] = base64char[(value >> 18) & 63];
					output[o + 1] = base64char[(value >> 12) & 63];
					output[o + 2] = base64char[(value >> 6) & 63];
					output[o + 3] = base64char[value & 63];
				}
			}

			const DataBufferPtr &result() const override;
			void reset() override;
			void feed(const void *data, int size, bool append_result = true) override;
			void finalize(bool append_result = true) override;
		};

		const DataBufferPtr &Base64EncoderImpl::result() const
		{
			return _result;
		}

		void Base64EncoderImpl::reset()
		{
			_result->set_size(0);
			chunk_filled = 0;
		}

		void Base64EncoderImpl::feed(const void *_data, int size, bool append_result)
		{
			int pos = 0;
			const unsigned char *data = (const unsigned char *)_data;

			if (!append_result)
				_result->set_size(0);

			// Handle any left-over data from last encode:

			if (chunk_filled > 0)
			{
				int needed = 3 - chunk_filled;
				if (size >= needed)
				{
					memcpy(chunk + chunk_filled, data, needed);
					int out_pos = _result->size();
					_result->set_size(out_pos + 4);
					encode_data((unsigned char *)_result->data() + out_pos, chunk, 3);
					pos += needed;
					chunk_filled = 0;
				}
				else
				{
					memcpy(chunk + chunk_filled, data, size);
					chunk_filled += size;
					return;
				}
			}

			// Base64 encode what's available to us now:

			int blocks = (size - pos) / 3;
			int out_pos = _result->size();
			_result->set_size(out_pos + blocks * 4);
			encode_data((unsigned char *)_result->data() + out_pos, data + pos, blocks * 3);
			pos += blocks * 3;

			// Save data for last incomplete block:

			int leftover = size - pos;
			if (leftover > 3)
				throw Exception("Base64 encoder is broken!");
			chunk_filled = leftover;
			memcpy(chunk, data + pos, leftover);
		}

		void Base64EncoderImpl::finalize(bool append_result)
		{
			if (!append_result)
				_result->set_size(0);
			if (chunk_filled == 0)
				return;

			// Allocate memory for last block:

			int pos = _result->size();
			_result->set_size(pos + 4);
			unsigned char *output = (unsigned char *)_result->data() + pos;
			unsigned char *input = chunk;
			int size = chunk_filled;

			// Base64 last block:

			memset(input + size, 0, 3 - size);

			unsigned int v1 = input[0];
			unsigned int v2 = input[1];
			unsigned int v3 = input[2];
			unsigned int value = (v1 << 16) + (v2 << 8) + v3;

			output[0] = base64char[(value >> 18) & 63];
			output[1] = base64char[(value >> 12) & 63];
			output[2] = base64char[(value >> 6) & 63];
			output[3] = base64char[value & 63];

			if (chunk_filled == 1)
			{
				output[2] = '=';
				output[3] = '=';
			}
			else if (chunk_filled == 2)
			{
				output[3] = '=';
			}
		}
	}

	std::shared_ptr<Base64Encoder> Base64Encoder::create()
	{
		return std::make_shared<Base64EncoderImpl>();
	}

	std::string Base64Encoder::encode(const void *data, int size)
	{
		Base64EncoderImpl encoder;
		encoder.feed(data, size);
		encoder.finalize(true);
		return std::string(encoder.result()->data(), encoder.result()->size());
	}

	std::string Base64Encoder::encode(const std::string &data)
	{
		return encode(data.data(), data.length());
	}

	std::string Base64Encoder::encode(const DataBufferPtr &data)
	{
		return encode(data->data(), data->size());
	}
}
