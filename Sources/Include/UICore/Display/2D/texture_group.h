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
**    Magnus Norddahl
*/

#pragma once

#include <memory>
#include "../../Core/Math/size.h"
#include "../../Core/Math/rect.h"

namespace uicore
{
	class Texture2D;
	class GraphicContext;

	/// \brief Image position in a TextureGroup
	class TextureGroupImage
	{
	public:
		TextureGroupImage() { }
		TextureGroupImage(const std::shared_ptr<Texture2D> &texture, const Rect &geometry) : _texture(texture), _geometry(geometry) { }

		explicit operator bool() { return (bool)texture(); }

		const std::shared_ptr<Texture2D> &texture() const { return _texture; }
		const Rect &geometry() const { return _geometry; }

	private:
		std::shared_ptr<Texture2D> _texture;
		Rect _geometry;
	};

	/// \brief TextureGroup allocation policy.
	enum class TextureGroupAllocationPolicy
	{
		create_new_texture,
		search_previous_textures
	};

	/// \brief Dynamic atlas texture class
	class TextureGroup
	{
	public:
		/// \brief Constructs a texture group
		static std::shared_ptr<TextureGroup> create(const Size &texture_size);

		/// \brief Returns the amount of sub-textures allocated in group.
		virtual int subtexture_count() const = 0;

		/// \brief Returns the amount of sub-textures for a specific texture index.
		virtual int subtexture_count(unsigned int texture_index) const = 0;

		/// \brief Returns the amount of textures used by group.
		virtual int texture_count() const = 0;

		/// \brief Returns the texture allocation policy.
		virtual TextureGroupAllocationPolicy allocation_policy() const = 0;

		/// \brief Returns the size of the textures used by this texture group.
		virtual Size texture_size() const = 0;

		/// \brief Returns the textures.
		virtual std::vector<std::shared_ptr<Texture2D>> textures() const = 0;

		/// \brief Allocate space for another sub texture.
		virtual TextureGroupImage add(const std::shared_ptr<GraphicContext> &context, const Size &size) = 0;

		/// \brief Deallocate space, from a previously allocated texture
		///
		/// Warning - It is advised to set TextureGroupAllocationPolicy to search_previous_textures
		/// if using this function.  Also be aware of texture fragmentation.
		/// Empty textures are not removed.
		virtual void remove(const TextureGroupImage &subtexture) = 0;

		/// \brief Set the texture allocation policy.
		virtual void set_allocation_policy(TextureGroupAllocationPolicy policy) = 0;

		/// \brief Insert an existing texture into the texture group
		///
		/// \param texture = Texture to insert
		/// \param texture_rect = Free space within the texture that the texture group can use
		virtual void insert_texture(const std::shared_ptr<Texture2D> &texture, const Rect &texture_rect) = 0;
	};
}
