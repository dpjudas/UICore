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
#include "UICore/UI/View/view_geometry.h"
#include "UICore/UI/Style/style_cascade.h"
#include "UICore/UI/Style/style_get_value.h"

namespace uicore
{
	Rectf ViewGeometry::margin_box() const
	{
		return Rectf::ltrb(
			content_x - margin_left - border_left - padding_left,
			content_y - margin_top - border_top - padding_top,
			content_x + content_width + margin_right + border_right + padding_right,
			content_y + content_height + margin_bottom + border_bottom + padding_bottom);
	}

	Rectf ViewGeometry::border_box() const
	{
		return Rectf::ltrb(
			content_x - border_left - padding_left,
			content_y - border_top - padding_top,
			content_x + content_width + border_right + padding_right,
			content_y + content_height + border_bottom + padding_bottom);
	}

	Rectf ViewGeometry::padding_box() const
	{
		return Rectf::ltrb(
			content_x - padding_left,
			content_y - padding_top,
			content_x + content_width + padding_right,
			content_y + content_height + padding_bottom);
	}

	Rectf ViewGeometry::content_box() const
	{
		return Rectf::xywh(content_x, content_y, content_width, content_height);
	}

	Pointf ViewGeometry::content_pos() const
	{
		return Pointf(content_x, content_y);
	}

	Sizef ViewGeometry::content_size() const
	{
		return Sizef(content_width, content_height);
	}
}
