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

	/// \brief Interface for implementing a GraphicContext target.
	class GraphicContextProvider
	{
	public:
		virtual ~GraphicContextProvider() { return; }

		/// \brief Returns the maximum amount of attributes available.
		virtual int get_max_attributes() = 0;

		/// \brief Returns the maximum size of a texture this graphic context supports.
		/** <p>It returns Size(0,0) if there is no known limitation to the max
			texture size.</p>*/
		virtual Size get_max_texture_size() const = 0;

		/// \brief Returns the current size of the display window
		virtual Size get_display_window_size() const = 0;

		/// \brief Physical pixels/dots per inch
		virtual float get_pixel_ratio() const = 0;

		/// \brief Get the window resized signal
		virtual Signal<void(const Size &)> &sig_window_resized() = 0;

		/// \brief Get a program object from the standard program specified
		virtual std::shared_ptr<ProgramObject> get_program_object(StandardProgram standard_program) const = 0;

		/// \brief Returns in what range clip space z values are clipped.
		virtual ClipZRange get_clip_z_range() const = 0;

		/// \brief Returns the Y axis direction for viewports, clipping rects, textures and render targets
		virtual TextureImageYAxis get_texture_image_y_axis() const = 0;

		/// \brief Returns the shader language used
		virtual ShaderLanguage get_shader_language() const = 0;

		/// \brief Returns the major version / feature level supported by the hardware
		///
		/// For the OpenGL target, this returns the major OpenGL version the driver supports.
		/// For the Direct3D target, this returns the major feature level.
		virtual int get_major_version() const = 0;

		/// \brief Returns the major version / feature level supported by the hardware
		///
		/// For the OpenGL target, this returns the minor OpenGL version the driver supports.
		/// For the Direct3D target, this returns the minor feature level.
		virtual int get_minor_version() const = 0;

		/// \brief Returns true if the hardware supports compute shaders
		///
		/// This always returns true for OpenGL 4.3 or newer, or Direct3D 11.0 or newer.
		/// For Direct3D 10.0 and 10.1 the support for compute shaders is optional.
		virtual bool has_compute_shader_support() const = 0;

		/// \brief Return the content of the draw buffer into a pixel buffer.
		virtual PixelBuffer get_pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const = 0;

		/// \brief Allocate texture provider for this gc.
		virtual TextureProvider *alloc_texture(TextureDimensions texture_dimensions) = 0;

		/// \brief Allocate pixel buffer provider for this gc.
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

		/// \brief Set active rasterizer state
		virtual void set_rasterizer_state(RasterizerState *state) = 0;

		/// \brief Set active blend state
		virtual void set_blend_state(BlendState *state, const Colorf &blend_color, unsigned int sample_mask) = 0;

		/// \brief Set active depth stencil state
		virtual void set_depth_stencil_state(DepthStencilState *state, int stencil_ref) = 0;

		/// \brief Set active program object to the standard program specified.
		virtual void set_program_object(StandardProgram standard_program) = 0;

		/// \brief Set active program object.
		///
		/// \param program = Program to set
		virtual void set_program_object(const ProgramObjectPtr &program) = 0;

		/// \brief Remove active program object.
		virtual void reset_program_object() = 0;

		/// \brief Select uniform buffer into index
		virtual void set_uniform_buffer(int index, const UniformBufferPtr &buffer) = 0;

		/// \brief Remove uniform buffer from index
		virtual void reset_uniform_buffer(int index) = 0;

		/// \brief Select storage buffer into index
		virtual void set_storage_buffer(int index, const StorageBufferPtr &buffer) = 0;

		/// \brief Remove storage buffer from index
		virtual void reset_storage_buffer(int index) = 0;

		/// \brief Select texture into unit.
		virtual void set_texture(int unit_index, const Texture &texture) = 0;

		/// \brief Remove texture from unit.
		virtual void reset_texture(int unit_index) = 0;

		/// \brief Select texture into image unit.
		virtual void set_image_texture(int unit_index, const Texture &texture) = 0;

		/// \brief Remove texture from image unit.
		virtual void reset_image_texture(int unit_index) = 0;

		/// \brief Returns true if this frame buffer object is owned by this graphic context.
		virtual bool is_frame_buffer_owner(const FrameBufferPtr &fb) = 0;

		/// \brief Set a frame buffer for off-screen rendering.
		virtual void set_frame_buffer(const FrameBufferPtr &write_buffer, const FrameBufferPtr &read_buffer) = 0;

		/// \brief Set the rendering buffer back to the screen.
		virtual void reset_frame_buffer() = 0;

		virtual void set_draw_buffer(DrawBuffer buffer) = 0;

		/// \brief Returns true if this primitives_array is owned by this graphic context.
		virtual bool is_primitives_array_owner(const PrimitivesArrayPtr &primitives_array) = 0;

		/// \brief Draw primitives on gc.
		virtual void draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArrayPtr &primitives_array) = 0;

		/// \brief Set the primitives array on the gc.
		virtual void set_primitives_array(const PrimitivesArrayPtr &primitives_array) = 0;

		/// \brief Draws primitives from the current assigned primitives array.
		virtual void draw_primitives_array(PrimitivesType type, int offset, int num_vertices) = 0;

		/// \brief Draws instanced primitives from the current assigned primitives array.
		virtual void draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count) = 0;

		/// \brief Sets current elements array buffer
		virtual void set_primitives_elements(ElementArrayBuffer *array_provider) = 0;

		/// \brief Draw primitives elements
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		virtual void draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset = 0) = 0;

		/// \brief Draw primitives elements instanced
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		/// \param instance_count = number of instances drawn
		virtual void draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count) = 0;

		/// \brief Resets current elements array buffer
		virtual void reset_primitives_elements() = 0;

		/// \brief Draw primitives elements
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param array_provider = Element Array Buffer Provider
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		virtual void draw_primitives_elements(PrimitivesType type, int count, ElementArrayBuffer *array_provider, VertexAttributeDataType indices_type, void *offset) = 0;

		/// \brief Draw primitives elements instanced
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param array_provider = Element Array Buffer Provider
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		virtual void draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayBuffer *array_provider, VertexAttributeDataType indices_type, void *offset, int instance_count) = 0;

		/// \brief Reset the primitives arrays.
		virtual void reset_primitives_array() = 0;

		/// \brief Set scissor.
		virtual void set_scissor(const Rect &rect) = 0;

		/// \brief Reset scissor.
		virtual void reset_scissor() = 0;

		/// \brief Execute a compute shader.
		virtual void dispatch(int x, int y, int z) = 0;

		/// \brief Clears the whole context using the specified color.
		virtual void clear(const Colorf &color) = 0;

		/// \brief Clear the depth buffer
		///
		/// \param value: value to clear to. Range 0.0 - 1.0.
		virtual void clear_depth(float value) = 0;

		/// \brief Clear the stencil buffer
		///
		/// \param value value to clear to.
		virtual void clear_stencil(int value) = 0;

		/// \brief Set the viewport to be used in user projection map mode.
		virtual void set_viewport(const Rectf &viewport) = 0;

		/// \brief Set the specified viewport to be used in user projection map mode.
		virtual void set_viewport(int index, const Rectf &viewport) = 0;

		/// \brief Specifies the depth range for all viewports
		virtual void set_depth_range(float n, float f) = 0;

		/// \brief Specifies the depth range for the specified viewport
		virtual void set_depth_range(int viewport, float n, float f) = 0;

		virtual void flush() = 0;
	};
}
