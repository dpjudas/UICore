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

#pragma once


#include "UICore/Display/Render/shader_object_impl.h"
#include "UICore/GL/opengl_wrap.h"
#include "UICore/GL/opengl.h"
#include "UICore/GL/gl_share_list.h"

namespace uicore
{
	class GL3GraphicContext;

	class GL3ShaderObject : public ShaderObjectImpl, GLSharedResource
	{
	public:
		GL3ShaderObject(ShaderType type, const std::string &source);
		virtual ~GL3ShaderObject();

		unsigned int get_handle() const;
		bool get_compile_status() const;

		ShaderType shader_type() const override;
		std::string info_log() const override;
		std::string shader_source() const override;
		bool try_compile() override;

	private:
		void on_dispose() override;
		GLenum shadertype_to_opengl(ShaderType type);

		GLuint handle = 0;
		ShaderType type;
	};
}
