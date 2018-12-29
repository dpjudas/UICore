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

#pragma once

#include "../View/view.h"

namespace uicore
{
	class Image;

	class ProgressView : public View
	{
	public:
		float progress() const;
		void set_progress(float value, bool animated = false);

		std::shared_ptr<Image> track_image() const;
		void set_track_image(const std::shared_ptr<Image> &value);

		std::shared_ptr<Image> progress_image() const;
		void set_progress_image(const std::shared_ptr<Image> &value);

		Colorf progress_color() const;
		void set_progress_color(const Colorf &value);
	};
}
