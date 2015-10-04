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

#include "UICore/precomp.h"
#include "graphic_context_impl.h"
#include "primitives_array_impl.h"
#include "UICore/Display/Render/shared_gc_data.h"

namespace uicore
{
	GraphicContext_Impl::GraphicContext_Impl(GraphicContextProvider *provider)
		: provider(provider)
	{
		SharedGCData::add_ref();

		resize_slot = provider->sig_window_resized().connect(bind_member(this, &GraphicContext_Impl::on_window_resized));
		set_viewport(-1, provider->get_display_window_size());
	}

	GraphicContext_Impl::~GraphicContext_Impl()
	{
		SharedGCData::release_ref();
	}

	void GraphicContext_Impl::on_window_resized(const Size &size)
	{
		display_window_size = size;
		if (!write_frame_buffer)
			set_viewport(-1, get_size());
	}

	Size GraphicContext_Impl::get_size()
	{
		if (!write_frame_buffer)
		{
			if ((display_window_size.width == 0) || (display_window_size.height == 0))
				display_window_size = provider->get_display_window_size();
			return display_window_size;
		}
		else
		{
			return write_frame_buffer->get_size();
		}
	}

	void GraphicContext_Impl::set_texture(int unit_index, const Texture &texture)
	{
		// Limit the number of unit index to 255, this should always be enough. This simplifies the saving of the texture
		if ((unit_index < 0) || (unit_index > 255))
			throw Exception("Invalid texture unit index");

		// Extend the selected textures array if required
		if (unit_index >= textures.size())
			textures.resize(unit_index + 1);

		textures[unit_index] = texture;
		if (!texture.is_null())
			provider->set_texture(unit_index, texture);
		else
			provider->reset_texture(unit_index);
	}

	void GraphicContext_Impl::set_textures(std::vector<Texture> &new_textures)
	{
		for (auto i = textures.size(); i < new_textures.size(); i++)
			provider->reset_texture(i);

		textures = new_textures;

		int i = 0;
		for (const auto &texture : textures)
		{
			if (!texture.is_null())
				provider->set_texture(i, texture);
			else
				provider->reset_texture(i);
			i++;
		}
	}

	void GraphicContext_Impl::set_image_texture(int unit_index, const Texture &texture)
	{
		// Limit the number of unit index to 255, this should always be enough. This simplifies the saving of the texture
		if ((unit_index < 0) || (unit_index > 255))
			throw Exception("Invalid texture unit index");

		// Extend the selected textures array if required
		if (unit_index >= image_textures.size())
			image_textures.resize(unit_index + 1);

		image_textures[unit_index] = texture;

		if (!texture.is_null())
			provider->set_image_texture(unit_index, texture);
		else
			provider->reset_image_texture(unit_index);
	}

	void GraphicContext_Impl::set_image_textures(std::vector<Texture> &new_textures)
	{
		for (auto i = textures.size(); i < new_textures.size(); i++)
			provider->reset_texture(i);

		image_textures = new_textures;

		int i = 0;
		for (const auto &texture : image_textures)
		{
			if (!texture.is_null())
				provider->set_image_texture(i, texture);
			else
				provider->reset_image_texture(i);
			i++;
		}

		image_textures = new_textures;
	}

	void GraphicContext_Impl::set_uniform_buffer(int index, const UniformBufferPtr &buffer)
	{
		// Limit the number of unit index to 255, this should always be enough. This simplifies the saving of the texture
		if ((index < 0) || (index > 255))
			throw Exception("Invalid uniform buffer index");

		// Extend the selected uniform_buffers array if required
		if (index >= uniform_buffers.size())
			uniform_buffers.resize(index + 1);

		uniform_buffers[index] = buffer;

		if (buffer)
			provider->set_uniform_buffer(index, buffer);
		else
			provider->reset_uniform_buffer(index);
	}

	void GraphicContext_Impl::set_storage_buffer(int index, const StorageBufferPtr &buffer)
	{
		// Limit the number of unit index to 255, this should always be enough. This simplifies the saving of the texture
		if ((index < 0) || (index > 255))
			throw Exception("Invalid storage buffer index");

		// Extend the selected storage_buffers array if required
		if (index >= storage_buffers.size())
			storage_buffers.resize(index + 1);

		storage_buffers[index] = buffer;

		if (buffer)
			provider->set_storage_buffer(index, buffer);
		else
			provider->reset_storage_buffer(index);
	}

	void GraphicContext_Impl::set_scissor(const Rect &rect)
	{
		scissor_set = true;
		scissor = rect;
		provider->set_scissor(rect);
	}

	void GraphicContext_Impl::reset_scissor()
	{
		scissor_set = false;
		provider->reset_scissor();
	}

	void GraphicContext_Impl::set_viewport(int index, const Rectf &viewport_box)
	{
		if (index < 0)	// Special case, clear all viewports and only have a single one
		{
			if (viewport.size() != 1)
				viewport.resize(1);
			index = 0;
		}

		if (viewport.size() <= index)	// Grow vector if not large enough
			viewport.resize(index + 1);

		viewport[index] = viewport_box;

		provider->set_viewport(index, viewport_box);
	}

	void GraphicContext_Impl::set_depth_range(int viewport, float n, float f)
	{
		if (viewport < 0)	// Special case, clear all depth ranges and only have a single one
		{
			if (depth_range.size() != 1)
				depth_range.resize(1);
			viewport = 0;
		}

		if (depth_range.size() <= viewport)	// Grow vector if not large enough
			depth_range.resize(viewport + 1);

		depth_range[viewport] = Sizef(n, f);

		if (depth_range.size() == 1)
			provider->set_depth_range(n, f);
		else
			provider->set_depth_range(viewport, n, f);
	}

	void GraphicContext_Impl::set_frame_buffer(const FrameBufferPtr &write_buffer, const FrameBufferPtr &read_buffer)
	{
		write_frame_buffer = write_buffer;
		read_frame_buffer = read_buffer;
		if (!write_frame_buffer)
			provider->reset_frame_buffer();
		else
			provider->set_frame_buffer(write_buffer, read_buffer);
	}

	void GraphicContext_Impl::set_program_object(StandardProgram standard_program)
	{
		program = provider->get_program_object(standard_program);
		program_standard_set = true;
		program_standard = standard_program;
		provider->set_program_object(standard_program);
	}

	ProgramObjectPtr GraphicContext_Impl::get_program_object() const
	{
		return program;
	}

	void GraphicContext_Impl::set_program_object(const ProgramObjectPtr &new_program)
	{
		program = new_program;
		program_standard_set = false;
		if (program)
			provider->set_program_object(program);
		else
			provider->reset_program_object();
	}

	void GraphicContext_Impl::reset_program_object()
	{
		program.reset();
		program_standard_set = false;
		provider->reset_program_object();
	}

	void GraphicContext_Impl::set_rasterizer_state(const RasterizerStatePtr &state)
	{
		rasterizer_state = state;
		provider->set_rasterizer_state(static_cast<RasterizerState*>(state.get()));
	}

	void GraphicContext_Impl::set_blend_state(const BlendStatePtr &state, const Colorf &blend_color, unsigned int sample_mask)
	{
		provider->set_blend_state(static_cast<BlendState*>(state.get()), blend_color, sample_mask);
	}

	void GraphicContext_Impl::set_depth_stencil_state(const DepthStencilStatePtr &state, int new_stencil_ref)
	{
		depth_stencil_state = state;
		stencil_ref = new_stencil_ref;
		provider->set_depth_stencil_state(static_cast<DepthStencilState*>(state.get()), stencil_ref);
	}

	void GraphicContext_Impl::set_draw_buffer(DrawBuffer buffer)
	{
		draw_buffer = buffer;
		provider->set_draw_buffer(buffer);
	}

	void GraphicContext_Impl::flush()
	{
		provider->flush();
	}
}
