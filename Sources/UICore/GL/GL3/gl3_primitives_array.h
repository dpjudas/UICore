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

#include "UICore/GL/opengl_wrap.h"
#include "UICore/GL/opengl.h"
#include "UICore/Display/Render/primitives_array_impl.h"
#include "UICore/Core/System/disposable_object.h"

namespace uicore
{
	class GL3GraphicContext;

	class GL3PrimitivesArray : public PrimitivesArrayImpl, DisposableObject
	{
	public:
		GL3PrimitivesArray(GL3GraphicContext *gc_provider);
		~GL3PrimitivesArray();

		GLuint handle;

		GL3GraphicContext *get_gc_provider() { return gc_provider; }

		void set_attribute(int index, const VertexData &data, bool normalize) override;

	private:
		void on_dispose() override;

		GL3GraphicContext *gc_provider;
	};

	class PrimitivesArrayStateTracker
	{
	public:
		PrimitivesArrayStateTracker(GL3GraphicContext *gc_provider, GLuint handle);
		~PrimitivesArrayStateTracker();

	private:
		GLuint last_vao;
		bool vao_set;
	};
}
