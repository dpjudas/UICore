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

#include "UICore/precomp.h"
#include "gl3_occlusion_query.h"
#include "UICore/GL/opengl_wrap.h"
#include "gl3_graphic_context.h"

namespace uicore
{
	GL3OcclusionQueryProvider::GL3OcclusionQueryProvider(GL3GraphicContextProvider *gc_provider)
		: handle(0), gc_provider(gc_provider)
	{
		OpenGL::set_active(gc_provider);

		if (handle)
		{
			glDeleteQueries(1, &handle);
			handle = 0;
		}

		glGenQueries(1, &handle);
	}

	GL3OcclusionQueryProvider::~GL3OcclusionQueryProvider()
	{
		dispose();
	}

	void GL3OcclusionQueryProvider::on_dispose()
	{
		if (handle)
		{
			if (OpenGL::set_active())
			{
				glDeleteQueries(1, &handle);
				handle = 0;
			}
		}
	}

	bool GL3OcclusionQueryProvider::is_result_ready() const
	{
		OpenGL::set_active(gc_provider);
		int available;
		glGetQueryObjectiv(handle, GL_QUERY_RESULT_AVAILABLE, &available);
		return (available != 0);
	}

	int GL3OcclusionQueryProvider::result() const
	{
		OpenGL::set_active(gc_provider);
		GLint result;
		glGetQueryObjectiv(handle, GL_QUERY_RESULT, &result);
		return result;
	}

	void GL3OcclusionQueryProvider::begin()
	{
		OpenGL::set_active(gc_provider);
		glBeginQuery(GL_SAMPLES_PASSED, handle);
	}

	void GL3OcclusionQueryProvider::end()
	{
		OpenGL::set_active(gc_provider);
		glEndQuery(GL_SAMPLES_PASSED);
	}
}
