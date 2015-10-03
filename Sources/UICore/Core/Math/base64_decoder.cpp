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
#include "UICore/Core/Math/base64_decoder.h"
#include "UICore/Core/System/databuffer.h"

namespace uicore
{
	namespace
	{
		/*
			'A','B','C','D','E','F','G','H','I','J','K','L','M',
			'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
			'a','b','c','d','e','f','g','h','i','j','k','l','m',
			'n','o','p','q','r','s','t','u','v','w','x','y','z',
			'0','1','2','3','4','5','6','7','8','9','+','/'
		*/

		static unsigned char char_to_base64[256] =
		{
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0, 0, 63,
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 0, 0, 0, 0, 0, 0,
			0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 0, 0, 0, 0, 0,
			0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
			41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 0, 0, 0, 0, 0,

			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
		};

		class Base64DecoderImpl : public Base64Decoder
		{
		public:
			DataBufferPtr _result = DataBuffer::create(0);
			unsigned char chunk[4];
			int chunk_filled = 0;

			static void decode_data(unsigned char *output, const unsigned char *input, int size_input)
			{
				int i, o;
				for (i = 0, o = 0; i < size_input; i += 4, o += 3)
				{
					unsigned int v1 = char_to_base64[input[i + 0]];
					unsigned int v2 = char_to_base64[input[i + 1]];
					unsigned int v3 = char_to_base64[input[i + 2]];
					unsigned int v4 = char_to_base64[input[i + 3]];
					unsigned int value = (v1 << 18) + (v2 << 12) + (v3 << 6) + v4;

					output[o + 0] = (value >> 16) & 255;
					output[o + 1] = (value >> 8) & 255;
					output[o + 2] = value & 255;
				}
			}

			const DataBufferPtr &result() const override;
			void reset() override;
			void feed(const void *data, int size, bool append_result = true) override;
		};

		const DataBufferPtr &Base64DecoderImpl::result() const
		{
			return _result;
		}

		void Base64DecoderImpl::reset()
		{
			_result->set_size(0);
			chunk_filled = 0;
		}

		void Base64DecoderImpl::feed(const void *_data, int size, bool append_result)
		{
			int pos = 0;
			const unsigned char *data = (const unsigned char *)_data;

			if (!append_result)
				_result->set_size(0);

			// Handle any left-over data from last encode:

			if (chunk_filled > 0)
			{
				int needed = 4 - chunk_filled;
				if (size >= needed)
				{
					// Finish left-over data block:

					memcpy(chunk + chunk_filled, data, needed);
					int out_pos = _result->size();
					_result->set_size(out_pos + 3);
					decode_data((unsigned char *)_result->data() + out_pos, chunk, 4);
					pos += needed;
					chunk_filled = 0;

					// Shorten result if we got an end of base64 data marker:

					if (chunk[2] == '=')
						_result->set_size(_result->size() - 2);
					else if (chunk[3] == '=')
						_result->set_size(_result->size() - 1);
				}
				else
				{
					memcpy(chunk + chunk_filled, data, size);
					chunk_filled += size;
					return;
				}
			}

			// Base64 encode what's available to us now:

			int blocks = (size - pos) / 4;
			int out_pos = _result->size();
			_result->set_size(out_pos + blocks * 3);
			decode_data((unsigned char *)_result->data() + out_pos, data + pos, blocks * 4);
			pos += blocks * 4;

			// Shorten result if we got an end of base64 data marker:

			if (blocks > 0)
			{
				if (data[pos - 2] == '=')
					_result->set_size(_result->size() - 2);
				else if (data[pos - 1] == '=')
					_result->set_size(_result->size() - 1);
			}

			// Save data for last incomplete block:

			int leftover = size - pos;
			if (leftover > 4)
				throw Exception("Base64 decoder is broken!");
			chunk_filled = leftover;
			memcpy(chunk, data + pos, leftover);
		}
	}

	std::shared_ptr<Base64Decoder> Base64Decoder::create()
	{
		return std::make_shared<Base64DecoderImpl>();
	}

	DataBufferPtr Base64Decoder::decode(const void *data, int size)
	{
		Base64DecoderImpl decoder;
		decoder.feed(data, size);
		return decoder.result();
	}

	DataBufferPtr Base64Decoder::decode(const std::string &data)
	{
		return decode(data.data(), data.length());
	}

	DataBufferPtr Base64Decoder::decode(const DataBufferPtr &data)
	{
		return decode(data->data(), data->size());
	}
}
