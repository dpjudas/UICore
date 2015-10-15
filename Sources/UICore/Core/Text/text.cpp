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
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Text/utf8_reader.h"
#include "UICore/Core/System/exception.h"
#include "UICore/Core/System/databuffer.h"
#ifndef WIN32
#include <wchar.h>
#include <wctype.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#else
#include <cstring>
#if defined UNICODE && !defined _UNICODE
#define _UNICODE
#endif
#include <tchar.h>
#endif

#include <sstream>

#ifdef __MINGW32__
#include <cstdio>
#endif

// This function or variable may be unsafe. Consider using xxxx instead.
// To disable deprecation, use _CRT_SECURE_NO_DEPRECATE. See online help for details.
#ifdef WIN32
#pragma warning(disable: 4996)
#endif
// We disable this warning because the usage patterns we use here are safe,
// and although we could switch to newer more 'safe' versions, this may break
// compilation in earlier versions of Visual Studio. -- mbn 13. sep 2006

namespace uicore
{
	namespace
	{
		const char trailing_bytes_for_utf8[256] =
		{
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
			2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
		};

		const unsigned char bitmask_leadbyte_for_utf8[6] =
		{
			0x7f,
			0x1f,
			0x0f,
			0x07,
			0x03,
			0x01
		};
	}
    
	std::vector<std::string> Text::split(const std::string &text, const std::string &split_string, bool skip_empty)
	{
		std::vector<std::string> result;
		std::string::size_type end_pos = 0, begin_pos = 0;
		while (true)
		{
			end_pos = text.find(split_string, begin_pos);
			if (end_pos == std::string::npos)
			{
				if (begin_pos != text.length())
					result.push_back(text.substr(begin_pos));
				break;
			}
			else
			{
				if (!skip_empty || begin_pos != end_pos)
					result.push_back(text.substr(begin_pos, end_pos-begin_pos));
				begin_pos = end_pos + split_string.length();
			}
		}
		return result;
	}

	std::string Text::trim(const std::string &text)
	{
		std::string::size_type first_char = text.find_first_not_of(" \r\n\t");
		std::string::size_type last_char = text.find_last_not_of(" \r\n\t");
		if (first_char == std::string::npos)
			return std::string();
		if (last_char == std::string::npos)
			return std::string();
		return text.substr(first_char, last_char - first_char + 1);
	}

	std::string Text::remove_trailing_zeros(std::string text)
	{
		if (text.find_first_of('.') != std::string::npos)
		{
			while (!text.empty() && text.back() == '0') text.pop_back();
			if (!text.empty() && text.back() == '.') text.pop_back();
		}
		return text;
	}

	bool Text::equal_caseless(const std::string &a, const std::string &b)
	{
#ifdef WIN32
		return stricmp(a.c_str(), b.c_str()) == 0;
#else
		return strcasecmp(a.c_str(), b.c_str()) == 0;
#endif
	}

	bool Text::less_caseless(const std::string &a, const std::string &b)
	{
#ifdef WIN32
		return stricmp(a.c_str(), b.c_str()) < 0;
#else
		return strcasecmp(a.c_str(), b.c_str()) < 0;
#endif
	}

	std::string Text::from_wchar(wchar_t value)
	{
		return from_utf32((unsigned int)value);
	}

	std::string Text::from_utf32(unsigned int value)
	{
		char text[8];

		if ( (value < 0x80) && (value > 0) )
		{
			text[0] = (char) value;
			text[1] = 0;
		}
		else if( value < 0x800 )
		{
			text[0] = (char) (  0xc0 | ( value >> 6 ));
			text[1] = (char) (  0x80 | ( value & 0x3f ));
			text[2] = 0;
		}
		else if( value < 0x10000 )
		{
			text[0] = (char) (  0xe0 | ( value >> 12 ));
			text[1] = (char) (  0x80 | ( (value >> 6) & 0x3f ));
			text[2] = (char) (  0x80 | ( value & 0x3f ));
			text[3] = 0;
	
		}
		else if( value < 0x200000 )
		{
			text[0] = (char) (  0xf0 | ( value >> 18 ));
			text[1] = (char) (  0x80 | ( (value >> 12) & 0x3f ));
			text[2] = (char) (  0x80 | ( (value >> 6) & 0x3f ));
			text[3] = (char) (  0x80 | ( value & 0x3f ));
			text[4] = 0;

		}
		else if( value < 0x4000000 )
		{
			text[0] = (char) (  0xf8 | ( value >> 24 ));
			text[1] = (char) (  0x80 | ( (value >> 18) & 0x3f ));
			text[2] = (char) (  0x80 | ( (value >> 12) & 0x3f ));
			text[3] = (char) (  0x80 | ( (value >> 6) & 0x3f ));
			text[4] = (char) (  0x80 | ( value & 0x3f ));
			text[5] = 0;

		}
		else if( value < 0x80000000 )
		{
			text[0] = (char) (  0xfc | ( value >> 30 ));
			text[1] = (char) (  0x80 | ( (value >> 24) & 0x3f ));
			text[2] = (char) (  0x80 | ( (value >> 18) & 0x3f ));
			text[3] = (char) (  0x80 | ( (value >> 12) & 0x3f ));
			text[4] = (char) (  0x80 | ( (value >> 6) & 0x3f ));
			text[5] = (char) (  0x80 | ( value & 0x3f ));
			text[6] = 0;
		}
		else
		{
			text[0] = 0;	// Invalid wchar value
		}
		return text;
	}

	std::string Text::to_upper(const std::string &s)
	{
		// To do: fix this so it works on UTF-16 pairs
		// To do: make sure this uses invariant culture
		std::wstring result = to_utf16(s);
		std::wstring::size_type index, size;
		size = result.length();
		for (index = 0; index < size; index++)
		{
			result[index] = towupper(result[index]);
		}
		return from_utf16(result);
	}
	
	std::string Text::to_lower(const std::string &s)
	{
		// To do: fix this so it works on UTF-16 pairs
		// To do: make sure this uses invariant culture
		std::wstring result = to_utf16(s);
		std::wstring::size_type index, size;
		size = result.length();
		for (index = 0; index < size; index++)
		{
			result[index] = towlower(result[index]);
		}
		return from_utf16(result);
	}

	std::string Text::to_string(float value, int num_decimals, bool remove_zeros)
	{
		char buf[64];
		memset(buf, 0, 64);
	#ifdef WIN32
		_snprintf(buf, 63, ("%." + to_string(num_decimals) + "f").c_str(), value);
	#else
		snprintf(buf, 63, ("%." + to_string(num_decimals) + "f").c_str(), value);
	#endif
		if (remove_zeros)
			return remove_trailing_zeros(std::string(buf));
		else
			return buf;
	}
	
	std::string Text::to_string(double value, int num_decimals, bool remove_zeros)
	{
		char buf[64];
		memset(buf, 0, 64);
	#ifdef WIN32
		_snprintf(buf, 63, ("%." + to_string(num_decimals) + "f").c_str(), value);
	#else
		snprintf(buf, 63, ("%." + to_string(num_decimals) + "f").c_str(), value);
	#endif
		if (remove_zeros)
			return remove_trailing_zeros(std::string(buf));
		else
			return buf;
	}
	
	std::string Text::to_string(int value, int base, bool uppercase)
	{
		if (base == 10)
		{
			char buf[32];
			memset(buf, 0, 32);
#ifdef WIN32
			_snprintf(buf, 31, "%d", value);
#else
			snprintf(buf, 31, "%d", value);
#endif
			return buf;
		}
		else
		{
			return value >= 0 ? to_string((unsigned int)value) : "-" + to_string((unsigned int)(-value));
		}
	}

	std::string Text::to_string(unsigned int value, int base, bool uppercase)
	{
		char buf[32];
		memset(buf, 0, 32);
#ifdef WIN32
		if (base == 10)
			_snprintf(buf, 31, "%u", value);
		else if (base == 16 && !uppercase)
			_snprintf(buf, 31, "%x", value);
		else if (base == 16)
			_snprintf(buf, 31, "%X", value);
		else if (base == 8)
			_snprintf(buf, 31, "%o", value);
		else
			throw Exception("Unsupported base passed for to_string");
#else
		if (base == 10)
			snprintf(buf, 31, "%u", value);
		else if (base == 16 && !uppercase)
			snprintf(buf, 31, "%x", value);
		else if (base == 16)
			snprintf(buf, 31, "%X", value);
		else if (base == 8)
			snprintf(buf, 31, "%o", value);
		else
			throw Exception("Unsupported base passed for to_string");
#endif
		return buf;
	}

	std::string Text::to_string(unsigned long long value)
	{
		std::ostringstream stream;
		stream << value;
		return stream.str();
	}

	std::string Text::to_string(long long value)
	{
		std::ostringstream stream;
		stream << value;
		return stream.str();
	}

	std::string Text::to_string(bool value)
	{
		return value ? "true" : "false";
	}

	float Text::parse_float(const std::string &value)
	{
		float result = 0.0;
		sscanf(value.c_str(), "%f", &result);
		return result;
	}

	double Text::parse_double(const std::string &value)
	{
		double result = 0.0;
		sscanf(value.c_str(), "%lf", &result);
		return result;
	}

	int Text::parse_int32(const std::string &value, int base)
	{
		if (base == 10)
		{
			return atoi(value.c_str());
		}
		else if (base == 16)
		{
			int result = 0;
			std::string::size_type i, length;
			length = value.length();
			for (i = 0; i < length; i++)
			{
				if (value[i] >= '0' && value[i] <= '9')
				{
					result = (result << 4) + (value[i] - '0');
				}
				else if (value[i] >= 'a' && value[i] <= 'f')
				{
					result = (result << 4) + 10 + (value[i] - 'a');
				}
				else if (value[i] >= 'A' && value[i] <= 'F')
				{
					result = (result << 4) + 10 + (value[i] - 'A');
				}
				else
				{
					break;
				}
			}
			return result;
		}
		else
		{
			throw Exception("Unsupported base passed for parse_int32");
		}
	}
	
	unsigned long long Text::parse_uint64(const std::string &value, int base)
	{
		if (base == 10)
		{
			std::istringstream stream(value.c_str());
			unsigned long long num;
			stream >> num;
			return num;
		}
		else if (base == 16)
		{
			unsigned long long result = 0;
			std::string::size_type i, length;
			length = value.length();
			for (i = 0; i < length; i++)
			{
				if (value[i] >= '0' && value[i] <= '9')
				{
					result = (result << 4) + (value[i] - '0');
				}
				else if (value[i] >= 'a' && value[i] <= 'f')
				{
					result = (result << 4) + 10 + (value[i] - 'a');
				}
				else if (value[i] >= 'A' && value[i] <= 'F')
				{
					result = (result << 4) + 10 + (value[i] - 'A');
				}
				else
				{
					break;
				}
			}
			return result;
		}
		else
		{
			throw Exception("Unsupported base passed for parse_uint64");
		}
	}
	
	long long Text::parse_int64(const std::string &value, int base)
	{
		if (base == 10)
		{
			std::istringstream stream(value.c_str());
			long long num;
			stream >> num;
			return num;
		}
		else if (base == 16)
		{
			long long result = 0;
			std::string::size_type i, length;
			length = value.length();
			for (i = 0; i < length; i++)
			{
				if (value[i] >= '0' && value[i] <= '9')
				{
					result = (result << 4) + (value[i] - '0');
				}
				else if (value[i] >= 'a' && value[i] <= 'f')
				{
					result = (result << 4) + 10 + (value[i] - 'a');
				}
				else if (value[i] >= 'A' && value[i] <= 'F')
				{
					result = (result << 4) + 10 + (value[i] - 'A');
				}
				else
				{
					break;
				}
			}
			return result;
		}
		else
		{
			throw Exception("Unsupported base passed for parse_int64");
		}
	}
	
	unsigned int Text::parse_uint32(const std::string &value, int base)
	{
		if (base == 10)
		{
			return (unsigned int) atoi(value.c_str());
		}
		else if (base == 16)
		{
			unsigned int result = 0;
			std::string::size_type i, length;
			length = value.length();
			for (i = 0; i < length; i++)
			{
				if (value[i] >= '0' && value[i] <= '9')
				{
					result = (result << 4) + (value[i] - '0');
				}
				else if (value[i] >= 'a' && value[i] <= 'f')
				{
					result = (result << 4) + 10 + (value[i] - 'a');
				}
				else if (value[i] >= 'A' && value[i] <= 'F')
				{
					result = (result << 4) + 10 + (value[i] - 'A');
				}
				else
				{
					break;
				}
			}
			return result;
		}
		else
		{
			throw Exception("Unsupported base passed for parse_uint32");
		}
	}
	
	bool Text::parse_bool(const std::string &value)
	{
		return equal_caseless(value, "true") || equal_caseless(value, "yes") || value == "1";
	}

	std::string Text::from_utf16(const std::wstring &utf16)
	{
		// Calculate length:

		std::wstring::size_type length_utf16 = utf16.length();
		std::string::size_type length_utf8 = 0;
		std::wstring::size_type pos;
		for (pos = 0; pos < length_utf16; pos++)
		{
			if (utf16[pos] < 0x0080)
				length_utf8++;
			else if (utf16[pos] < 0x0800)
				length_utf8 += 2;
			else
				length_utf8 += 3;
		}
	
		// Perform conversion:
	
		std::string utf8(length_utf8, ' ');
		std::string::size_type pos_utf8 = 0;
		for (pos = 0; pos < length_utf16; pos++)
		{
			if (utf16[pos] < 0x0080)
			{
				utf8[pos_utf8++] = (char) utf16[pos];
			}
			else if (utf16[pos] < 0x0800)
			{
				utf8[pos_utf8++] = 0xc0 + (utf16[pos] >> 6);
				utf8[pos_utf8++] = 0x80 + (utf16[pos] & 0x3f);
			}
			else
			{
				utf8[pos_utf8++] = 0xe0 + (utf16[pos] >> 12);
				utf8[pos_utf8++] = 0x80 + ((utf16[pos] >> 6) & 0x3f);
				utf8[pos_utf8++] = 0x80 + (utf16[pos] & 0x3f);
			}
		}

		return utf8;
	}

	std::wstring Text::to_utf16(const std::string &utf8)
	{
		// Calculate length:

		std::wstring::size_type length_ucs2 = 0;
		std::string::size_type length_utf8 = utf8.length();
		std::string::size_type pos = 0;
		while (pos < length_utf8)
		{
			unsigned char c = utf8[pos++];
			int trailing_bytes = trailing_bytes_for_utf8[c];
			length_ucs2++;
			pos += trailing_bytes;
		}
		if (pos != length_utf8)
		{
			// error in utf8 string
			length_ucs2--;
		}
		if (pos <= 0)
			return std::wstring();

		// Perform conversion:
	
		std::wstring ucs2(length_ucs2, L'?');
		pos = 0;
		std::wstring::size_type ucs2_pos = 0;
		while (pos < length_utf8 && ucs2_pos < length_ucs2)
		{
			unsigned char c = utf8[pos++];
			int trailing_bytes = trailing_bytes_for_utf8[c];
			unsigned int ucs4 = (c & bitmask_leadbyte_for_utf8[trailing_bytes]);
			for (int i=0; i<trailing_bytes; i++)
			{
				c = (unsigned char) utf8[pos+i];
				if (c < 0xc0)
				{
					ucs4 = (ucs4 << 6) + (c & 0x3f);
				}
				else
				{
					// error in utf8 string
					ucs4 = L'?';
					break;
				}
			}
			if (ucs4 > 0 && ucs4 <= 0xffff)
				ucs2[ucs2_pos] = ucs4;
			else
				ucs2[ucs2_pos] = L'?';

			ucs2_pos++;
			pos += trailing_bytes;
		}
		return ucs2;
	}

	std::string::size_type Text::char_length(const std::string &str)
	{
		std::string::size_type len = 0;
		UTF8_Reader utf8_reader(str.data(), str.length());
		while (!utf8_reader.is_end())
		{
			len++;
			utf8_reader.next();
		}

		return len;
	}

	ByteOrderMark Text::detect_bom(const void *data, std::string::size_type length)
	{
		const unsigned char utf32_be[] = { 0x00, 0x00, 0xfe, 0xff };
		const unsigned char utf32_le[] = { 0xff, 0xfe, 0x00, 0x00 };
		const unsigned char utf16_be[] = { 0xfe, 0xff };
		const unsigned char utf16_le[] = { 0xff, 0xfe };
		const unsigned char utf8[] = { 0xef, 0xbb, 0xbf };

		if (length >= 3 && memcmp(data, utf8, 3) == 0)
			return ByteOrderMark::utf8;
		else if (length >= 2 && memcmp(data, utf16_le, 2) == 0)
			return ByteOrderMark::utf16_le;
		else if (length >= 2 && memcmp(data, utf16_be, 2) == 0)
			return ByteOrderMark::utf16_be;
		else if (length >= 4 && memcmp(data, utf32_le, 4) == 0)
			return ByteOrderMark::utf32_le;
		else if (length >= 4 && memcmp(data, utf32_be, 4) == 0)
			return ByteOrderMark::utf32_be;
		else
			return ByteOrderMark::none;
	}
}
