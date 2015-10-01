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

#pragma once

#include "../System/exception.h"
#include "endian.h"

namespace uicore
{
	class IODevice
	{
	public:
		virtual ~IODevice() { }

		virtual long long size() const = 0;
		long long position() const { return const_cast<IODevice*>(this)->seek_from_current(0); }

		virtual long long seek(long long position) = 0;
		virtual long long seek_from_current(long long offset) = 0;
		virtual long long seek_from_end(long long offset) = 0;

		virtual int try_read(void *data, int size) = 0;
		void read(void *data, int size) { int bytes = try_read(data, size); if (bytes != size) throw Exception("Could not read all bytes"); }
		virtual void write(const void *data, int size) = 0;

		virtual void close() { }

		bool is_big_endian_mode() const { return swap_bytes; }
		void set_big_endian_mode(bool value = true) { swap_bytes = value; }

		uint8_t read_uint8() { return read_type<uint8_t>(); }
		int8_t read_int8() { return read_type<int8_t>(); }
		uint16_t read_uint16() { return read_type<uint16_t>(); }
		int16_t read_int16() { return read_type<int16_t>(); }
		uint32_t read_uint32() { return read_type<uint32_t>(); }
		int32_t read_int32() { return read_type<int32_t>(); }
		float read_float() { return read_type<float>(); }
		double read_double() { return read_type<double>(); }

		void write_uint8(uint8_t v) { write_type(v); }
		void write_int8(int8_t v) { write_type(v); }
		void write_uint16(uint16_t v) { write_type(v); }
		void write_int16(int16_t v) { write_type(v); }
		void write_uint32(uint32_t v) { write_type(v); }
		void write_int32(int32_t v) { write_type(v); }
		void write_uint64(uint64_t v) { write_type(v); }
		void write_int64(int64_t v) { write_type(v); }
		void write_float(float v) { write_type(v); }
		void write_double(double v) { write_type(v); }

		template<typename T> T read_type() { T v; read(&v, sizeof(T)); return swap_bytes ? Endian::swap(v) : v; }
		template<typename T> void write_type(T v) { if (swap_bytes) v = Endian::swap(v); write(&v, sizeof(T)); }

	private:
		bool swap_bytes = false;
	};
}
