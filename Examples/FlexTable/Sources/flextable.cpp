/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Mark Page
*/

#include "precomp.h"
#include "flextable.h"

using namespace uicore;

ApplicationInstance<FlexTable> uicoreapp;

FlexTable::FlexTable()
{
	WindowManager::present_main<FlexTableController>();
	WindowManager::set_exit_on_last_close();
}

FlexTableController::FlexTableController()
{
	set_title("UICore: Flex Table");
	set_content_size(Sizef(1400, 800), true);

	auto root = std::make_shared<ColumnView>();
	set_root_view(root);

	root_view()->style()->set("background-color: white;");

	auto outer_view = root_view()->add_child<View>();
	outer_view->style()->set(R"(
		border: 20px solid blue;
		padding: 15px;
		margin: 0px;
		background-color: black;
		width: 1000px;
		height: 500px;
	)");

	auto column_flex_view = outer_view->add_child<View>();
	column_flex_view->style()->set(R"(
		border: 8px solid yellow;
		padding: 15px;
		margin: 0px;
		align-items: flex-start;
		flex-direction: column;
		height: 400px;
		background-color: #666666;
	)");

	std::string row_flex_style = R"(
		border: 8px solid red;
		padding: 15px;
		margin: 0px;
		width: 100%;
		box-sizing: border-box;
		align-items:flex-start;
		background-color: #444444;
	)";

	std::string row_view_style = R"(
		color: white;
		height: 60px;
		border: 8px solid white;
		background: black;
	)";

	for (int cnt = 0; cnt < 2; cnt++)
	{
		auto row_flex_view = column_flex_view->add_child<View>();
		row_flex_view->style()->set(row_flex_style);

		auto row_view1 = row_flex_view->add_child<View>();
		row_view1->style()->set(row_view_style);
		row_view1->style()->set("flex: 0 0 400px;");

		auto row_view2 = row_flex_view->add_child<View>();
		row_view2->style()->set(row_view_style);
		row_view2->style()->set("flex: 1 0 auto;");

		auto row_view3 = row_flex_view->add_child<View>();
		row_view3->style()->set(row_view_style);
		row_view3->style()->set("flex: 0 0 200px;");
	}
}
