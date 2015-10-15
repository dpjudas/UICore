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
*/

#include "UICore/precomp.h"
#include "UICore/Display/display_target.h"
#ifdef WIN32
#include "UICore/D3D/d3d_target.h"
#else
#include "UICore/GL/opengl_target.h"
#endif
#include "setup_display.h"

namespace uicore
{
	namespace
	{
		std::shared_ptr<DisplayTargetProvider> current_target;
	}

	const std::shared_ptr<DisplayTargetProvider> &DisplayTarget::get_current_target()
	{
		SetupDisplay::start();

		if (!current_target)
		{
#ifdef WIN32
			D3DTarget::set_current();
#else
			OpenGLTarget::set_current();
#endif
		}

		return current_target;
	}

	void DisplayTarget::set_current_target(const std::shared_ptr<DisplayTargetProvider> &target)
	{
		current_target = target;
	}
}
