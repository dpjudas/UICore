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

#pragma once

#include <memory>

namespace uicore
{
	class Size;
	class Rect;
	class Texture1D;
	class Texture1DArray;
	class Texture2D;
	class Texture2DArray;
	class Texture3D;
	class TextureCube;
	class RenderBuffer;
	class GraphicContext;
	class FrameBufferProvider;
	class FrameBuffer_Impl;
	typedef std::shared_ptr<Texture1D> Texture1DPtr;
	typedef std::shared_ptr<Texture1DArray> Texture1DArrayPtr;
	typedef std::shared_ptr<Texture2D> Texture2DPtr;
	typedef std::shared_ptr<Texture2DArray> Texture2DArrayPtr;
	typedef std::shared_ptr<Texture3D> Texture3DPtr;
	typedef std::shared_ptr<TextureCube> TextureCubePtr;
	typedef std::shared_ptr<RenderBuffer> RenderBufferPtr;

	/// \brief Texture Subtype
	enum TextureSubtype
	{
		subtype_cube_map_positive_x,
		subtype_cube_map_negative_x,
		subtype_cube_map_positive_y,
		subtype_cube_map_negative_y,
		subtype_cube_map_positive_z,
		subtype_cube_map_negative_z
	};

	/// \brief Framebuffer bind target.
	enum FrameBufferBindTarget
	{
		framebuffer_draw,
		framebuffer_read
	};

	/// \brief Frame-buffer object class.
	class FrameBuffer
	{
	public:
		/// \brief Constructs a FrameBuffer
		///
		/// By default, the bind target is set to framebuffer_draw. See set_bind_target()
		static std::shared_ptr<FrameBuffer> create(GraphicContext &context);

		/// \brief Get the minumum size of all the frame buffer attachments
		virtual Size get_size() const = 0;

		/// \brief Get the bind target of the framebuffer
		///
		/// \return The bind target
		virtual FrameBufferBindTarget get_bind_target() const = 0;

		/// \brief Attach color buffer
		virtual void attach_color(int attachment_index, const RenderBufferPtr &render_buffer) = 0;
		virtual void attach_color(int attachment_index, const Texture1DPtr &texture, int level = 0) = 0;
		virtual void attach_color(int attachment_index, const Texture1DArrayPtr &texture, int array_index = -1, int level = 0) = 0;
		virtual void attach_color(int attachment_index, const Texture2DPtr &texture, int level = 0) = 0;
		virtual void attach_color(int attachment_index, const Texture2DArrayPtr &texture, int array_index = -1, int level = 0) = 0;
		virtual void attach_color(int attachment_index, const Texture3DPtr &texture, int depth, int level = 0) = 0;
		virtual void attach_color(int attachment_index, const TextureCubePtr &texture, TextureSubtype subtype, int level = 0) = 0;

		/// Detach color buffer
		virtual void detach_color(int attachment_index) = 0;

		/// Attach stencil buffer
		virtual void attach_stencil(const RenderBufferPtr &render_buffer) = 0;
		virtual void attach_stencil(const Texture2DPtr &texture, int level = 0) = 0;
		virtual void attach_stencil(const TextureCubePtr &texture, TextureSubtype subtype, int level = 0) = 0;
		virtual void detach_stencil() = 0;

		/// Attach depth buffer
		virtual void attach_depth(const RenderBufferPtr &render_buffer) = 0;
		virtual void attach_depth(const Texture2DPtr &texture, int level = 0) = 0;
		virtual void attach_depth(const TextureCubePtr &texture, TextureSubtype subtype, int level = 0) = 0;

		/// Detach depth buffer
		virtual void detach_depth() = 0;

		// Attach combined depth and stencil buffer
		virtual void attach_depth_stencil(const RenderBufferPtr &render_buffer) = 0;
		virtual void attach_depth_stencil(const Texture2DPtr &texture, int level = 0) = 0;
		virtual void attach_depth_stencil(const TextureCubePtr &texture, TextureSubtype subtype, int level = 0) = 0;

		// Detach combined depth and stencil buffer
		virtual void detach_depth_stencil() = 0;

		/// \brief Set the bind target of the framebuffer to either drawn to or read from.
		///
		/// Detach existing textures and renderbuffers before setting a new bind target
		virtual void set_bind_target(FrameBufferBindTarget target) = 0;
	};

	typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;
}
