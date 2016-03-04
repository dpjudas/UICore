
#pragma once

#include "View/Theme/form_view.h"
#include "View/Theme/button_view.h"
#include "View/Theme/checkbox_view.h"
#include "View/Theme/radio_button_view.h"
#include "View/Theme/form_view.h"
#include "View/Theme/image_view.h"
#include "View/Theme/label_view.h"
#include "View/Theme/listbox_view.h"
#include "View/Theme/scroll_view.h"
#include "View/Theme/slider_view.h"
#include "View/Theme/spin_view.h"
#include "View/Theme/text_block_view.h"
#include "View/Theme/text_area_view.h"
#include "View/Theme/text_field_view.h"

class MainWindowView : public FormView
{
public:
	MainWindowView()
	{
		using namespace uicore;

		style()->set("align-items: flex-start");

		auto headline = add_child<HeadlineView>();
		headline->style()->set("margin-top: 0");
		headline->add_text("Form example with themed views");

		auto p = add_child<ParagraphView>();
		p->style()->set("margin-top: 0");
		p->add_text("This is an example of how to create a form using themed UICore standard views.");

		auto button = add_child<ButtonView>();
		button->label()->set_text("Test Button");

		auto slider = add_child<SliderView>();
		slider->style()->set("width: 300px; margin-top: 5px");

		auto radio_row = add_child<RowView>();
		radio_row->style()->set("margin: 5px 0; align-items: center");
		for (auto text : { "Option 1", "Option 2", "Option 3" })
		{
			auto radio = radio_row->add_child<RadioButtonView>();
			radio->style()->set("margin-right: 5px");

			auto label = radio_row->add_child<LabelView>();
			label->style()->set("margin-right: 15px");
			label->set_text(text);
		}

		auto check_row = add_child<RowView>();
		check_row->style()->set("margin: 5px 0; align-items: center");
		for (auto text : { "Option 1", "Option 2", "Option 3" })
		{
			auto check = check_row->add_child<CheckBoxView>();
			check->style()->set("margin-right: 5px");

			auto label = check_row->add_child<LabelView>();
			label->style()->set("margin-right: 15px");
			label->set_text(text);
		}

		auto textfield = add_child<TextFieldView>();
		textfield->set_placeholder("A Textfield");
		textfield->set_preferred_size(40);
	}
};
