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
*/

#include "UICore/precomp.h"
#include "UICore/GL/opengl_wrap.h"
#include "UICore/GL/opengl.h"
#include "gl3_render_buffer.h"

namespace uicore
{
	GL3RenderBufferProvider::GL3RenderBufferProvider(int width, int height, TextureFormat texture_format, int multisample_samples) : _size({width, height})
	{
		OpenGL::set_active();
		GLuint last_render_buffer = 0;
		glGetIntegerv(GL_RENDERBUFFER_BINDING, (GLint *)&last_render_buffer);

		TextureFormat_GL tf = OpenGL::get_textureformat(texture_format);
		if (!tf.valid)
			throw Exception("Texture format not supported by OpenGL");

		glGenRenderbuffers(1, &handle);
		glBindRenderbuffer(GL_RENDERBUFFER, handle);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, multisample_samples, tf.pixel_format, width, height);

		glBindRenderbuffer(GL_RENDERBUFFER, last_render_buffer);
	}

	GL3RenderBufferProvider::~GL3RenderBufferProvider()
	{
		dispose();
	}

	void GL3RenderBufferProvider::on_dispose()
	{
		if (handle)
		{
			if (OpenGL::set_active())
			{
				glDeleteRenderbuffers(1, &handle);
				handle = 0;
			}
		}
	}

	GLuint GL3RenderBufferProvider::get_handle()
	{
		return handle;
	}
}
