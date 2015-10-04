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

#pragma once

#include <memory>
#include "graphic_context.h"

namespace uicore
{
	class GraphicContext;

	/// \brief Graphics occlusion query class.
	class OcclusionQuery
	{
	public:
		/// \brief Constructs an occlusion query object.
		static std::shared_ptr<OcclusionQuery> create(GraphicContext &context);

		/// \brief Returns the result of the occlusion query.
		virtual int result() const = 0;

		/// \brief Returns true if the GPU is ready to return the result.
		virtual bool is_result_ready() const = 0;

		/// \brief Start occlusion query.
		virtual void begin() = 0;

		/// \brief Finish occlusion query.
		virtual void end() = 0;
	};

	typedef std::shared_ptr<OcclusionQuery> OcclusionQueryPtr;
}
