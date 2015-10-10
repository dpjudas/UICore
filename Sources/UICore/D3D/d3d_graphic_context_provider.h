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

#include "UICore/Display/Render/graphic_context_provider.h"
#include "UICore/Display/Render/blend_state_description.h"
#include "UICore/Display/Render/depth_stencil_state_description.h"
#include "UICore/Display/Render/graphic_context.h"
#include "UICore/Display/Render/program_object.h"
#include "d3d_rasterizer_state_provider.h"
#include "d3d_blend_state_provider.h"
#include "d3d_depth_stencil_state_provider.h"
#include "standard_programs.h"
#include "d3d_unit_map.h"
#include <map>
#include <memory>
#include "d3d_render_buffer_provider.h"

namespace uicore
{
	class D3DDisplayWindowProvider;
	class D3DProgramObjectProvider;
	class D3DPrimitivesArrayProvider;
	class D3DElementArrayBufferProvider;
	class DisplayWindowDescription;

	class D3DGraphicContextProvider : public GraphicContextProvider
	{
	public:
		D3DGraphicContextProvider(D3DDisplayWindowProvider *window, const DisplayWindowDescription &display_desc);
		~D3DGraphicContextProvider();

		int max_attributes() override;
		Size max_texture_size() const override;
		Size display_window_size() const override;
		float pixel_ratio() const override;

		D3DDisplayWindowProvider *get_window() const { return window; }

		Signal<void(const Size &)> &sig_window_resized() override { return window_resized_signal; }

		StandardPrograms standard_programs;

		ClipZRange clip_z_range() const override { return clip_zero_positive_w; }
		TextureImageYAxis texture_image_y_axis() const override { return y_axis_top_down; }

		ShaderLanguage shader_language() const override { return shader_hlsl; }
		int major_version() const override;
		int minor_version() const override;
		bool has_compute_shader_support() const override;
		std::shared_ptr<PixelBuffer> pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const override;

		std::shared_ptr<RasterizerState> create_rasterizer_state(const RasterizerStateDescription &desc) override;
		std::shared_ptr<BlendState> create_blend_state(const BlendStateDescription &desc) override;
		std::shared_ptr<DepthStencilState> create_depth_stencil_state(const DepthStencilStateDescription &desc) override;
		std::shared_ptr<ProgramObject> create_program() override;
		std::shared_ptr<ShaderObject> create_shader(ShaderType type, const std::string &source) override;
		std::shared_ptr<ShaderObject> create_shader(ShaderType type, const void *bytecode, int bytecode_size) override;
		std::shared_ptr<OcclusionQuery> create_occlusion_query() override;
		std::shared_ptr<FrameBuffer> create_frame_buffer() override;
		std::shared_ptr<RenderBuffer> create_render_buffer(int width, int height, TextureFormat texture_format, int multisample_samples) override;
		std::shared_ptr<StorageBuffer> create_storage_buffer(int size, int stride, BufferUsage usage) override;
		std::shared_ptr<StorageBuffer> create_storage_buffer(const void *data, int size, int stride, BufferUsage usage) override;
		std::shared_ptr<ElementArrayBuffer> create_element_array_buffer(int size, BufferUsage usage) override;
		std::shared_ptr<ElementArrayBuffer> create_element_array_buffer(const void *data, int size, BufferUsage usage) override;
		std::shared_ptr<VertexArrayBuffer> create_vertex_array_buffer(int size, BufferUsage usage) override;
		std::shared_ptr<VertexArrayBuffer> create_vertex_array_buffer(const void *data, int size, BufferUsage usage) override;
		std::shared_ptr<UniformBuffer> create_uniform_buffer(int size, BufferUsage usage) override;
		std::shared_ptr<UniformBuffer> create_uniform_buffer(const void *data, int size, BufferUsage usage) override;
		std::shared_ptr<TransferBuffer> create_transfer_buffer(int size, BufferUsage usage) override;
		std::shared_ptr<TransferBuffer> create_transfer_buffer(const void *data, int size, BufferUsage usage) override;
		std::shared_ptr<PrimitivesArray> create_primitives_array() override;
		std::shared_ptr<Texture1D> create_texture_1d(int width, TextureFormat texture_format, int levels) override;
		std::shared_ptr<Texture1DArray> create_texture_1d_array(int width, int array_size, TextureFormat texture_format, int levels) override;
		std::shared_ptr<Texture2D> create_texture_2d(int width, int height, TextureFormat texture_format, int levels) override;
		std::shared_ptr<Texture2DArray> create_texture_2d_array(int width, int height, int array_size, TextureFormat texture_format, int levels) override;
		std::shared_ptr<Texture3D> create_texture_3d(int width, int height, int depth, TextureFormat texture_format, int levels) override;
		std::shared_ptr<TextureCube> create_texture_cube(int width, int height, TextureFormat texture_format, int levels) override;
		std::shared_ptr<TextureCubeArray> create_texture_cube_array(int width, int height, int array_size, TextureFormat texture_format, int levels) override;
		std::shared_ptr<TransferTexture> create_transfer_texture(const void *data, const Size &size, PixelBufferDirection direction, TextureFormat new_format, BufferUsage usage) override;

		FrameBufferPtr write_frame_buffer() const { return _write_frame_buffer; }
		FrameBufferPtr read_frame_buffer() const { return _read_frame_buffer; }
		ProgramObjectPtr program_object() const override { return _program_object; }

		void set_rasterizer_state(const RasterizerStatePtr &state) override;
		void set_blend_state(const BlendStatePtr &state, const Colorf &blend_color, unsigned int sample_mask) override;
		void set_depth_stencil_state(const DepthStencilStatePtr &state, int stencil_ref) override;
		void set_program_object(StandardProgram standard_program) override;
		void set_program_object(const ProgramObjectPtr &program) override;
		void set_uniform_buffer(int index, const UniformBufferPtr &buffer) override;
		void set_storage_buffer(int index, const StorageBufferPtr &buffer) override;
		void set_texture(int unit_index, const TexturePtr &texture) override;
		void set_image_texture(int unit_index, const TexturePtr &texture) override;
		bool is_frame_buffer_owner(const FrameBufferPtr &fb) override;
		void set_frame_buffer(const FrameBufferPtr &write_buffer, const FrameBufferPtr &read_buffer) override;
		void set_draw_buffer(DrawBuffer buffer) override;

		bool is_primitives_array_owner(const PrimitivesArrayPtr &primitives_array) override;
		void draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArrayPtr &primitives_array) override;
		void set_primitives_array(const PrimitivesArrayPtr &primitives_array) override;
		void draw_primitives_array(PrimitivesType type, int offset, int num_vertices) override;
		void draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count) override;
		void set_primitives_elements(const ElementArrayBufferPtr &array_provider) override;
		void draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset = 0) override;
		void draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count) override;
		void draw_primitives_elements(PrimitivesType type, int count, const ElementArrayBufferPtr &array_provider, VertexAttributeDataType indices_type, size_t offset) override;
		void draw_primitives_elements_instanced(PrimitivesType type, int count, const ElementArrayBufferPtr &array_provider, VertexAttributeDataType indices_type, size_t offset, int instance_count) override;
		void set_scissor(const Rect &rect) override;
		void reset_scissor() override;
		void dispatch(int x, int y, int z) override;
		void clear(const Colorf &color) override;
		void clear_depth(float value) override;
		void clear_stencil(int value) override;
		void set_viewport(const Rectf &viewport) override;
		void set_viewport(int index, const Rectf &viewport) override;

		void set_depth_range(int viewport, float n, float f) override;

		void on_window_resized() override;

		void begin_resize_swap_chain();
		void end_resize_swap_chain();

		void flush() override;

	private:
		void set_default_dsv();
		static D3D11_PRIMITIVE_TOPOLOGY to_d3d_primitive_topology(PrimitivesType type);
		static DXGI_FORMAT to_d3d_format(VertexAttributeDataType indices_type);
		static UINT to_d3d_index_location(VertexAttributeDataType indices_type, size_t offset);
		void apply_input_layout();
		void clear_input_layout();

		D3DDisplayWindowProvider *window;
		D3DPrimitivesArrayProvider *current_prim_array_provider;
		D3DProgramObjectProvider *current_program_provider;
		D3DElementArrayBufferProvider *current_element_array_provider;
		D3D11_VIEWPORT viewports[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		D3D11_RECT scissor_rects[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		bool input_layout_set;
		D3DUnitMap unit_map;
		bool shader_bound[(int)ShaderType::num_types];

		Signal<void(const Size &)> window_resized_signal;

		std::map<RasterizerStateDescription, std::shared_ptr<RasterizerState> > rasterizer_states;
		std::map<BlendStateDescription, std::shared_ptr<BlendState> > blend_states;
		std::map<DepthStencilStateDescription, std::shared_ptr<DepthStencilState> > depth_stencil_states;

		std::shared_ptr<D3DRenderBufferProvider> default_depth_render_buffer;
		ComPtr<ID3D11DepthStencilView> default_dsv;
		int default_depth;

		FrameBufferPtr _read_frame_buffer;
		FrameBufferPtr _write_frame_buffer;
		ProgramObjectPtr _program_object;
	};
}
