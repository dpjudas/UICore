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
	enum class ContentOverflow
	{
		hidden,
		scroll,
		automatic
	};
	
	class ScrollBarBaseView;
	class ScrollBaseViewImpl;
	
	class ScrollBaseView : public View
	{
	public:
		ScrollBaseView();
		~ScrollBaseView();
		
		std::shared_ptr<ScrollBarBaseView> scrollbar_x_view() const;
		std::shared_ptr<ScrollBarBaseView> scrollbar_y_view() const;
		
		std::shared_ptr<View> content_view() const;
		
		ContentOverflow overflow_x() const;
		ContentOverflow overflow_y() const;
		void set_overflow_x(ContentOverflow value);
		void set_overflow_y(ContentOverflow value);
		void set_overflow(ContentOverflow value_x, ContentOverflow value_y);
		
		Pointf content_offset() const;
		void set_content_offset(const Pointf &offset, bool animated = false);
		
		void layout_children(const CanvasPtr &canvas) override;

	protected:
		float calculate_preferred_width(const CanvasPtr &canvas) override;
		float calculate_preferred_height(const CanvasPtr &canvas, float width) override;
		float calculate_first_baseline_offset(const CanvasPtr &canvas, float width) override;
		float calculate_last_baseline_offset(const CanvasPtr &canvas, float width) override;
		
	private:
		std::unique_ptr<ScrollBaseViewImpl> impl;
	};
}
