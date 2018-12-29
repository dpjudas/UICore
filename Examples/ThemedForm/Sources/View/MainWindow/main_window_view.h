
#pragma once

/*
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
*/

class BorderImage
{
public:
	float top;
	float right;
	float bottom;
	float left;
	std::string image;
};

class WindowsUITheme : public uicore::UITheme
{
public:
	std::map<std::string, BorderImage> border_images;

	WindowsUITheme()
	{
		border_images["button"] = { 6, 6, 5, 5, "button_normal.png" };
	}

	std::unique_ptr<uicore::ViewTheme> create_view_theme(const char *styleclass) override;
};

class WindowsViewTheme : public uicore::ViewTheme
{
public:
	WindowsViewTheme(WindowsUITheme *uitheme, const std::string &styleclass) : uitheme(uitheme), styleclass(styleclass)
	{
	}

	uicore::ViewThemeBorder border() override
	{
		uicore::ViewThemeBorder border;
		if (styleclass == "button")
		{
			const BorderImage &bi = uitheme->border_images[styleclass];
			border.left = bi.left;
			border.top = 4;// bi.top;
			border.right = bi.right;
			border.bottom = 2;// bi.bottom;
		}
		else
		{
			border.left = 11;
			border.top = 11;
			border.right = 11;
			border.bottom = 11;
		}
		return border;
	}

	void render(const std::shared_ptr<uicore::Canvas> &canvas, const uicore::ViewGeometry &geometry)
	{
		using namespace uicore;

		if (styleclass == "button")
		{
			const BorderImage &border = uitheme->border_images[styleclass];
			if (!image)
				image = Image::create(canvas, "Resources/button_normal.png");

			Rectf bbox = geometry.border_box();
			float u[4], v[4], x[4], y[4];

			u[0] = 0.0f;
			u[1] = border.left;
			u[2] = image->width() - border.right;
			u[3] = image->width();

			v[0] = 0.0f;
			v[1] = border.top;
			v[2] = image->height() - border.bottom;
			v[3] = image->height();

			x[0] = bbox.left;
			x[1] = bbox.left + border.left;
			x[2] = bbox.right - border.right;
			x[3] = bbox.right;

			y[0] = bbox.top;
			y[1] = bbox.top + border.top;
			y[2] = bbox.bottom - border.bottom;
			y[3] = bbox.bottom;

			for (int i = 0; i < 3; i++)
			{
				if (x[i] < x[i + 1])
				{
					for (int j = 0; j < 3; j++)
					{
						if (y[j] < y[j + 1])
							image->draw(canvas, Rectf::ltrb(u[i], v[j], u[i + 1], v[j + 1]), Rectf::ltrb(x[i], y[j], x[i + 1], y[j + 1]));
					}
				}
			}
		}
		else if (styleclass == "form")
		{
			Path::rect(geometry.border_box())->fill(canvas, Colorf(240 / 255.0f, 240 / 255.0f, 240 / 255.0f));
		}
	}

	std::shared_ptr<uicore::Font> font(const std::shared_ptr<uicore::Canvas> &canvas)
	{
		if (!mfont)
			mfont = uicore::Font::create("Segue UI", 13.0f);
		return mfont;
	}

	uicore::Colorf text_color()
	{
		return uicore::StandardColorf::black();
	}

	void set_state(const std::string &name, bool value)
	{
		state[name] = value;
	}

	std::map<std::string, bool> state;

	WindowsUITheme *uitheme;
	std::string styleclass;
	std::shared_ptr<uicore::Image> image;
	std::shared_ptr<uicore::Font> mfont;
};

inline std::unique_ptr<uicore::ViewTheme> WindowsUITheme::create_view_theme(const char *styleclass)
{
	return std::make_unique<WindowsViewTheme>(this, styleclass);
}

class MainWindowView : public uicore::ColumnView
{
public:
	MainWindowView()
	{
		using namespace uicore;

		UITheme::set(std::make_shared<WindowsUITheme>());

		set_theme(UITheme::get()->create_view_theme("form"));

		auto button = std::make_shared<ButtonBaseView>();
		add_child(button, "button");
		button->label()->set_text("Test Button");

		button = std::make_shared<ButtonBaseView>();
		add_child(button, "button");
		button->label()->set_text("Test Button 2");

		/*
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
		*/
	}
};
