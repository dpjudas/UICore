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
**    Mark Page
*/

#pragma once

#include <memory>
#include "UICore/Display/Render/graphic_context.h"
#include "../Image/pixel_buffer.h"
#include "font_description.h"
#include "glyph_metrics.h"

namespace uicore
{
	/// \brief FontFamily class
	///
	/// A FontFamily is a collection of font descriptions
	class FontFamily
	{
	public:
		/// \brief Constructs a font family with the given family name
		static std::shared_ptr<FontFamily> create(const std::string &family_name);

		/// \brief Font family name used for this font family
		virtual const std::string &family_name() const = 0;

		/// \brief Add standard font
		virtual void add(const std::string &typeface_name, float height) = 0;

		// \brief Add standard font
		virtual void add(const std::string &typeface_name, const FontDescription &desc) = 0;

		// \brief Add standard font
		virtual void add(const FontDescription &desc, const std::string &ttf_filename) = 0;
	};

	typedef std::shared_ptr<FontFamily> FontFamilyPtr;
}
