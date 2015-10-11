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
**    Kenneth Gangstoe
**    Mark Page
*/

#pragma once

#include "UICore/Display/Render/graphic_context.h"

namespace uicore
{
	class Canvas_Impl;
	class GraphicContext;
	class RenderBatcher;
	class Colorf;
	class Pointf;
	class DisplayWindow;
	typedef std::shared_ptr<DisplayWindow> DisplayWindowPtr;
	class Path;
	class Pen;
	class Brush;

	/// \brief 2D Graphics Canvas
	class Canvas
	{
	public:
		/// \brief Constructs a null instance.
		Canvas();

		/// \brief Constructs a Canvas
		explicit Canvas(const DisplayWindowPtr &window);

		~Canvas();

		/// \brief Returns true if this object is invalid.
		bool is_null() const { return !impl; }

		/// \brief Throw an exception if this object is invalid.
		void throw_if_null() const;

		/// \brief Get gc
		///
		/// \return Graphic Context
		const GraphicContextPtr &gc() const;

		/// \brief Returns the current effective transform matrix.
		const Mat4f &transform() const;

		/// \brief Returns the inverse of the current effective transform matrix
		const Mat4f &inverse_transform() const;

		/// \brief Returns the current effective projection matrix.
		const Mat4f &projection() const;

		/// \brief Returns the current width of the context.
		inline float width() const { return gc()->dip_width(); }

		/// \brief Returns the current height of the context.
		inline float height() const { return gc()->dip_height(); }

		/// \brief Returns the current size of the context.
		inline Sizef size() const { return gc()->dip_size(); }

		/// \brief Returns the current clipping rectangle
		Rectf cliprect() const;

		/// \brief Return the content of the read buffer into a pixel buffer.
		PixelBufferPtr pixeldata(const Rect& rect, TextureFormat texture_format = tf_rgba8, bool clamp = true);

		/// \brief Return the content of the read buffer into a pixel buffer.
		PixelBufferPtr pixeldata(TextureFormat texture_format = tf_rgba8, bool clamp = true);

		/// Retrieves the display pixel ratio of the context.
		/// \seealso Resolution Independence
		float pixel_ratio() const { return gc()->pixel_ratio(); }

		/// \brief Set active rasterizer state
		void set_rasterizer_state(const RasterizerStatePtr &state);

		/// \brief Set active blend state
		void set_blend_state(const BlendStatePtr &state, const Colorf &blend_color = Colorf::white, unsigned int sample_mask = 0xffffffff);

		/// \brief Set active depth stencil state
		void set_depth_stencil_state(const DepthStencilStatePtr &state, int stencil_ref = 0);

		/// \brief Set active rasterizer state
		void reset_rasterizer_state();

		/// \brief Set active blend state
		void reset_blend_state();

		/// \brief Set active depth stencil state
		void reset_depth_stencil_state();

		/// \brief Set the current clipping rectangle.
		void set_cliprect(const Rectf &rect);

		/// \brief Push current clipping rectangle to stack.
		///
		/// If a rectangle is passed, it afterwards sets clipping
		/// rectangle to the union of the current rectangle and the passed
		/// rectangle.
		void push_cliprect(const Rectf &rect);

		/// \brief Push cliprect
		void push_cliprect();

		/// \brief Pop current clipping rectangle from the stack.
		void pop_cliprect();

		/// \brief Removes the set clipping rectangle and empties the cliprect stack.
		void reset_cliprect();

		/// \brief Clears the whole context using the specified color.
		void clear(const Colorf &color = Colorf::black);

		/// \brief Set active program object to the standard program specified.
		void set_program_object(StandardProgram standard_program);

		/// \brief Specifies which render batcher is to be currently active
		///
		/// If the render batcher is already active, nothing happens. If a different render batcher
		/// is currently active, it is flushed before the new batcher is made active.
		void set_batcher(RenderBatcher *batcher);

		/// \brief Sets the transform matrix to a new matrix.
		void set_transform(const Mat4f &matrix);

		/// \brief Multiplies the passed matrix onto the transform matrix.
		void mult_transform(const Mat4f &matrix);

		/// \brief Flushes the render batcher currently active.
		void flush();

		/// \brief Snaps the point to the nearest pixel corner
		Pointf grid_fit(const Pointf &pos);

	private:
		std::shared_ptr<Canvas_Impl> impl;

		friend class Image;
		friend class Font_Impl;
		friend class Font_DrawSubPixel;
		friend class Font_DrawFlat;
		friend class Font_DrawScaled;
		friend class Path;
	};
}
