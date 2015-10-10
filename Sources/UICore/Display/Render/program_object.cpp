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
**    Kenneth Gangstoe
*/

#include "UICore/precomp.h"
#include "UICore/Core/IOData/path_help.h"
#include "UICore/Display/Render/shader_object.h"
#include "UICore/Display/Render/program_object.h"
#include "UICore/Display/Render/graphic_context.h"
#include "UICore/Display/Render/graphic_context_impl.h"
#include "UICore/Display/Render/program_object_impl.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Text/string_format.h"
#include "UICore/Core/IOData/iodevice.h"

namespace uicore
{
	std::shared_ptr<ProgramObject> ProgramObject::create(const GraphicContextPtr &gc)
	{
		return static_cast<GraphicContextImpl*>(gc.get())->create_program();
	}

	std::shared_ptr<ProgramObject> ProgramObject::load(const GraphicContextPtr &gc, const std::string &vertex_filename, const std::string &fragment_filename)
	{
		auto program_object = create(gc);

		auto vertex_shader = ShaderObject::load_and_compile(gc, ShaderType::vertex, vertex_filename);
		program_object->attach(vertex_shader);

		auto fragment_shader = ShaderObject::load_and_compile(gc, ShaderType::fragment, fragment_filename);
		program_object->attach(fragment_shader);

		return program_object;
	}

	std::shared_ptr<ProgramObject> ProgramObject::load(const GraphicContextPtr &gc, const std::string &vertex_filename, const std::string &geometry_filename, const std::string &fragment_filename)
	{
		auto program_object = create(gc);

		auto vertex_shader = ShaderObject::load_and_compile(gc, ShaderType::vertex, vertex_filename);
		program_object->attach(vertex_shader);

		auto geometry_shader = ShaderObject::load_and_compile(gc, ShaderType::geometry, geometry_filename);
		program_object->attach(geometry_shader);

		auto fragment_shader = ShaderObject::load_and_compile(gc, ShaderType::fragment, fragment_filename);
		program_object->attach(fragment_shader);

		return program_object;
	}

	std::shared_ptr<ProgramObject> ProgramObject::load(const GraphicContextPtr &gc, IODevice &vertex_file, IODevice &fragment_file)
	{
		auto program_object = create(gc);

		auto vertex_shader = ShaderObject::load_and_compile(gc, ShaderType::vertex, vertex_file);
		program_object->attach(vertex_shader);

		auto fragment_shader = ShaderObject::load_and_compile(gc, ShaderType::fragment, fragment_file);
		program_object->attach(fragment_shader);

		return program_object;
	}

	std::shared_ptr<ProgramObject> ProgramObject::load(const GraphicContextPtr &gc, IODevice &vertex_file, IODevice &geometry_file, IODevice &fragment_file)
	{
		auto program_object = create(gc);

		auto vertex_shader = ShaderObject::load_and_compile(gc, ShaderType::vertex, vertex_file);
		program_object->attach(vertex_shader);

		auto geometry_shader = ShaderObject::load_and_compile(gc, ShaderType::geometry, geometry_file);
		program_object->attach(geometry_shader);

		auto fragment_shader = ShaderObject::load_and_compile(gc, ShaderType::fragment, fragment_file);
		program_object->attach(fragment_shader);

		return program_object;
	}

	std::shared_ptr<ProgramObject> ProgramObject::load_and_link(const GraphicContextPtr &gc, const std::string &vertex_filename, const std::string &fragment_filename)
	{
		auto program_object = load(gc, vertex_filename, fragment_filename);

		if (!program_object->try_link())
			throw Exception(string_format("Unable to link program object: %1", program_object->get_info_log()));

		return program_object;
	}

	std::shared_ptr<ProgramObject> ProgramObject::load_and_link(const GraphicContextPtr &gc, const std::string &vertex_filename, const std::string &geometry_filename, const std::string &fragment_filename)
	{
		auto program_object = load(gc, vertex_filename, geometry_filename, fragment_filename);

		if (!program_object->try_link())
			throw Exception(string_format("Unable to link program object: %1", program_object->get_info_log()));

		return program_object;
	}

	std::shared_ptr<ProgramObject> ProgramObject::load_and_link(const GraphicContextPtr &gc, IODevice &vertex_file, IODevice &fragment_file)
	{
		auto program_object = load(gc, vertex_file, fragment_file);

		if (!program_object->try_link())
			throw Exception(string_format("Unable to link program object: %1", program_object->get_info_log()));

		return program_object;
	}

	std::shared_ptr<ProgramObject> ProgramObject::load_and_link(const GraphicContextPtr &gc, IODevice &vertex_file, IODevice &geometry_file, IODevice &fragment_file)
	{
		auto program_object = load(gc, vertex_file, geometry_file, fragment_file);

		if (!program_object->try_link())
			throw Exception(string_format("Unable to link program object: %1", program_object->get_info_log()));

		return program_object;
	}
}
