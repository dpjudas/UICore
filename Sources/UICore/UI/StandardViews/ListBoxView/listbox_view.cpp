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
**    Mark Page
*/

#include "UICore/precomp.h"
#include "UICore/UI/StandardViews/listbox_view.h"
#include "UICore/UI/StandardViews/label_view.h"
#include "UICore/UI/Events/key_event.h"
#include "UICore/UI/Events/pointer_event.h"
#include "listbox_view_impl.h"

namespace uicore
{
	ListBoxBaseView::ListBoxBaseView() : impl(new ListBoxBaseViewImpl())
	{
		impl->listbox = this;
		content_view()->style()->set("flex-direction: column");
		
		set_focus_policy(FocusPolicy::accept);

		slots.connect(sig_key_press(), [this](KeyEvent *e) { impl->on_key_press(*e); });
		slots.connect(sig_pointer_press(), [this](PointerEvent *e) { impl->on_pointer_press(*e); });
		slots.connect(sig_pointer_release(), [this](PointerEvent *e) { impl->on_pointer_release(*e); });
	}

	ListBoxBaseView::~ListBoxBaseView()
	{
	}
	
	void ListBoxBaseView::set_items(const std::vector<std::shared_ptr<View>> &items)
	{
		impl->selected_item = -1;
		
		auto views = content_view()->children();
		while (!views.empty())
			views.back()->remove_from_parent();
		
		for (auto &item : items)
		{
			content_view()->add_child(item);
			slots.connect(item->sig_pointer_enter(), [this](PointerEvent *e) { impl->on_pointer_enter(*e); });
			slots.connect(item->sig_pointer_leave(), [this](PointerEvent *e) { impl->on_pointer_leave(*e); });
		}
	}
	
	int ListBoxBaseView::selected_item() const
	{
		return impl->selected_item;
	}
	
	void ListBoxBaseView::set_selected_item(int index)
	{
		if (index == impl->selected_item)
			return;
		
		if (index < -1 || index >= (int) content_view()->children().size())
			throw Exception("Listbox index out of bounds");

		if (impl->selected_item != -1)
			content_view()->children().at(impl->selected_item)->set_state("selected", false);
		
		if (index != -1)
		{
			if (impl->hot_item == index)
				impl->set_hot_item(-1);

			auto new_selected_item = content_view()->children().at(index);
			new_selected_item->set_state("selected", true);
			
			// To do: call set_content_offset() if new_selected_item is not within range (maybe add a helper on ScrollBaseView for this?)
		}
		
		impl->selected_item = index;
	}

	Signal<void()> &ListBoxBaseView::sig_selection_changed()
	{
		return impl->sig_selection_changed;
	}
}
