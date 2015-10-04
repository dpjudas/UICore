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

#include "UICore/Display/TargetProviders/graphic_context_provider.h"
#include "UICore/Display/Render/program_object.h"
#include "UICore/Display/Render/storage_buffer.h"
#include "UICore/Display/Render/uniform_buffer.h"
#include "UICore/Display/Font/font.h"
#include "UICore/Core/Math/mat4.h"

namespace uicore
{
	class GraphicContext_Impl
	{
	public:
		GraphicContext_Impl(GraphicContextProvider *provider);
		~GraphicContext_Impl();

		void set_scissor(const Rect &rect);
		void reset_scissor();

		Size get_size();

		void set_frame_buffer(const FrameBufferPtr &write_buffer, const FrameBufferPtr &read_buffer);
		void set_viewport(int index, const Rectf &viewport);
		void set_depth_range(int viewport, float n, float f);

		void set_texture(int unit_index, const Texture &texture);
		void set_textures(std::vector<Texture> &textures);

		void set_image_texture(int unit_index, const Texture &texture);
		void set_image_textures(std::vector<Texture> &textures);

		void set_uniform_buffer(int index, const UniformBufferPtr &buffer);
		void set_storage_buffer(int index, const StorageBufferPtr &buffer);

		void set_program_object(StandardProgram standard_program);
		void set_program_object(const ProgramObjectPtr &program);
		void reset_program_object();

		ProgramObjectPtr get_program_object() const;

		void set_rasterizer_state(const RasterizerStatePtr &state);
		void set_blend_state(const BlendStatePtr &state, const Colorf &blend_color, unsigned int sample_mask);
		void set_depth_stencil_state(const DepthStencilStatePtr &state, int stencil_ref);

		void set_draw_buffer(DrawBuffer buffer);

		void flush();

	private:
		void on_window_resized(const Size &size);

		std::shared_ptr<GraphicContextProvider> provider;

		Size display_window_size;
		Slot resize_slot;

		RasterizerStatePtr default_rasterizer_state;
		BlendStatePtr default_blend_state;
		DepthStencilStatePtr default_depth_stencil_state;

		FrameBufferPtr read_frame_buffer;
		FrameBufferPtr write_frame_buffer;

		std::vector<Texture> textures;
		std::vector<Texture> image_textures;
		std::vector<UniformBufferPtr> uniform_buffers;
		std::vector<StorageBufferPtr> storage_buffers;

		Rect scissor;
		bool scissor_set = false;

		std::vector<Rectf> viewport = std::vector<Rectf>({ {} });
		std::vector<Sizef> depth_range = std::vector<Sizef>({ { 0.0f, 1.0f } });

		StandardProgram program_standard = program_color_only;
		bool program_standard_set = false;
		ProgramObjectPtr program;	//<-- Note this is valid when program_standard_set is true

		RasterizerStatePtr rasterizer_state;
		DepthStencilStatePtr depth_stencil_state;
		int stencil_ref = 0;
		DrawBuffer draw_buffer = buffer_back;

		friend class GraphicContext;
	};
}
