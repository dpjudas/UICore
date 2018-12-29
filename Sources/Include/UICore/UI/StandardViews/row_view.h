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

#pragma once

#include "../View/view.h"

namespace uicore
{
	class RowViewImpl;

	class RowView : public View
	{
	public:
		RowView();
		~RowView();

		void add_child(std::shared_ptr<View> view, const char *style);
		void remove_child(std::shared_ptr<View> view);

		float preferred_width(const std::shared_ptr<Canvas> &canvas) override;
		float preferred_height(const std::shared_ptr<Canvas> &canvas, float width) override;
		float first_baseline_offset(const std::shared_ptr<Canvas> &canvas, float width) override;
		float last_baseline_offset(const std::shared_ptr<Canvas> &canvas, float width) override;
		bool needs_layout() const override;
		void set_needs_layout() override;
		std::shared_ptr<View> find_view_at(const Pointf &pos) const override;
		void render_content(const std::shared_ptr<Canvas> &canvas) override;

	private:
		std::unique_ptr<RowViewImpl> impl;
	};
}
