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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#include "UICore/precomp.h"
#include "gl3_shader_object.h"
#include "gl3_graphic_context.h"
#include "UICore/Core/System/exception.h"
#include "UICore/Core/Text/text.h"
#include "UICore/Core/Text/string_format.h"

namespace uicore
{
	GL3ShaderObject::GL3ShaderObject(ShaderType shader_type, const std::string &source)
	{
		OpenGL::set_active();

		type = shader_type;
		handle = glCreateShader(shadertype_to_opengl(type));

		const GLchar *sources[1];
		GLint source_lengths[1];
		source_lengths[0] = source.length();
		sources[0] = source.c_str();
		glShaderSource(handle, 1, sources, source_lengths);
	}

	GL3ShaderObject::~GL3ShaderObject()
	{
		dispose();
	}

	void GL3ShaderObject::on_dispose()
	{
		if (handle)
		{
			if (OpenGL::set_active())
			{
				glDeleteShader(handle);
				handle = 0;
			}
		}
	}

	unsigned int GL3ShaderObject::get_handle() const
	{
		return (unsigned int)handle;
	}

	bool GL3ShaderObject::get_compile_status() const
	{
		OpenGL::set_active();
		GLint status = 0;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
		return (status != GL_FALSE);
	}

	ShaderType GL3ShaderObject::shader_type() const
	{
		return type;
	}

	std::string GL3ShaderObject::info_log() const
	{
		OpenGL::set_active();
		std::string result;
		GLsizei buffer_size = 16 * 1024;
		while (buffer_size < 2 * 1024 * 1024)
		{
			auto info_log = new GLchar[buffer_size];
			GLsizei length = 0;
			glGetShaderInfoLog(handle, buffer_size, &length, info_log);
			if (length < buffer_size - 1)
				result = std::string(info_log, length);
			delete[] info_log;
			if (length < buffer_size - 1)
				break;
			buffer_size *= 2;
		}
		return result;
	}

	std::string GL3ShaderObject::shader_source() const
	{
		OpenGL::set_active();
		std::string result;
		GLsizei buffer_size = 16 * 1024;
		while (buffer_size < 2 * 1024 * 1024)
		{
			auto shader_source = new GLchar[buffer_size];
			GLsizei length = 0;
			glGetShaderSource(handle, buffer_size, &length, shader_source);
			if (length < buffer_size - 1)
				result = std::string(shader_source, length);
			delete[] shader_source;
			if (length < buffer_size - 1)
				break;
			buffer_size *= 2;
		}
		return result;
	}

	bool GL3ShaderObject::try_compile()
	{
		OpenGL::set_active();
		glCompileShader(handle);
		return get_compile_status();
	}

	GLenum GL3ShaderObject::shadertype_to_opengl(ShaderType type)
	{
		switch (type)
		{
		case ShaderType::vertex:
			return GL_VERTEX_SHADER;
		case ShaderType::geometry:
			return GL_GEOMETRY_SHADER;
		case ShaderType::fragment:
			return GL_FRAGMENT_SHADER;
		case ShaderType::tess_evaluation:
			return GL_TESS_EVALUATION_SHADER;
		case ShaderType::tess_control:
			return GL_TESS_CONTROL_SHADER;
		case ShaderType::compute:
			return GL_COMPUTE_SHADER;
		default:
			throw Exception(string_format("GL3ShaderObject: Unknown shader type: %1", (int)type));
		}
	}
}
