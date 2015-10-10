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
**    Harry Storbacka
*/

#pragma once

#include <vector>
#include "UICore/Display/Render/program_object.h"
#include "UICore/Display/Render/shader_object.h"

namespace uicore
{
	class UniformBuffer;

	class ProgramObjectImpl : public ProgramObject
	{
	public:
		using ProgramObject::set_uniform_buffer_index;
		using ProgramObject::set_storage_buffer_index;
		using ProgramObject::get_storage_buffer_index;
		using ProgramObject::get_uniform_buffer_size;

		void link() override
		{
			bool result = try_link();
			if (!result)
				throw Exception("Shader linking failed: " + get_info_log());
		}

		void set_uniform_buffer_index(const std::string &block_name, int bind_index) override
		{
			set_uniform_buffer_index(get_uniform_buffer_index(block_name), bind_index);
		}

		void set_storage_buffer_index(const std::string &block_name, int bind_index) override
		{
			set_storage_buffer_index(get_storage_buffer_index(block_name), bind_index);
		}

		int get_uniform_buffer_size(const std::string &block_name) const override
		{
			int block_index = get_uniform_buffer_index(block_name);
			return get_uniform_buffer_size(block_index);
		}
	};
}
