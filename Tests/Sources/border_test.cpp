
#include "precomp.h"
#include "border_test.h"

using namespace uicore;

BorderTestController::BorderTestController()
{
	set_title("UICore Border Test");
	set_frame_size({ 1200.0f, 650.0f });
	set_root_view(root);

	root->style()->set(R"(
		background: rgb(240,240,240);
		padding: 11px;
	)");

	root->add_child()->style()->set(R"(
		width: 100px;
		height: 100px;
		border: 10px solid black;
		background: orangered;
		margin-right: 15px;
	)");

	root->add_child()->style()->set(R"(
		width: 100px;
		height: 100px;
		border: 10px solid black;
		border-top-color: #ddd;
		border-left-color: #ddd;
		border-right-color: #666;
		border-bottom-color: #666;
		background: #999;
		margin-right: 15px;
	)");

	root->add_child()->style()->set(R"(
		width: 100px;
		height: 100px;
		border: 10px solid black;
		border-radius: 5px;
		background: orangered;
		margin-right: 15px;
	)");

	root->add_child()->style()->set(R"(
		width: 100px;
		height: 100px;
		border: 10px solid black;
		border-radius: 20px;
		background: orangered;
		margin-right: 15px;
	)");

	root->add_child()->style()->set(R"(
		width: 100px;
		height: 100px;
		border: 10px solid black;
		border-radius: 20px 10px;
		background: orangered;
		margin-right: 15px;
	)");

	root->add_child()->style()->set(R"(
		width: 100px;
		height: 100px;
		border: 10px solid black;
		border-radius: 20px 10px;
		border-top-color: #ddd;
		border-left-color: #ddd;
		border-right-color: #666;
		border-bottom-color: #666;
		background: #999;
		margin-right: 15px;
	)");

	root->add_child()->style()->set(R"(
		width: 100px;
		height: 100px;
		border: 10px solid black;
		border-top-color: rgba(0,0,0,0);
		border-left-color: rgba(0,0,0,0);
		border-right-color: #666;
		border-bottom-color: #666;
		margin-right: 15px;
	)");

	root->add_child()->style()->set(R"(
		width: 100px;
		height: 100px;
		padding: 10px;
		padding-bottom: 0;
		border-bottom: 10px solid black;
		background: orangered;
		margin-right: 15px;
	)");
}
