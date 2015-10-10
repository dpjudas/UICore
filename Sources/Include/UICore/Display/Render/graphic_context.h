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
*/

#pragma once

#include "../2D/color.h"
#include "../Image/texture_format.h"
#include <memory>
#include "../../Core/Math/mat4.h"
#include "../../Core/Math/rect.h"
#include "../../Core/Signals/signal.h"
#include "primitives_array.h"
#include "frame_buffer.h"
#include "element_array_vector.h"
#include "shader_object.h"
#include "program_object.h"

namespace uicore
{
	class Size;
	class Texture;
	class FrameBuffer;
	class PixelBuffer;
	class PrimitivesArray;
	class Font;
	class FontMetrics;
	class GraphicContextProvider;
	class GraphicContext_Impl;
	class ProgramObject;
	class Angle;
	class FontProvider_Freetype;
	class UniformBuffer;
	class StorageBuffer;
	class RasterizerStateDescription;
	class BlendStateDescription;
	class DepthStencilStateDescription;
	typedef std::shared_ptr<ProgramObject> ProgramObjectPtr;
	typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;
	typedef std::shared_ptr<StorageBuffer> StorageBufferPtr;
	typedef std::shared_ptr<UniformBuffer> UniformBufferPtr;
	typedef std::shared_ptr<PrimitivesArray> PrimitivesArrayPtr;
	typedef std::shared_ptr<Texture> TexturePtr;
	typedef std::shared_ptr<PixelBuffer> PixelBufferPtr;

	/// Polygon culling modes.
	enum CullMode
	{
		cull_front,
		cull_back,
		cull_front_and_back
	};

	/// Polygon filling modes.
	enum FillMode
	{
		fill_point,
		fill_line,
		fill_polygon
	};

	/// Front face modes.
	enum FaceSide
	{
		face_clockwise,
		face_counter_clockwise
	};

	/// Compare functions.
	enum CompareFunction
	{
		compare_lequal,
		compare_gequal,
		compare_less,
		compare_greater,
		compare_equal,
		compare_notequal,
		compare_always,
		compare_never
	};

	/// Stencil operations
	enum StencilOp
	{
		stencil_keep,
		stencil_zero,
		stencil_replace,
		stencil_incr,
		stencil_decr,
		stencil_invert,
		stencil_incr_wrap,
		stencil_decr_wrap
	};

	/// Drawing buffers.
	enum DrawBuffer
	{
		buffer_none,
		buffer_front_left,
		buffer_front_right,
		buffer_back_left,
		buffer_back_right,
		buffer_front,
		buffer_back,
		buffer_left,
		buffer_right,
		buffer_front_and_back
	};

	/// Logic Op
	enum LogicOp
	{
		logic_clear,
		logic_and,
		logic_and_reverse,
		logic_copy,
		logic_and_inverted,
		logic_noop,
		logic_xor,
		logic_or,
		logic_nor,
		logic_equiv,
		logic_invert,
		logic_or_reverse,
		logic_copy_inverted,
		logic_or_inverted,
		logic_nand,
		logic_set
	};

	/// Blending functions.
	enum BlendFunc
	{
		/// source or destination (0, 0, 0, 0)
		blend_zero,

		/// source or destination (1, 1, 1, 1)
		blend_one,

		/// source (Rd, Gd, Bd, Ad)
		blend_dest_color,

		/// destination (Rs, Gs, Bs, As)
		blend_src_color,

		/// source (1, 1, 1, 1) - (Rd, Gd, Bd, Ad)
		blend_one_minus_dest_color,

		/// destination (1, 1, 1, 1) - (Rs, Gs, Bs, As)
		blend_one_minus_src_color,

		/// source or destination (As, As, As, As)
		blend_src_alpha,

		/// source or destination (1, 1, 1, 1) - (As, As, As, As)
		blend_one_minus_src_alpha,

		/// source or destination (Ad, Ad, Ad, Ad)
		blend_dest_alpha,

		/// source or destination (1, 1, 1, 1) - (Ad, Ad, Ad, Ad)
		blend_one_minus_dest_alpha,

		/// source (f, f, f, 1) - f = min(As, 1 - Ad)
		blend_src_alpha_saturate,

		/// source or destination (Rc, Gc, Bc, Ac)
		blend_constant_color,

		/// source or destination (1, 1, 1, 1) - (Rc, Gc, Bc, Ac)
		blend_one_minus_constant_color,

		/// source or destination (Ac, Ac, Ac, Ac)
		blend_constant_alpha,

		/// source or destination (1, 1, 1, 1) - (Ac, Ac, Ac, Ac)
		blend_one_minus_constant_alpha
	};

	/// Blending equations.
	enum BlendEquation
	{
		equation_add,
		equation_subtract,
		equation_reverse_subtract,
		equation_min,
		equation_max
	};

	/// Point Sprite Origin
	enum PointSpriteOrigin
	{
		origin_upper_left,
		origin_lower_left
	};

	/// Primitive types.
	enum PrimitivesType
	{
		type_points,
		type_line_strip,
		type_line_loop,
		type_lines,
		type_triangle_strip,
		type_triangle_fan,
		type_triangles
	};

	/// Y axis direction for viewports, clipping rects, textures and render targets
	enum TextureImageYAxis
	{
		y_axis_bottom_up,  //!< OpenGL, origin is lower left with Y going upwards
		y_axis_top_down    //!< Direct3D, origin is upper left with Y going downwards
	};

	/// Standard Program
	enum StandardProgram
	{
		program_color_only,
		program_single_texture,
		program_sprite,
		program_path
	};

	/// Shader language used
	enum ShaderLanguage
	{
		shader_glsl,
		shader_hlsl,
		shader_fixed_function,
		num_shader_languages
	};

	class RasterizerState { };
	typedef std::shared_ptr<RasterizerState> RasterizerStatePtr;

	class BlendState { };
	typedef std::shared_ptr<BlendState> BlendStatePtr;

	class DepthStencilState { };
	typedef std::shared_ptr<DepthStencilState> DepthStencilStatePtr;

	/// Interface to drawing graphics.
	class GraphicContext
	{
	public:
		/// Returns in what range clip space z values are clipped.
		virtual ClipZRange clip_z_range() const = 0;

		/// Returns the Y axis direction for viewports, clipping rects, textures and render targets
		virtual TextureImageYAxis texture_image_y_axis() const = 0;

		/// Returns the shader language used
		virtual ShaderLanguage shader_language() const = 0;

		/** Returns the major version / feature level supported by the hardware.
		 *  For an OpenGL target, this returns the major OpenGL version the driver supports.
		 *  For a Direct3D target, this returns the major feature level.
		 */
		virtual int major_version() const = 0;

		/** Returns the minor version / feature level supported by the hardware.
		 *  For an OpenGL target, this returns the minor OpenGL version the driver supports.
		 *  For a Direct3D target, this returns the minor feature level.
		 */
		virtual int minor_version() const = 0;

		/** Returns `true` if the hardware supports compute shaders.
		 *  This function will always returns true for OpenGL 4.3 or newer, or
		 *  Direct3D 11.0 or newer. For Direct3D 10.0 and 10.1, the support for
		 *  compute shaders is optional.
		 */
		virtual bool has_compute_shader_support() const = 0;

		/** Returns the currently selected write frame buffer.
		 *  \return The frame buffer.
		 */
		virtual FrameBufferPtr write_frame_buffer() const = 0;

		/// Returns the currently selected read frame buffer.
		///
		/// \return The frame buffer.
		virtual FrameBufferPtr read_frame_buffer() const = 0;

		/// Returns the currently selected program object
		virtual ProgramObjectPtr program_object() const = 0;

		/// Returns the current actual width of the context.
		int width() const { return size().width; }

		/// Returns the current actual height of the context.
		int height() const { return size().height; }

		/// Returns the current actual size of the context.
		virtual Size size() const = 0;

		/// Retrieves the display pixel ratio of the context.
		/// \seealso Resolution Independence
		virtual float pixel_ratio() const = 0;

		/// Calculates the device independent width of the context.
		/// \seealso Resolution Independence
		float dip_width() const { return width() / pixel_ratio(); }

		/// Calculates the device independent height of the context.
		/// \seealso Resolution Independence
		float dip_height() const { return height() / pixel_ratio(); }

		/// Calculates the device independent dimensions of the context.
		/// \seealso Resolution Independence
		Sizef dip_size() const { return Sizef{ dip_width(), dip_height() }; }

		/** Retrieves the maximum size for a texture that this graphic context will
		 *  allow. Size(0, 0) will be returned if there is no known limitation to
		 *  the maximum texture size allowed for the context.
		 */
		virtual Size max_texture_size() const = 0;

		/// Return the content of the read buffer into a pixel buffer.
		virtual PixelBufferPtr pixeldata(const Rect& rect, TextureFormat texture_format = tf_rgba8, bool clamp = true) const = 0;

		/// Return the content of the read buffer into a pixel buffer.
		PixelBufferPtr pixeldata(TextureFormat texture_format = tf_rgba8, bool clamp = true) { return pixeldata(Rect(Point(), size()), texture_format, clamp); }

		/** Returns `true` if this frame buffer object is owned by this graphic
		 *  context.
		 *
		 *  Frame buffer objects cannot be shared between graphic contexts. This
		 *  function will verify if the frame buffer object belongs to this graphic
		 *  context.
		 */
		virtual bool is_frame_buffer_owner(const FrameBufferPtr &fb) = 0;

		/// \brief Create rasterizer state object.
		virtual std::shared_ptr<RasterizerState> create_rasterizer_state(const RasterizerStateDescription &desc) = 0;

		/// \brief Create blend state object.
		virtual std::shared_ptr<BlendState> create_blend_state(const BlendStateDescription &desc) = 0;

		/// \brief Create depth-stencil state object.
		virtual std::shared_ptr<DepthStencilState> create_depth_stencil_state(const DepthStencilStateDescription &desc) = 0;

		/// Sets the current frame buffer.
		void set_frame_buffer(const FrameBufferPtr &write_buffer) { set_frame_buffer(write_buffer, write_buffer); }
		virtual void set_frame_buffer(const FrameBufferPtr &write_buffer, const FrameBufferPtr &read_buffer) = 0;

		/// Resets the current frame buffer to be the initial frame buffer.
		void reset_frame_buffer() { set_frame_buffer(nullptr); }

		/// Select uniform buffer into index
		virtual void set_uniform_buffer(int index, const UniformBufferPtr &buffer) = 0;

		/// Remove uniform buffer from index
		void reset_uniform_buffer(int index) { set_uniform_buffer(index, nullptr); }

		/// Select storage buffer into index
		virtual void set_storage_buffer(int index, const StorageBufferPtr &buffer) = 0;

		/// Remove storage buffer from index
		void reset_storage_buffer(int index) { set_storage_buffer(index, nullptr); }

		/// Select texture into index.
		///
		/// \param unit_index = 0 to x, the index of this texture
		/// \param texture = The texture to select. This can be a null texture to clear that unit
		virtual void set_texture(int unit_index, const TexturePtr &texture) = 0;

		/// Select textures
		///
		/// \param start_index = First unit to select the textures into
		/// \param textures = The textures to select. These may contain null textures
		void set_textures(const std::vector<TexturePtr> &textures) { set_textures(0, textures); }
		void set_textures(int start_index, const std::vector<TexturePtr> &textures) { for (int i = 0; i < (int)textures.size(); i++) set_texture(start_index + i, textures[i]); }

		/// Remove texture from index.
		///
		/// \param unit_index = 0 to x, the index of the texture
		void reset_texture(int unit_index) { set_texture(unit_index, nullptr); }

		/// Remove all selected textures
		void reset_textures(int start_index, size_t count) { for (int i = 0; i < (int)count; i++) set_texture(start_index + i, nullptr); }

		/// Select texture image into index.
		///
		/// \param unit_index = 0 to x, the index of this texture
		/// \param texture = The texture to select. This can be a null texture to clear that unit
		virtual void set_image_texture(int unit_index, const TexturePtr &texture) = 0;

		/// Select texture images
		///
		/// Only textures units from 0 to textures.size()-1 are set.
		///
		/// \param textures = The texture to select. These may contain null textures
		/// \param start_index = First unit to select the textures into
		void set_image_textures(const std::vector<TexturePtr> &textures) { set_image_textures(0, textures); }
		void set_image_textures(int start_index, const std::vector<TexturePtr> &textures) { for (int i = 0; i < (int)textures.size(); i++) set_image_texture(start_index + i, textures[i]); }

		/// Remove texture from index.
		///
		/// \param unit_index = 0 to x, the index of the texture
		void reset_image_texture(int unit_index) { set_image_texture(unit_index, nullptr); }

		/// Remove all selected textures
		void reset_image_textures(int start_index, size_t count) { for (int i = 0; i < (int)count; i++) set_image_texture(start_index + i, nullptr); }

		/// Set active rasterizer state
		virtual void set_rasterizer_state(const RasterizerStatePtr &state) = 0;

		/// Set active blend state
		virtual void set_blend_state(const BlendStatePtr &state, const Colorf &blend_color = Colorf::white, unsigned int sample_mask = 0xffffffff) = 0;

		/// Set active depth stencil state
		virtual void set_depth_stencil_state(const DepthStencilStatePtr &state, int stencil_ref = 0) = 0;

		/// Set active rasterizer state
		void reset_rasterizer_state() { set_rasterizer_state(nullptr); }

		/// Set active blend state
		void reset_blend_state() { set_blend_state(nullptr); }

		/// Set active depth stencil state
		void reset_depth_stencil_state() { set_depth_stencil_state(nullptr); }

		/// Set active program object to the standard program specified.
		virtual void set_program_object(StandardProgram standard_program) = 0;

		/// Set active program object.
		virtual void set_program_object(const ProgramObjectPtr &program) = 0;

		/// Remove active program object.
		void reset_program_object() { set_program_object(nullptr); }

		/// Returns true if this primitives array is owned by this graphic context.
		///
		/// Primitive array objects cannot be shared between graphic contexts.  This function verifies that the primitives array
		/// belongs to this graphic context.
		virtual bool is_primitives_array_owner(const PrimitivesArrayPtr &primitives_array) = 0;

		/// Draw primitives on gc
		virtual void draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArrayPtr &array) = 0;

		/// Set the primitives array on the gc
		virtual void set_primitives_array(const PrimitivesArrayPtr &array) = 0;

		/// Draws primitives from the current assigned primitives array.
		void draw_primitives_array(PrimitivesType type, int num_vertices) { draw_primitives_array(type, 0, num_vertices); }

		/// Draw primitives array
		///
		/// \param type = Primitives Type
		/// \param offset = value
		/// \param num_vertices = value
		virtual void draw_primitives_array(PrimitivesType type, int offset, int num_vertices) = 0;

		/// Draw primitives array instanced
		///
		/// \param type = Primitives Type
		/// \param offset = value
		/// \param num_vertices = value
		/// \param instance_count = number of instances drawn
		virtual void draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count) = 0;

		/// Sets current elements array buffer
		virtual void set_primitives_elements(const ElementArrayBufferPtr &element_array) = 0;

		/// Draw primitives elements
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		virtual void draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset = 0) = 0;

		/// Draw primitives elements instanced
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		/// \param instance_count = number of instances drawn
		virtual void draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count) = 0;

		/// Resets current elements array buffer
		void reset_primitives_elements() { set_primitives_elements(nullptr); }

		/// Draw primitives elements
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param element_array = Element Array Buffer
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		virtual void draw_primitives_elements(PrimitivesType type, int count, const ElementArrayBufferPtr &element_array, VertexAttributeDataType indices_type, size_t offset = 0) = 0;

		/// Draw primitives elements instanced
		///
		/// \param type = Primitives Type
		/// \param count = value
		/// \param element_array = Element Array Buffer
		/// \param indices_type = Vertex Attribute Data Type
		/// \param offset = void
		/// \param instance_count = number of instances drawn
		virtual void draw_primitives_elements_instanced(PrimitivesType type, int count, const ElementArrayBufferPtr &element_array, VertexAttributeDataType indices_type, size_t offset, int instance_count) = 0;

		/// Reset the primitives arrays.
		void reset_primitives_array() { set_primitives_array(nullptr); }

		/// Execute a compute shader.
		virtual void dispatch(int x = 1, int y = 1, int z = 1) = 0;

		/// Clears the whole context using the specified color.
		virtual void clear(const Colorf &color = Colorf::black) = 0;

		/// Clear the stencil buffer
		///
		/// \param value value to clear to.
		virtual void clear_stencil(int value = 0) = 0;

		/// Clear the depth buffer
		///
		/// \param value: value to clear to. Range 0.0 - 1.0.
		virtual void clear_depth(float value = 0) = 0;

		/// Set the current clipping rectangle.
		virtual void set_scissor(const Rect &rect, TextureImageYAxis y_axis) = 0;

		/// Removes the set clipping rectangle
		virtual void reset_scissor() = 0;

		/// Set the viewport to be used in user projection map mode.
		///
		/// \param viewport = The viewport to set
		virtual void set_viewport(const Rectf &viewport, TextureImageYAxis y_axis) = 0;

		/// Set the specified viewport to be used in user projection map mode.
		///
		/// \param index = The viewport index (0 to x)
		/// \param viewport = The viewport to set
		virtual void set_viewport(int index, const Rectf &viewport, TextureImageYAxis y_axis) = 0;

		/// Specifies the depth range for all viewports
		void set_depth_range(float n, float f) { set_depth_range(-1, n, f); }

		/// Specifies the depth range for the specified viewport
		virtual void set_depth_range(int viewport, float n, float f) = 0;

		/// Set used draw buffer.
		virtual void set_draw_buffer(DrawBuffer buffer) = 0;

		/// Flush the command buffer
		virtual void flush() = 0;
	};

	typedef std::shared_ptr<GraphicContext> GraphicContextPtr;
}
