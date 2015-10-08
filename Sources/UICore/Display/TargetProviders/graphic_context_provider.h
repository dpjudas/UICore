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
*/

#pragma once

#include <memory>
#include "UICore/Display/Render/graphic_context.h"
#include "UICore/Display/Render/primitives_array.h"
#include "UICore/Display/Render/texture.h"
#include "UICore/Core/Math/mat4.h"
#include "UICore/Core/Signals/signal.h"

namespace uicore
{
	class Texture;
	class Stencil;
	class TextureProvider;
	class FontProvider;
	class Font;
	class OcclusionQuery;
	class ProgramObjectProvider;
	class ShaderObjectProvider;
	class FrameBuffer;
	class RenderBuffer;
	class VertexArrayBufferProvider;
	class ElementArray;
	class TransferBufferProvider;
	class FontDescription;
	class PixelBufferProvider;
	class UniformBufferProvider;
	class StorageBuffer;
	class PrimitivesArrayProvider;
	class RasterizerStateDescription;
	class BlendStateDescription;
	class DepthStencilStateDescription;
	class TransferTexture;
	enum class ShaderType;
	enum PixelBufferDirection;

	class GraphicContextProvider : public GraphicContext
	{
	public:
		virtual std::shared_ptr<ProgramObject> create_program() = 0;
		virtual std::shared_ptr<ShaderObject> create_shader(ShaderType type, const std::string &source) = 0;
		virtual std::shared_ptr<ShaderObject> create_shader(ShaderType type, const void *bytecode, int bytecode_size) = 0;
		virtual std::shared_ptr<OcclusionQuery> create_occlusion_query() = 0;
		virtual std::shared_ptr<FrameBuffer> create_frame_buffer() = 0;
		virtual std::shared_ptr<RenderBuffer> create_render_buffer(int width, int height, TextureFormat texture_format, int multisample_samples) = 0;
		virtual std::shared_ptr<StorageBuffer> create_storage_buffer(int size, int stride, BufferUsage usage) = 0;
		virtual std::shared_ptr<StorageBuffer> create_storage_buffer(const void *data, int size, int stride, BufferUsage usage) = 0;
		virtual std::shared_ptr<ElementArrayBuffer> create_element_array_buffer(int size, BufferUsage usage) = 0;
		virtual std::shared_ptr<ElementArrayBuffer> create_element_array_buffer(const void *data, int size, BufferUsage usage) = 0;
		virtual std::shared_ptr<VertexArrayBuffer> create_vertex_array_buffer(int size, BufferUsage usage) = 0;
		virtual std::shared_ptr<VertexArrayBuffer> create_vertex_array_buffer(const void *data, int size, BufferUsage usage) = 0;
		virtual std::shared_ptr<UniformBuffer> create_uniform_buffer(int size, BufferUsage usage) = 0;
		virtual std::shared_ptr<UniformBuffer> create_uniform_buffer(const void *data, int size, BufferUsage usage) = 0;
		virtual std::shared_ptr<TransferBuffer> create_transfer_buffer(int size, BufferUsage usage) = 0;
		virtual std::shared_ptr<TransferBuffer> create_transfer_buffer(const void *data, int size, BufferUsage usage) = 0;
		virtual std::shared_ptr<PrimitivesArray> create_primitives_array() = 0;
		virtual std::shared_ptr<Texture1D> create_texture_1d(int width, TextureFormat texture_format, int levels) = 0;
		virtual std::shared_ptr<Texture1DArray> create_texture_1d_array(int width, int array_size, TextureFormat texture_format, int levels) = 0;
		virtual std::shared_ptr<Texture2D> create_texture_2d(int width, int height, TextureFormat texture_format, int levels) = 0;
		virtual std::shared_ptr<Texture2DArray> create_texture_2d_array(int width, int height, int array_size, TextureFormat texture_format, int levels) = 0;
		virtual std::shared_ptr<Texture3D> create_texture_3d(int width, int height, int depth, TextureFormat texture_format, int levels) = 0;
		virtual std::shared_ptr<TextureCube> create_texture_cube(int width, int height, TextureFormat texture_format, int levels) = 0;
		virtual std::shared_ptr<TextureCubeArray> create_texture_cube_array(int width, int height, int array_size, TextureFormat texture_format, int levels) = 0;
		virtual std::shared_ptr<TransferTexture> create_transfer_texture(const void *data, const Size &new_size, PixelBufferDirection direction, TextureFormat new_format, BufferUsage usage) = 0;

		virtual Size display_window_size() const = 0;
		virtual Signal<void(const Size &)> &sig_window_resized() = 0;
		virtual void on_window_resized() = 0;

		virtual int max_attributes() = 0;

		void set_viewport(const Rectf &rect, TextureImageYAxis y_axis)
		{
			Rectf rect2 = rect;
			if (texture_image_y_axis() != y_axis)
			{
				rect2 = Rectf(Pointf(rect.left, size().height - rect.bottom), rect.get_size());
			}
			set_viewport(rect2);
		}

		void set_viewport(int index, const Rectf &rect, TextureImageYAxis y_axis)
		{
			Rectf rect2 = rect;
			if (texture_image_y_axis() != y_axis)
			{
				rect2 = Rectf(Pointf(rect.left, size().height - rect.bottom), rect.get_size());
			}
			set_viewport(index, rect2);
		}

		virtual void set_viewport(const Rectf &viewport) = 0;
		virtual void set_viewport(int index, const Rectf &viewport) = 0;

		void set_scissor(const Rect &rect, TextureImageYAxis y_axis) override
		{
			Rect rect2 = rect;
			if (texture_image_y_axis() != y_axis)
			{
				rect2 = Rect(Point(rect.left, size().height - rect.bottom), rect.get_size());
			}
			set_scissor(rect2);
		}

		virtual void set_scissor(const Rect &rect) = 0;

		Size size() const override
		{
			if (!write_frame_buffer())
			{
				return display_window_size();
			}
			else
			{
				return write_frame_buffer()->get_size();
			}
		}

		/*
		GraphicContextProvider()
		{
			SharedGCData::add_ref();

			resize_slot = provider->sig_window_resized().connect(bind_member(this, &GraphicContext_Impl::on_window_resized));
			set_viewport(-1, provider->get_display_window_size());

			default_rasterizer_state = impl->provider->create_rasterizer_state(RasterizerStateDescription());
			default_blend_state = impl->provider->create_blend_state(BlendStateDescription());
			default_depth_stencil_state = impl->provider->create_depth_stencil_state(DepthStencilStateDescription());

			reset_rasterizer_state();
			reset_blend_state();
			reset_depth_stencil_state();
		}

		~GraphicContextProvider()
		{
			SharedGCData::release_ref();
		}

		void on_window_resized(const Size &size)
		{
			display_window_size = size;
			if (!write_frame_buffer)
				set_viewport(-1, get_size());
		}

		Size display_window_size;
		Slot resize_slot;

		RasterizerStatePtr default_rasterizer_state;
		BlendStatePtr default_blend_state;
		DepthStencilStatePtr default_depth_stencil_state;
		*/
	};
}
