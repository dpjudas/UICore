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
	enum class ShaderType;

	class GraphicContextProvider
	{
	public:
		virtual ~GraphicContextProvider() { }

		virtual PixelBufferProvider *alloc_pixel_buffer() = 0;

		virtual std::shared_ptr<RasterizerState> create_rasterizer_state(const RasterizerStateDescription &desc) = 0;
		virtual std::shared_ptr<BlendState> create_blend_state(const BlendStateDescription &desc) = 0;
		virtual std::shared_ptr<DepthStencilState> create_depth_stencil_state(const DepthStencilStateDescription &desc) = 0;
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

		virtual int get_max_attributes() = 0;
		virtual Size get_max_texture_size() const = 0;

		virtual Size get_display_window_size() const = 0;
		virtual float get_pixel_ratio() const = 0;
		virtual Signal<void(const Size &)> &sig_window_resized() = 0;

		virtual std::shared_ptr<ProgramObject> get_program_object(StandardProgram standard_program) const = 0;

		virtual ClipZRange get_clip_z_range() const = 0;
		virtual TextureImageYAxis get_texture_image_y_axis() const = 0;

		virtual ShaderLanguage get_shader_language() const = 0;
		virtual int get_major_version() const = 0;
		virtual int get_minor_version() const = 0;
		virtual bool has_compute_shader_support() const = 0;

		virtual PixelBuffer get_pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const = 0;

		virtual void set_rasterizer_state(RasterizerState *state) = 0;
		virtual void set_blend_state(BlendState *state, const Colorf &blend_color, unsigned int sample_mask) = 0;
		virtual void set_depth_stencil_state(DepthStencilState *state, int stencil_ref) = 0;

		virtual void set_program_object(StandardProgram standard_program) = 0;
		virtual void set_program_object(const ProgramObjectPtr &program) = 0;
		virtual void reset_program_object() = 0;

		virtual void set_uniform_buffer(int index, const UniformBufferPtr &buffer) = 0;
		virtual void reset_uniform_buffer(int index) = 0;

		virtual void set_storage_buffer(int index, const StorageBufferPtr &buffer) = 0;
		virtual void reset_storage_buffer(int index) = 0;

		virtual void set_texture(int unit_index, const TexturePtr &texture) = 0;
		virtual void reset_texture(int unit_index) = 0;

		virtual void set_image_texture(int unit_index, const TexturePtr &texture) = 0;
		virtual void reset_image_texture(int unit_index) = 0;

		virtual bool is_frame_buffer_owner(const FrameBufferPtr &fb) = 0;
		virtual void set_frame_buffer(const FrameBufferPtr &write_buffer, const FrameBufferPtr &read_buffer) = 0;
		virtual void reset_frame_buffer() = 0;

		virtual void set_draw_buffer(DrawBuffer buffer) = 0;

		virtual bool is_primitives_array_owner(const PrimitivesArrayPtr &primitives_array) = 0;
		virtual void draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArrayPtr &primitives_array) = 0;
		virtual void set_primitives_array(const PrimitivesArrayPtr &primitives_array) = 0;
		virtual void draw_primitives_array(PrimitivesType type, int offset, int num_vertices) = 0;
		virtual void draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count) = 0;
		virtual void set_primitives_elements(ElementArrayBuffer *array_provider) = 0;
		virtual void draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset = 0) = 0;
		virtual void draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count) = 0;
		virtual void reset_primitives_elements() = 0;
		virtual void draw_primitives_elements(PrimitivesType type, int count, ElementArrayBuffer *array_provider, VertexAttributeDataType indices_type, void *offset) = 0;
		virtual void draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayBuffer *array_provider, VertexAttributeDataType indices_type, void *offset, int instance_count) = 0;
		virtual void reset_primitives_array() = 0;

		virtual void set_scissor(const Rect &rect) = 0;
		virtual void reset_scissor() = 0;

		virtual void dispatch(int x, int y, int z) = 0;

		virtual void clear(const Colorf &color) = 0;
		virtual void clear_depth(float value) = 0;
		virtual void clear_stencil(int value) = 0;

		virtual void set_viewport(const Rectf &viewport) = 0;
		virtual void set_viewport(int index, const Rectf &viewport) = 0;
		virtual void set_depth_range(float n, float f) = 0;
		virtual void set_depth_range(int viewport, float n, float f) = 0;

		virtual void flush() = 0;
	};
}
