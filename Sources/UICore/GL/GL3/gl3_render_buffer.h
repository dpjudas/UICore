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
*/

#pragma once


#include "gl3_graphic_context.h"
#include "UICore/Display/Render/render_buffer.h"
#include "UICore/GL/gl_share_list.h"

namespace uicore
{
	class GL3RenderBuffer : public RenderBuffer, GLSharedResource
	{
	public:
		GL3RenderBuffer(int width, int height, TextureFormat texture_format, int multisample_samples);
		~GL3RenderBuffer();

		GLuint get_handle();

		Size size() const override { return _size; }

	private:
		void on_dispose() override;

		GLuint handle = 0;
		Size _size;
	};
}
