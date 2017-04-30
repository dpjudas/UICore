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
**    Harry Storbacka
**    Mark Page
*/

#pragma once

#include <map>

#include "gl1_graphic_context.h"
#include "UICore/Display/Render/texture_2d.h"
#include "UICore/Display/Render/frame_buffer.h"
#include "UICore/Core/System/disposable_object.h"
#include "pbuffer.h"

namespace uicore
{
	class GL1FrameBuffer : public FrameBuffer, DisposableObject
	{
	public:
		GL1FrameBuffer(GL1GraphicContext *gc_provider);
		~GL1FrameBuffer();

		Size size() const override;
		FrameBufferBindTarget bind_target() const override;

		void attach_color(int attachment_index, const std::shared_ptr<RenderBuffer> &render_buffer) override;
		void attach_color(int attachment_index, const std::shared_ptr<Texture1D> &texture, int level) override;
		void attach_color(int attachment_index, const std::shared_ptr<Texture1DArray> &texture, int array_index, int level) override;
		void attach_color(int attachment_index, const std::shared_ptr<Texture2D> &texture, int level) override;
		void attach_color(int attachment_index, const std::shared_ptr<Texture2DArray> &texture, int array_index, int level) override;
		void attach_color(int attachment_index, const std::shared_ptr<Texture3D> &texture, int depth, int level) override;
		void attach_color(int attachment_index, const std::shared_ptr<TextureCube> &texture, TextureSubtype subtype, int level) override;
		void detach_color(int attachment_index) override;

		void attach_stencil(const std::shared_ptr<RenderBuffer> &render_buffer) override;
		void attach_stencil(const std::shared_ptr<Texture2D> &texture, int level) override;
		void attach_stencil(const std::shared_ptr<TextureCube> &texture, TextureSubtype subtype, int level) override;
		void detach_stencil() override;

		void attach_depth(const std::shared_ptr<RenderBuffer> &render_buffer) override;
		void attach_depth(const std::shared_ptr<Texture2D> &texture, int level) override;
		void attach_depth(const std::shared_ptr<TextureCube> &texture, TextureSubtype subtype, int level) override;
		void detach_depth() override;

		void attach_depth_stencil(const std::shared_ptr<RenderBuffer> &render_buffer) override;
		void attach_depth_stencil(const std::shared_ptr<Texture2D> &texture, int level) override;
		void attach_depth_stencil(const std::shared_ptr<TextureCube> &texture, TextureSubtype subtype, int level) override;
		void detach_depth_stencil() override;

		void set_bind_target(FrameBufferBindTarget target) override;

		void set_active();
		void stop();
		void start();

		void set_state(OpenGLBlendState &new_blend);
		void set_state(OpenGLRasterizerState &new_rasterizer);
		void set_state(OpenGLDepthStencilState &new_depth_stencil);
		void set_state(GL1State &new_selected_state);

	private:
		void on_dispose() override;
		void sync_pbuffer();
		void sync_texture();
		void detach_all();

		mutable bool pbuffer_changed;
		struct WeakPtrCompare
		{
			bool operator()(const std::weak_ptr<Texture> &p1, const std::weak_ptr<Texture> &p2) const
			{
				return p1.lock() < p2.lock();
			}
		};
		std::map< std::weak_ptr<Texture>, PBuffer_GL1, WeakPtrCompare> texture_pbuffer_map;

		PBuffer_GL1 pbuffer;
		std::shared_ptr<Texture2D> selected_surface;
		GL1TextureObject *selected_texture_provider;

		GL1State selected_state;

		GL1GraphicContext *gc_provider;
	};
}
