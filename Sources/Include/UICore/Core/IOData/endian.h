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

#include <cstdint>

#if defined(_MSC_VER)
#include <stdlib.h>
#elif !defined(__APPLE__)
#include <byteswap.h>
#define HAS_BYTESWAP
#endif

namespace uicore
{
#if defined(_MSC_VER)

	class Endian
	{
	public:
		static uint8_t swap(uint8_t val)
		{
			return val;
		}

		static int8_t swap(int8_t val)
		{
			return val;
		}

		static uint16_t swap(uint16_t val)
		{
			return _byteswap_ushort(val);
		}

		static int16_t swap(int16_t val)
		{
			return (int16_t)swap((uint16_t)val);
		}

		static uint32_t swap(uint32_t val)
		{
			return _byteswap_ulong(val);
		}

		static int32_t swap(int32_t val)
		{
			return (int32_t)swap((uint32_t)val);
		}

		static uint64_t swap(uint64_t val)
		{
			return _byteswap_uint64(val);
		}

		static int64_t swap(int64_t val)
		{
			return (int64_t)swap((uint64_t)val);
		}

		static float swap(float val)
		{
			return val;
		}

		static double swap(double val)
		{
			return val;
		}
	};

#elif defined(HAS_BYTESWAP)

	class Endian
	{
	public:
		static uint8_t swap(uint8_t val)
		{
			return val;
		}

		static int8_t swap(int8_t val)
		{
			return val;
		}

		static uint16_t swap(uint16_t val)
		{
#ifdef bswap_16
			return bswap_16(val);
#else
			return __bswap_16(val);
#endif
		}

		static int16_t swap(int16_t val)
		{
			return (int16_t)swap((uint16_t)val);
		}

		static uint32_t swap(uint32_t val)
		{
#ifdef bswap_32
			return bswap_32(val);
#else
			return __bswap_32(val);
#endif
		}

		static int32_t swap(int32_t val)
		{
			return (int32_t)swap((uint32_t)val);
		}

		static uint64_t swap(uint64_t val)
		{
#ifdef bswap_64
			return bswap_64(val);
#else
			return __bswap_64(val);
#endif
		}

		static int64_t swap(int64_t val)
		{
			return (int64_t)swap((uint64_t)val);
		}

		static float swap(float val)
		{
			return val;
		}

		static double swap(double val)
		{
			return val;
		}
	};

#else

	class Endian
	{
	public:
		static uint8_t swap(uint8_t val)
		{
			return val;
		}

		static int8_t swap(int8_t val)
		{
			return val;
		}

		static uint16_t swap(uint16_t val)
		{
			return (val << 8) | (val >> 8);
		}

		static int16_t swap(int16_t val)
		{
			return (val << 8) | ((val >> 8) & 0xFF);
		}

		static uint32_t swap(uint32_t val)
		{
			val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
			return (val << 16) | (val >> 16);
		}

		static int32_t swap(int32_t val)
		{
			val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
			return (val << 16) | ((val >> 16) & 0xFFFF);
		}

		static uint64_t swap(uint64_t val)
		{
			val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
			val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
			return (val << 32) | (val >> 32);
		}

		static int64_t swap(int64_t val)
		{
			val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
			val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
			return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
		}

		static float swap(float val)
		{
			return val;
		}

		static double swap(double val)
		{
			return val;
		}
	};

#endif
}

