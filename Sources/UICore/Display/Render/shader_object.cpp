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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#include "UICore/precomp.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Core/IOData/file.h"
#include "UICore/Core/Text/string_format.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/IOData/iodevice.h"
#include "UICore/Display/Render/shader_object.h"
#include "UICore/Display/Render/shader_object_impl.h"
#include "UICore/Display/Render/graphic_context_impl.h"

namespace uicore
{
	class ShaderObject_Impl
	{
	public:
		ShaderObject_Impl() : provider(nullptr)
		{
		}

		~ShaderObject_Impl()
		{
			if (provider)
				delete provider;
		}

		ShaderObjectImpl *provider;
	};

	std::shared_ptr<ShaderObject> ShaderObject::create(const GraphicContextPtr &gc, ShaderType type, const std::string &source)
	{
		return static_cast<GraphicContextImpl*>(gc.get())->create_shader(type, source);
	}

	std::shared_ptr<ShaderObject> ShaderObject::create(const GraphicContextPtr &gc, ShaderType type, const void *bytecode, int bytecode_size)
	{
		return static_cast<GraphicContextImpl*>(gc.get())->create_shader(type, bytecode, bytecode_size);
	}

	std::shared_ptr<ShaderObject> ShaderObject::load(const GraphicContextPtr &gc, ShaderType shader_type, const std::string &filename)
	{
		return create(gc, shader_type, File::read_all_text(filename));
	}

	std::shared_ptr<ShaderObject> ShaderObject::load(const GraphicContextPtr &gc, ShaderType shader_type, const IODevicePtr &file)
	{
		int size = file->size();
		std::string source(size, 0);
		file->read(&source[0], size);

		return create(gc, shader_type, source);
	}

	std::shared_ptr<ShaderObject> ShaderObject::load_and_compile(const GraphicContextPtr &gc, ShaderType shader_type, const std::string &filename)
	{
		auto shader_object = ShaderObject::load(gc, shader_type, filename);

		if (!shader_object->try_compile())
			throw Exception(string_format("Unable to compile shader program %1: %2", filename, shader_object->info_log()));

		return shader_object;
	}

	std::shared_ptr<ShaderObject> ShaderObject::load_and_compile(const GraphicContextPtr &gc, ShaderType shader_type, const IODevicePtr &file)
	{
		auto shader_object = ShaderObject::load(gc, shader_type, file);

		if (!shader_object->try_compile())
			throw Exception(string_format("Unable to compile shader program : %1", shader_object->info_log()));

		return shader_object;
	}
}
