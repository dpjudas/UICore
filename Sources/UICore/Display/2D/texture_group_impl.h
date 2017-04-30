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
**    Kenneth Gangstoe
*/

#pragma once

#include <list>
#include "UICore/Display/Render/texture_2d.h"
#include "UICore/Display/2D/texture_group.h"

namespace uicore
{
	class GraphicContext;

	class TextureGroupImpl : public TextureGroup
	{
	public:
		TextureGroupImpl(const Size &texture_size);
		~TextureGroupImpl();

		int subtexture_count() const override;
		int subtexture_count(unsigned int texture_index) const override;
		int texture_count() const override { return root_nodes.size(); }
		TextureGroupAllocationPolicy allocation_policy() const override { return texture_allocation_policy; }
		Size texture_size() const override { return initial_texture_size; }
		std::vector<std::shared_ptr<Texture2D>> textures() const override;
		TextureGroupImage add(const std::shared_ptr<GraphicContext> &context, const Size &size) override { return add_new_node(context, size); }
		void remove(const TextureGroupImage &subtexture) override;
		void set_allocation_policy(TextureGroupAllocationPolicy policy) override { texture_allocation_policy = policy; }
		void insert_texture(const std::shared_ptr<Texture2D> &texture, const Rect &texture_rect) override;

	private:
		class Node
		{
		public:
			Node();
			Node(const Rect &rect);
			~Node();

			int get_subtexture_count() const;

			Node *insert(const Size &texture_size, int texture_id);
			Node *find_image_rect(const Rect &new_rect);

			void clear();

			Node *child[2];
			Rect node_rect;

			int id;
			Rect image_rect;
		};

		struct RootNode
		{
		public:
			std::shared_ptr<Texture2D> texture;
			Node node;
		};

		TextureGroupImage add_new_node(const std::shared_ptr<GraphicContext> &context, const Size &texture_size);
		RootNode *add_new_root(const std::shared_ptr<GraphicContext> &context, const Size &texture_size);

		std::vector<RootNode *> root_nodes;

		Size initial_texture_size;
		TextureGroupAllocationPolicy texture_allocation_policy = TextureGroupAllocationPolicy::create_new_texture;

		RootNode *active_root;
		int next_id;
	};
}
