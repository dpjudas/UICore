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
**    Mark Page
*/

#pragma once

#include <memory>
#include <cmath>

namespace uicore
{
	/// \brief Font metrics class.
	///
	/// <img src="../../img/FontMetrics.png">
	class FontMetrics
	{
	public:
		// \brief Text metrics constructor
		// \param line_height If 0, then line_height is calculated as height + external_leading
		FontMetrics() { }
		FontMetrics(float height, float ascent, float descent, float internal_leading, float external_leading, float line_height, float pixel_ratio)
			: _height(height), _ascent(ascent), _descent(descent), _internal_leading(internal_leading), _external_leading(external_leading), _line_height(line_height)
		{
			// Calculate line_height when not specified
			if (_line_height == 0.0f)
				_line_height = height + external_leading;

			_baseline_offset = (_line_height - _height) * 0.5f + ascent;
			_baseline_offset = std::round(_baseline_offset / pixel_ratio) * pixel_ratio;
		}

		/// \brief Returns the height of the font.
		float height() const { return _height; }

		/// \brief Return the distance between lines.
		float line_height() const { return _line_height; }

		/// \brief Returns the baseline offset from the top of a line
		float baseline_offset() const { return _baseline_offset; }

		/// \brief Returns the font ascender.
		float ascent() const { return _ascent; }

		/// \brief Returns the font descender.
		float descent() const { return _descent; }

		/// \brief Returns the amount of leading (space) inside the bounds set by the height() function.
		float internal_leading() const { return _internal_leading; }

		/// \brief Returns the amount of extra leading (space) that to add between rows.
		float external_leading() const { return _external_leading; }

	private:
		float _height = 0.0f;
		float _ascent = 0.0f;
		float _descent = 0.0f;
		float _internal_leading = 0.0f;
		float _external_leading = 0.0f;
		float _line_height = 0.0f;
		float _baseline_offset = 0.0f;
	};
}
