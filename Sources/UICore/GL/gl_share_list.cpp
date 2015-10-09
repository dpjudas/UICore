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
#include "gl_share_list.h"

namespace uicore
{
	void GLShareList::context_created(GraphicContextProvider *gc)
	{
		contexts.push_back(gc);
	}

	void GLShareList::context_destroyed(GraphicContextProvider *gc)
	{
		bool last_context = contexts.front() == contexts.back();
		if (last_context)
		{
			std::list<GLSharedResource *>::iterator it;
			for (it = resources.begin(); it != resources.end(); ++it)
				(*it)->dispose();
		}

		contexts.erase(std::find(contexts.begin(), contexts.end(), gc));
	}

	std::list<GLSharedResource *>::iterator GLShareList::resource_created(GLSharedResource *resource)
	{
		return resources.insert(resources.end(), resource);
	}

	void GLShareList::resource_destroyed(std::list<GLSharedResource *>::iterator it)
	{
		resources.erase(it);
	}

	std::list<GraphicContextProvider *> GLShareList::contexts;
	std::list<GLSharedResource *> GLShareList::resources;
}
