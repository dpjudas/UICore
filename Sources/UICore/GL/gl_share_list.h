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
**    Mark Page
*/

#pragma once

#include "UICore/Core/System/disposable_object.h"
#include <memory>
#include <list>

namespace uicore
{
	class GraphicContextImpl;
	class GLSharedResource;

	class GLShareList
	{
	public:
		static void context_created(GraphicContextImpl *gc);
		static void context_destroyed(GraphicContextImpl *gc);

		static GraphicContextImpl *any_context() { return !contexts.empty() ? contexts.front() : nullptr; }
		static const std::list<GraphicContextImpl *> &all_contexts() { return contexts; }

	private:
		static std::list<GLSharedResource *>::iterator resource_created(GLSharedResource *resource);
		static void resource_destroyed(std::list<GLSharedResource *>::iterator it);

		static std::list<GraphicContextImpl *> contexts;
		static std::list<GLSharedResource *> resources;

		friend class GLSharedResource;
	};

	class GLSharedResource : public DisposableObject
	{
	public:
		GLSharedResource() { shared_it = GLShareList::resource_created(this); }
		virtual ~GLSharedResource() { GLShareList::resource_destroyed(shared_it); }

	private:
		std::list<GLSharedResource *>::iterator shared_it;
	};
}
