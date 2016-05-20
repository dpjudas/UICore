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

#include "../../Core/Math/mat4.h"
#include "../../Core/Math/mat3.h"
#include "../../Core/Math/point.h"
#include "../../Core/Math/size.h"
#include "../../Core/Math/rect.h"
#include "../../Core/Math/color.h"

namespace uicore
{
	class DisplayWindow;
	typedef std::shared_ptr<DisplayWindow> DisplayWindowPtr;
	class GraphicContext;
	typedef std::shared_ptr<GraphicContext> GraphicContextPtr;
	class BlendState;
	typedef std::shared_ptr<BlendState> BlendStatePtr;
	class Path;
	class Pen;
	class Brush;

	/// \brief 2D Graphics Canvas
	class Canvas
	{
	public:
		/// \brief Constructs a canvas
		static std::shared_ptr<Canvas> create(const DisplayWindowPtr &window);

		/// \brief Returns the graphic context associated with this canvas
		virtual const GraphicContextPtr &gc() const = 0;

		/// \brief Returns the transform matrix
		virtual const Mat4f &transform() const = 0;

		/// \brief Returns the inverse of the transform matrix
		virtual const Mat4f &inverse_transform() const = 0;

		/// \brief Returns the current effective projection matrix
		virtual const Mat4f &projection() const = 0;

		/// \brief Returns the current width of the context.
		virtual float width() const = 0;

		/// \brief Returns the current height of the context.
		virtual float height() const = 0;

		/// \brief Returns the current size of the context.
		virtual Sizef size() const = 0;

		/// \brief Returns the current clipping rectangle
		virtual Rectf clip() const = 0;

		/// Retrieves the display pixel ratio of the context.
		/// \seealso Resolution Independence
		virtual float pixel_ratio() const = 0;

		/// \brief Set the current clipping rectangle.
		virtual void set_clip(const Rectf &rect) = 0;

		/// \brief Push current clipping rectangle to stack.
		///
		/// If a rectangle is passed, it afterwards sets clipping
		/// rectangle to the union of the current rectangle and the passed
		/// rectangle.
		virtual void push_clip(const Rectf &rect) = 0;
		virtual void push_clip() = 0;

		/// \brief Pop current clipping rectangle from the stack.
		virtual void pop_clip() = 0;

		/// \brief Removes any active clipping rectangle and empties the clip stack.
		virtual void reset_clip() = 0;

		/// \brief Clears the whole context using the specified color.
		virtual void clear(const Colorf &color = StandardColorf::black()) = 0;

		/// \brief Sets the transform matrix to a new matrix.
		virtual void set_transform(const Mat4f &matrix) = 0;

		/// \brief Multiplies the passed matrix onto the transform matrix.
		void mult_transform(const Mat4f &matrix) { set_transform(transform() * matrix); }

		/// \brief Marks the start of a canvas rendering pass
		///
		/// Prepares the graphic context for rendering by binding the appropriate state.
		virtual void begin() = 0;

		/// \brief Marks the end of a canvas rendering pass
		///
		/// All batched rendering requests are flushed and the graphic context state is reset to its defaults.
		virtual void end() = 0;

		/// \brief Snaps the point to the nearest pixel corner
		virtual Pointf grid_fit(const Pointf &pos) = 0;
	};

	typedef std::shared_ptr<Canvas> CanvasPtr;
}
