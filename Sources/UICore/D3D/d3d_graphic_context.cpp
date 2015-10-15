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
*/

#include "UICore/precomp.h"
#include "d3d_graphic_context.h"
#include "d3d_display_window.h"
#include "d3d_texture_object.h"
#include "d3d_element_array_buffer.h"
#include "d3d_staging_texture.h"
#include "d3d_frame_buffer.h"
#include "d3d_occlusion_query.h"
#include "d3d_program_object.h"
#include "d3d_render_buffer.h"
#include "d3d_shader_object.h"
#include "d3d_vertex_array_buffer.h"
#include "d3d_primitives_array.h"
#include "d3d_uniform_buffer.h"
#include "d3d_storage_buffer.h"
#include "d3d_element_array_buffer.h"
#include "d3d_staging_buffer.h"
#include "UICore/Display/Render/staging_texture.h"
#include "UICore/Display/Window/display_window_description.h"
#include "UICore/D3D/d3d_target.h"

namespace uicore
{
	D3DGraphicContext::D3DGraphicContext(D3DDisplayWindow *window, const DisplayWindowDescription &display_desc)
		: window(window),
		current_prim_array_provider(0),
		current_program_provider(0),
		input_layout_set(false)
	{
		default_depth = display_desc.depth_size();

		Size viewport_size = display_window_size();
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			viewports[i].Width = viewport_size.width;
			viewports[i].Height = viewport_size.height;
			viewports[i].TopLeftX = 0;
			viewports[i].TopLeftY = 0;
			viewports[i].MinDepth = 0.0f;
			viewports[i].MaxDepth = 1.0f;
		}
		window->get_device_context()->RSSetViewports(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, viewports);

		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			scissor_rects[i].left = 0;
			scissor_rects[i].top = 0;
			scissor_rects[i].right = 0;
			scissor_rects[i].bottom = 0;
		}

		for (auto &v : shader_bound) v = false;

		set_default_dsv();

		D3DShareList::context_created(this);

		set_default_state();
		reset_scissor();
	}

	D3DGraphicContext::~D3DGraphicContext()
	{
		D3DShareList::context_destroyed(this);
	}

	void D3DGraphicContext::begin_resize_swap_chain()
	{
		window->get_device_context()->OMSetRenderTargets(0, 0, 0);
		window->get_device_context()->Flush();

		default_dsv.clear();
		default_depth_render_buffer.reset();
	}

	void D3DGraphicContext::end_resize_swap_chain()
	{
		Size viewport_size = display_window_size();
		viewports[0].Width = viewport_size.width;
		viewports[0].Height = viewport_size.height;
		viewports[0].TopLeftX = 0;
		viewports[0].TopLeftY = 0;
		window->get_device_context()->RSSetViewports(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, viewports);

		set_default_dsv();
	}

	void D3DGraphicContext::flush()
	{
		window->get_device_context()->Flush();
	}

	int D3DGraphicContext::max_attributes()
	{
		return 16; // To do: this is the D3D10 limit - is it still the same for D3D11?
	}

	Size D3DGraphicContext::max_texture_size() const
	{
		switch (window->get_feature_level())
		{
		case D3D_FEATURE_LEVEL_9_1:
		case D3D_FEATURE_LEVEL_9_2: return Size(2048, 2048);
		case D3D_FEATURE_LEVEL_9_3: return Size(4096, 4096);
		case D3D_FEATURE_LEVEL_10_0:
		case D3D_FEATURE_LEVEL_10_1: return Size(8192, 8192);
		default:
		case D3D_FEATURE_LEVEL_11_0: return Size(16384, 16384);
		}
	}

	Size D3DGraphicContext::display_window_size() const
	{
		return window->backing_viewport().size();
	}

	float D3DGraphicContext::pixel_ratio() const
	{
		return window->pixel_ratio();
	}

	std::shared_ptr<PixelBuffer> D3DGraphicContext::pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const
	{
		// To do: fetch format from window->get_back_buffer()->GetDesc(&desc)
		// To do: window->get_back_buffer() is only correct when no frame buffer is bound
		auto pixels = std::make_shared<D3DStagingTexture>(window->get_device(), nullptr, rect.size(), StagingDirection::from_gpu, texture_format, usage_stream_copy);
		D3D11_BOX box;
		box.left = rect.left;
		box.top = rect.top;
		box.right = rect.right;
		box.bottom = rect.bottom;
		box.front = 0;
		box.back = 1;
		window->get_device_context()->CopySubresourceRegion(pixels->get_texture_2d(window->get_device()), 0, 0, 0, 0, window->get_back_buffer(), 0, &box);
		return pixels;
	}

	std::shared_ptr<RasterizerState> D3DGraphicContext::create_rasterizer_state(const RasterizerStateDescription &desc)
	{
		auto it = rasterizer_states.find(desc);
		if (it != rasterizer_states.end())
		{
			return it->second;
		}
		else
		{
			auto state = std::make_shared<D3DRasterizerState>(window->get_device(), desc);
			rasterizer_states[desc.clone()] = state;
			return state;
		}
	}

	std::shared_ptr<BlendState> D3DGraphicContext::create_blend_state(const BlendStateDescription &desc)
	{
		auto it = blend_states.find(desc);
		if (it != blend_states.end())
		{
			return it->second;
		}
		else
		{
			auto state = std::make_shared<D3DBlendState>(window->get_device(), desc);
			blend_states[desc.clone()] = state;
			return state;
		}
	}

	std::shared_ptr<DepthStencilState> D3DGraphicContext::create_depth_stencil_state(const DepthStencilStateDescription &desc)
	{
		auto it = depth_stencil_states.find(desc);
		if (it != depth_stencil_states.end())
		{
			return it->second;
		}
		else
		{
			auto state = std::make_shared<D3DDepthStencilState>(window->get_device(), desc);
			depth_stencil_states[desc.clone()] = state;
			return state;
		}
	}

	std::shared_ptr<ShaderObject> D3DGraphicContext::create_shader(ShaderType type, const std::string &source)
	{
		return std::make_shared<D3DShaderObject>(window->get_device(), window->get_feature_level(), type, source);
	}

	std::shared_ptr<ShaderObject> D3DGraphicContext::create_shader(ShaderType type, const void *bytecode, int bytecode_size)
	{
		return std::make_shared<D3DShaderObject>(window->get_device(), window->get_feature_level(), type, bytecode, bytecode_size);
	}

	std::shared_ptr<ProgramObject> D3DGraphicContext::create_program()
	{
		return std::make_shared<D3DProgramObject>(window->get_device(), window->get_device_context());
	}

	std::shared_ptr<OcclusionQuery> D3DGraphicContext::create_occlusion_query()
	{
		return std::make_shared<D3DOcclusionQuery>();
	}

	std::shared_ptr<FrameBuffer> D3DGraphicContext::create_frame_buffer()
	{
		return std::make_shared<D3DFrameBuffer>(window->get_device());
	}

	std::shared_ptr<RenderBuffer> D3DGraphicContext::create_render_buffer(int width, int height, TextureFormat texture_format, int multisample_samples)
	{
		return std::make_shared<D3DRenderBuffer>(window->get_device(), width, height, texture_format, multisample_samples);
	}

	std::shared_ptr<StorageBuffer> D3DGraphicContext::create_storage_buffer(int size, int stride, BufferUsage usage)
	{
		return std::make_shared<D3DStorageBuffer>(window->get_device(), size, stride, usage);
	}

	std::shared_ptr<StorageBuffer> D3DGraphicContext::create_storage_buffer(const void *data, int size, int stride, BufferUsage usage)
	{
		return std::make_shared<D3DStorageBuffer>(window->get_device(), data, size, stride, usage);
	}

	std::shared_ptr<ElementArrayBuffer> D3DGraphicContext::create_element_array_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<D3DElementArrayBuffer>(window->get_device(), size, usage);
	}

	std::shared_ptr<ElementArrayBuffer> D3DGraphicContext::create_element_array_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<D3DElementArrayBuffer>(window->get_device(), data, size, usage);
	}

	std::shared_ptr<VertexArrayBuffer> D3DGraphicContext::create_vertex_array_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<D3DVertexArrayBuffer>(window->get_device(), size, usage);
	}

	std::shared_ptr<VertexArrayBuffer> D3DGraphicContext::create_vertex_array_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<D3DVertexArrayBuffer>(window->get_device(), data, size, usage);
	}

	std::shared_ptr<UniformBuffer> D3DGraphicContext::create_uniform_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<D3DUniformBuffer>(window->get_device(), size, usage);
	}

	std::shared_ptr<UniformBuffer> D3DGraphicContext::create_uniform_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<D3DUniformBuffer>(window->get_device(), data, size, usage);
	}

	std::shared_ptr<StagingBuffer> D3DGraphicContext::create_staging_buffer(int size, BufferUsage usage)
	{
		return std::make_shared<D3DStagingBuffer>(window->get_device(), size, usage);
	}

	std::shared_ptr<StagingBuffer> D3DGraphicContext::create_staging_buffer(const void *data, int size, BufferUsage usage)
	{
		return std::make_shared<D3DStagingBuffer>(window->get_device(), data, size, usage);
	}

	std::shared_ptr<PrimitivesArray> D3DGraphicContext::create_primitives_array()
	{
		return std::make_shared<D3DPrimitivesArray>(window->get_device());
	}

	std::shared_ptr<Texture1D> D3DGraphicContext::create_texture_1d(int width, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture1DImpl<D3DTextureObject>>(D3DTextureObject::InitData(window->get_device(), window->get_feature_level()), width, texture_format, levels);
	}

	std::shared_ptr<Texture1DArray> D3DGraphicContext::create_texture_1d_array(int width, int array_size, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture1DArrayImpl<D3DTextureObject>>(D3DTextureObject::InitData(window->get_device(), window->get_feature_level()), width, array_size, texture_format, levels);
	}

	std::shared_ptr<Texture2D> D3DGraphicContext::create_texture_2d(int width, int height, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture2DImpl<D3DTextureObject>>(D3DTextureObject::InitData(window->get_device(), window->get_feature_level()), width, height, texture_format, levels);
	}

	std::shared_ptr<Texture2DArray> D3DGraphicContext::create_texture_2d_array(int width, int height, int array_size, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture2DArrayImpl<D3DTextureObject>>(D3DTextureObject::InitData(window->get_device(), window->get_feature_level()), width, height, array_size, texture_format, levels);
	}

	std::shared_ptr<Texture3D> D3DGraphicContext::create_texture_3d(int width, int height, int depth, TextureFormat texture_format, int levels)
	{
		return std::make_shared<Texture3DImpl<D3DTextureObject>>(D3DTextureObject::InitData(window->get_device(), window->get_feature_level()), width, height, depth, texture_format, levels);
	}

	std::shared_ptr<TextureCube> D3DGraphicContext::create_texture_cube(int width, int height, TextureFormat texture_format, int levels)
	{
		return std::make_shared<TextureCubeImpl<D3DTextureObject>>(D3DTextureObject::InitData(window->get_device(), window->get_feature_level()), width, height, texture_format, levels);
	}

	std::shared_ptr<TextureCubeArray> D3DGraphicContext::create_texture_cube_array(int width, int height, int array_size, TextureFormat texture_format, int levels)
	{
		return std::make_shared<TextureCubeArrayImpl<D3DTextureObject>>(D3DTextureObject::InitData(window->get_device(), window->get_feature_level()), width, height, array_size, texture_format, levels);
	}

	std::shared_ptr<StagingTexture> D3DGraphicContext::create_staging_texture(const void *data, const Size &size, StagingDirection direction, TextureFormat format, BufferUsage usage)
	{
		return std::make_shared<D3DStagingTexture>(window->get_device(), data, size, direction, format, usage);
	}

	void D3DGraphicContext::set_rasterizer_state(const RasterizerStatePtr &state)
	{
		if (state)
		{
			D3DRasterizerState *d3d_state = static_cast<D3DRasterizerState*>(state.get());
			window->get_device_context()->RSSetState(d3d_state->state);
		}
		else
		{
			set_rasterizer_state(default_rasterizer_state());
		}
	}

	void D3DGraphicContext::set_blend_state(const BlendStatePtr &state, const Colorf &blend_color, unsigned int sample_mask)
	{
		if (state)
		{
			D3DBlendState *d3d_state = static_cast<D3DBlendState*>(state.get());
			FLOAT blend_factor[4] = { blend_color.r, blend_color.g, blend_color.b, blend_color.a };
			window->get_device_context()->OMSetBlendState(d3d_state->state, blend_factor, sample_mask);
		}
		else
		{
			set_blend_state(default_blend_state(), blend_color, sample_mask);
		}
	}

	void D3DGraphicContext::set_depth_stencil_state(const DepthStencilStatePtr &state, int stencil_ref)
	{
		if (state)
		{
			D3DDepthStencilState *d3d_state = static_cast<D3DDepthStencilState*>(state.get());
			window->get_device_context()->OMSetDepthStencilState(d3d_state->state, stencil_ref);
		}
		else
		{
			set_depth_stencil_state(default_depth_stencil_state(), stencil_ref);
		}
	}

	void D3DGraphicContext::set_program_object(StandardProgram standard_program)
	{
		ProgramObjectPtr program = standard_programs.get_program_object(standard_program);
		set_program_object(program);
	}

	void D3DGraphicContext::set_program_object(const ProgramObjectPtr &program)
	{
		_program_object = program;

		if (program)
		{
			D3DProgramObject *new_program_provider = static_cast<D3DProgramObject *>(program.get());
			if (new_program_provider == current_program_provider)
				return;

			if (current_program_provider)
				unit_map.unbind_program(this, current_program_provider);

			current_program_provider = new_program_provider;

			clear_input_layout();

			for (int j = 0; j < (int)ShaderType::num_types; j++)
			{
				D3DShaderObject *shader_provider = current_program_provider->get_shader_provider((ShaderType)j);
				if (shader_provider)
				{
					switch ((ShaderType)j)
					{
					case ShaderType::vertex: window->get_device_context()->VSSetShader(shader_provider->get_vertex(), 0, 0); break;
					case ShaderType::tess_control: window->get_device_context()->HSSetShader(shader_provider->get_hull(), 0, 0); break;
					case ShaderType::tess_evaluation: window->get_device_context()->DSSetShader(shader_provider->get_domain(), 0, 0); break;
					case ShaderType::geometry: window->get_device_context()->GSSetShader(shader_provider->get_geometry(), 0, 0); break;
					case ShaderType::fragment: window->get_device_context()->PSSetShader(shader_provider->get_pixel(), 0, 0); break;
					case ShaderType::compute: window->get_device_context()->CSSetShader(shader_provider->get_compute(), 0, 0); break;
					}
					shader_bound[j] = true;
				}
				else if (shader_bound[j])
				{
					switch ((ShaderType)j)
					{
					case ShaderType::vertex: window->get_device_context()->VSSetShader(0, 0, 0); break;
					case ShaderType::tess_control: window->get_device_context()->HSSetShader(0, 0, 0); break;
					case ShaderType::tess_evaluation: window->get_device_context()->DSSetShader(0, 0, 0); break;
					case ShaderType::geometry: window->get_device_context()->GSSetShader(0, 0, 0); break;
					case ShaderType::fragment: window->get_device_context()->PSSetShader(0, 0, 0); break;
					case ShaderType::compute: window->get_device_context()->CSSetShader(0, 0, 0); break;
					}
					shader_bound[j] = false;
				}
			}

			unit_map.bind_program(this, current_program_provider);
		}
		else
		{
			if (current_program_provider)
			{
				unit_map.unbind_program(this, current_program_provider);
				current_program_provider = 0;
				for (int j = 0; j < (int)ShaderType::num_types; j++)
				{
					if (shader_bound[j])
					{
						switch ((ShaderType)j)
						{
						case ShaderType::vertex: window->get_device_context()->VSSetShader(0, 0, 0); break;
						case ShaderType::tess_control: window->get_device_context()->HSSetShader(0, 0, 0); break;
						case ShaderType::tess_evaluation: window->get_device_context()->DSSetShader(0, 0, 0); break;
						case ShaderType::geometry: window->get_device_context()->GSSetShader(0, 0, 0); break;
						case ShaderType::fragment: window->get_device_context()->PSSetShader(0, 0, 0); break;
						case ShaderType::compute: window->get_device_context()->CSSetShader(0, 0, 0); break;
						}
						shader_bound[j] = false;
					}
				}
				clear_input_layout();
			}
		}
	}

	void D3DGraphicContext::set_uniform_buffer(int index, const UniformBufferPtr &buffer)
	{
		unit_map.set_uniform_buffer(this, index, buffer);
	}

	void D3DGraphicContext::set_storage_buffer(int index, const StorageBufferPtr &buffer)
	{
		unit_map.set_storage_buffer(this, index, buffer);
	}

	void D3DGraphicContext::set_texture(int unit_index, const TexturePtr &texture)
	{
		unit_map.set_texture(this, unit_index, texture);
	}

	void D3DGraphicContext::set_image_texture(int unit_index, const TexturePtr &texture)
	{
		unit_map.set_image(this, unit_index, texture);
	}

	bool D3DGraphicContext::is_frame_buffer_owner(const FrameBufferPtr &fb)
	{
		D3DFrameBuffer *fb_provider = static_cast<D3DFrameBuffer *>(fb.get());
		if (fb_provider)
			return fb_provider->get_device() == window->get_device();
		else
			return false;
	}

	void D3DGraphicContext::set_frame_buffer(const FrameBufferPtr &write_buffer, const FrameBufferPtr &read_buffer)
	{
		_write_frame_buffer = write_buffer;
		_read_frame_buffer = read_buffer;

		if (write_buffer && read_buffer)
		{
			D3DFrameBuffer *fb_provider = static_cast<D3DFrameBuffer *>(write_buffer.get());
			ID3D11DepthStencilView *dsv = 0;
			std::vector<ID3D11RenderTargetView *> rtvs = fb_provider->get_views(dsv);
			window->get_device_context()->OMSetRenderTargets(rtvs.size(), (!rtvs.empty()) ? &rtvs[0] : 0, dsv);
		}
		else
		{
			set_default_dsv();
		}
	}

	void D3DGraphicContext::set_draw_buffer(DrawBuffer buffer)
	{
		// To do: what does this map to in D3D?
	}

	bool D3DGraphicContext::is_primitives_array_owner(const PrimitivesArrayPtr &primitives_array)
	{
		D3DPrimitivesArray *array_provider = static_cast<D3DPrimitivesArray *>(primitives_array.get());
		if (array_provider)
			return array_provider->get_device() == window->get_device();
		else
			return false;
	}

	void D3DGraphicContext::apply_input_layout()
	{
		if (!input_layout_set)
		{
			window->get_device_context()->IASetInputLayout(current_prim_array_provider->get_input_layout(current_program_provider));
			input_layout_set = true;
		}
	}

	void D3DGraphicContext::clear_input_layout()
	{
		if (input_layout_set)
		{
			input_layout_set = false;
			window->get_device_context()->IASetInputLayout(0);
		}
	}

	void D3DGraphicContext::draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArrayPtr &primitives_array)
	{
		set_primitives_array(primitives_array);
		draw_primitives_array(type, 0, num_vertices);
		reset_primitives_array();
	}

	void D3DGraphicContext::set_primitives_array(const PrimitivesArrayPtr &primitives_array)
	{
		clear_input_layout();
		if (current_prim_array_provider)
		{
			std::vector<ID3D11Buffer*> buffers;
			std::vector<UINT> strides, offsets;
			size_t range = current_prim_array_provider->get_vertex_buffers_range();
			buffers.resize(range);
			strides.resize(range);
			offsets.resize(range);
			if (range != 0)
				window->get_device_context()->IASetVertexBuffers(0, buffers.size(), &buffers[0], &strides[0], &offsets[0]);
		}

		current_prim_array_provider = 0;

		if (primitives_array)
		{
			current_prim_array_provider = static_cast<D3DPrimitivesArray *>(primitives_array.get());
			std::vector<ID3D11Buffer*> buffers;
			std::vector<UINT> strides, offsets;
			current_prim_array_provider->get_vertex_buffers(buffers, strides, offsets);
			if (!buffers.empty())
				window->get_device_context()->IASetVertexBuffers(0, buffers.size(), &buffers[0], &strides[0], &offsets[0]);
		}
	}

	void D3DGraphicContext::draw_primitives_array(PrimitivesType type, int offset, int num_vertices)
	{
		apply_input_layout();
		window->get_device_context()->IASetPrimitiveTopology(to_d3d_primitive_topology(type));
		window->validate_context();
		window->get_device_context()->Draw(num_vertices, offset);
	}

	void D3DGraphicContext::draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count)
	{
		apply_input_layout();
		window->get_device_context()->IASetPrimitiveTopology(to_d3d_primitive_topology(type));
		window->validate_context();
		window->get_device_context()->DrawInstanced(num_vertices, instance_count, offset, 0);
	}

	void D3DGraphicContext::set_primitives_elements(const ElementArrayBufferPtr &array_provider)
	{
		if (array_provider)
		{
			current_element_array_provider = static_cast<D3DElementArrayBuffer*>(array_provider.get());
		}
		else
		{
			window->get_device_context()->IASetIndexBuffer(0, DXGI_FORMAT_UNKNOWN, 0);
			current_element_array_provider = 0;
		}
	}

	void D3DGraphicContext::draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset)
	{
		apply_input_layout();
		window->get_device_context()->IASetPrimitiveTopology(to_d3d_primitive_topology(type));
		window->get_device_context()->IASetIndexBuffer(current_element_array_provider->get_buffer(window->get_device()), to_d3d_format(indices_type), 0);
		window->validate_context();
		window->get_device_context()->DrawIndexed(count, to_d3d_index_location(indices_type, offset), 0);
	}

	void D3DGraphicContext::draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count)
	{
		apply_input_layout();
		window->get_device_context()->IASetPrimitiveTopology(to_d3d_primitive_topology(type));
		window->get_device_context()->IASetIndexBuffer(current_element_array_provider->get_buffer(window->get_device()), to_d3d_format(indices_type), 0);
		window->validate_context();
		window->get_device_context()->DrawIndexedInstanced(count, instance_count, to_d3d_index_location(indices_type, offset), 0, 0);
	}

	void D3DGraphicContext::draw_primitives_elements(PrimitivesType type, int count, const ElementArrayBufferPtr &array_provider, VertexAttributeDataType indices_type, size_t offset)
	{
		set_primitives_elements(array_provider);
		apply_input_layout();
		window->get_device_context()->IASetPrimitiveTopology(to_d3d_primitive_topology(type));
		window->get_device_context()->IASetIndexBuffer(current_element_array_provider->get_buffer(window->get_device()), to_d3d_format(indices_type), 0);
		window->validate_context();
		window->get_device_context()->DrawIndexed(count, to_d3d_index_location(indices_type, static_cast<UINT>(offset)), 0);
		reset_primitives_elements();
	}

	void D3DGraphicContext::draw_primitives_elements_instanced(PrimitivesType type, int count, const ElementArrayBufferPtr &array_provider, VertexAttributeDataType indices_type, size_t offset, int instance_count)
	{
		set_primitives_elements(array_provider);
		apply_input_layout();
		window->get_device_context()->IASetPrimitiveTopology(to_d3d_primitive_topology(type));
		window->get_device_context()->IASetIndexBuffer(current_element_array_provider->get_buffer(window->get_device()), to_d3d_format(indices_type), 0);
		window->validate_context();
		window->get_device_context()->DrawIndexedInstanced(count, instance_count, to_d3d_index_location(indices_type, static_cast<UINT>(offset)), 0, 0);
		reset_primitives_elements();
	}

	void D3DGraphicContext::set_scissor(const Rect &rect)
	{
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			scissor_rects[i].left = rect.left;
			scissor_rects[i].top = rect.top;
			scissor_rects[i].right = rect.right;
			scissor_rects[i].bottom = rect.bottom;
		}
		window->get_device_context()->RSSetScissorRects(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, scissor_rects);
	}

	void D3DGraphicContext::reset_scissor()
	{
		// To do: remove reset_scissor as the only way to disable scissor is to create a rasterizer state with ScissorEnable to false
		set_scissor(Rect(0, 0, 0x7FFFFFFF, 0x7FFFFFFF));
	}

	void D3DGraphicContext::dispatch(int x, int y, int z)
	{
		window->get_device_context()->Dispatch(x, y, z);
	}

	void D3DGraphicContext::clear(const Colorf &color)
	{
		ID3D11RenderTargetView *views[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		window->get_device_context()->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, views, 0);

		FLOAT c[4] = { color.r, color.g, color.b, color.a };
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			if (views[i])
			{
				window->get_device_context()->ClearRenderTargetView(views[i], c);
				views[i]->Release();
			}
		}
	}

	void D3DGraphicContext::clear_depth(float value)
	{
		ID3D11DepthStencilView *dsv = 0;
		window->get_device_context()->OMGetRenderTargets(0, 0, &dsv);
		if (dsv)
		{
			window->get_device_context()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, value, 0);
			dsv->Release();
		}
	}

	void D3DGraphicContext::clear_stencil(int value)
	{
		ID3D11DepthStencilView *dsv = 0;
		window->get_device_context()->OMGetRenderTargets(0, 0, &dsv);
		if (dsv)
		{
			window->get_device_context()->ClearDepthStencilView(dsv, D3D11_CLEAR_STENCIL, 0, value);
			dsv->Release();
		}
	}

	void D3DGraphicContext::set_viewport(const Rectf &viewport)
	{
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			viewports[i].TopLeftX = viewport.left;
			viewports[i].TopLeftY = viewport.top;
			viewports[i].Width = viewport.width();
			viewports[i].Height = viewport.height();
		}
		window->get_device_context()->RSSetViewports(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, viewports);
	}

	void D3DGraphicContext::set_viewport(int index, const Rectf &viewport)
	{
		if (index >= 0 && index < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		{
			viewports[index].TopLeftX = viewport.left;
			viewports[index].TopLeftY = viewport.top;
			viewports[index].Width = viewport.width();
			viewports[index].Height = viewport.height();
			window->get_device_context()->RSSetViewports(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, viewports);
		}
	}

	void D3DGraphicContext::set_depth_range(int index, float n, float f)
	{
		if (index >= 0 && index < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		{
			viewports[index].MinDepth = n;
			viewports[index].MaxDepth = f;
			window->get_device_context()->RSSetViewports(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, viewports);
		}
		else if (index == -1)
		{
			for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
			{
				viewports[i].MinDepth = n;
				viewports[i].MaxDepth = f;
			}
			window->get_device_context()->RSSetViewports(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, viewports);
		}
	}

	void D3DGraphicContext::on_window_resized()
	{
		Size new_size = window->backing_viewport().size();
		window_resized_signal(new_size);
	}

	int D3DGraphicContext::major_version() const
	{
		switch (window->get_feature_level())
		{
		case D3D_FEATURE_LEVEL_9_1:
		case D3D_FEATURE_LEVEL_9_2:
		case D3D_FEATURE_LEVEL_9_3: return 9;
		case D3D_FEATURE_LEVEL_10_0:
		case D3D_FEATURE_LEVEL_10_1: return 10;
		case D3D_FEATURE_LEVEL_11_0:
		default:
		case D3D_FEATURE_LEVEL_11_1: return 11;
		}
	}

	int D3DGraphicContext::minor_version() const
	{
		switch (window->get_feature_level())
		{
		case D3D_FEATURE_LEVEL_9_1: return 1;
		case D3D_FEATURE_LEVEL_9_2: return 2;
		case D3D_FEATURE_LEVEL_9_3: return 3;
		case D3D_FEATURE_LEVEL_10_0: return 0;
		case D3D_FEATURE_LEVEL_10_1: return 1;
		case D3D_FEATURE_LEVEL_11_0: return 0;
		default:
		case D3D_FEATURE_LEVEL_11_1: return 1;
		}
	}

	bool D3DGraphicContext::has_compute_shader_support() const
	{
		switch (window->get_feature_level())
		{
		case D3D_FEATURE_LEVEL_9_1:
		case D3D_FEATURE_LEVEL_9_2:
		case D3D_FEATURE_LEVEL_9_3: return false;
		case D3D_FEATURE_LEVEL_10_0:
		case D3D_FEATURE_LEVEL_10_1: break;
		case D3D_FEATURE_LEVEL_11_0:
		default:
		case D3D_FEATURE_LEVEL_11_1: return true;
		}

		D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS options = { 0 };

		HRESULT result = window->get_device()->CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS, &options, sizeof(D3D11_FEATURE_DATA_D3D10_X_HARDWARE_OPTIONS));
		D3DTarget::throw_if_failed("D3D11Device.CheckFeatureSupport(D3D11_FEATURE_D3D10_X_HARDWARE_OPTIONS) failed", result);

		return options.ComputeShaders_Plus_RawAndStructuredBuffers_Via_Shader_4_x != FALSE;
	}

	D3D11_PRIMITIVE_TOPOLOGY D3DGraphicContext::to_d3d_primitive_topology(PrimitivesType type)
	{
		switch (type)
		{
		case type_points: return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		case type_line_strip: return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		case type_line_loop: break;
		case type_lines: return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		case type_triangle_strip: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		case type_triangle_fan: break;
		case type_triangles: return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}
		throw Exception("Unsupported primitives type");
	}

	DXGI_FORMAT D3DGraphicContext::to_d3d_format(VertexAttributeDataType indices_type)
	{
		switch (indices_type)
		{
		case type_unsigned_byte: return DXGI_FORMAT_R8_UINT;
		case type_unsigned_short: return DXGI_FORMAT_R16_UINT;
		case type_unsigned_int: return DXGI_FORMAT_R32_UINT;
		case type_byte: return DXGI_FORMAT_R8_SINT;
		case type_short: return DXGI_FORMAT_R16_SINT;
		case type_int: return DXGI_FORMAT_R32_SINT;
		case type_float: return DXGI_FORMAT_R32_FLOAT;
		}
		throw Exception("Unsupported index type");
	}

	UINT D3DGraphicContext::to_d3d_index_location(VertexAttributeDataType indices_type, size_t offset)
	{
		int index_data_bytesize = 4;
		switch (indices_type)
		{
		case type_unsigned_byte:
		case type_byte:
			index_data_bytesize = 1;
			break;

		case type_unsigned_short:
		case type_short:
			index_data_bytesize = 2;
			break;

		case type_unsigned_int:
		case type_int:
		case type_float:
			index_data_bytesize = 4;
			break;
		}
		if (offset % index_data_bytesize != 0)
			throw Exception("Unaligned element offsets not supported by D3D target");
		return offset / index_data_bytesize;
	}

	void D3DGraphicContext::set_default_dsv()
	{
		if (default_depth != 0 && !default_depth_render_buffer)
		{
			TextureFormat texture_format = tf_depth_component32;
			if (default_depth <= 16)
				texture_format = tf_depth_component32;
			else  if (default_depth <= 24)
				texture_format = tf_depth_component24;

			Size viewport_size = display_window_size();

			default_depth_render_buffer = std::make_shared<D3DRenderBuffer>(window->get_device(), viewport_size.width, viewport_size.height, texture_format, 1);
			default_dsv = default_depth_render_buffer->create_dsv(window->get_device());
		}

		ID3D11RenderTargetView *targets[] = { window->get_back_buffer_rtv() };
		ID3D11DepthStencilView *default_depth_stencil_rtv = (default_depth != 0) ? default_dsv.get() : nullptr;
		window->get_device_context()->OMSetRenderTargets(1, targets, default_depth_stencil_rtv);
	}
}
