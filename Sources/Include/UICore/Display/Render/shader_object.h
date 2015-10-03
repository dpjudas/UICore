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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#pragma once

#include <memory>
#include "graphic_context.h"
#include <vector>

namespace uicore
{
	class IODevice;

	/// \brief Shader Type
	enum class ShaderType : int
	{
		vertex,
		geometry,
		fragment,
		tess_evaluation,
		tess_control,
		compute,
		num_types
	};

	/// \brief Shader Object
	class ShaderObject
	{
	public:
		/// \brief Constructs a shader
		static std::shared_ptr<ShaderObject> create(GraphicContext &gc, ShaderType type, const std::string &source);

		/// \brief Load
		static std::shared_ptr<ShaderObject> load(GraphicContext &gc, ShaderType type, const std::string &fullname);
		static std::shared_ptr<ShaderObject> load(GraphicContext &gc, ShaderType type, IODevice &file);

		/// \brief Load and compile
		static std::shared_ptr<ShaderObject> load_and_compile(GraphicContext &gc, ShaderType type, const std::string &filename);
		static std::shared_ptr<ShaderObject> load_and_compile(GraphicContext &gc, ShaderType type, IODevice &file);

		/// \brief Gets the shader type
		virtual ShaderType shader_type() const = 0;

		/// \brief Get shader object's info log
		virtual std::string info_log() const = 0;

		/// \brief Get shader source code.
		virtual std::string shader_source() const = 0;

		/// \brief Compile program.
		///
		/// An exception will be thrown if the compiling fails
		virtual void compile() = 0;

		/// \brief Compile program.
		///
		/// If the compiling fails, info_log() will return the compile log
		virtual bool try_compile() = 0;
	};

	typedef std::shared_ptr<ShaderObject> ShaderObjectPtr;
}
