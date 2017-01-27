
#include "precomp.h"
#include "border_test.h"

using namespace uicore;

class TestApplication : public Application
{
public:
	TestApplication()
	{
		WindowManager::set_exit_on_last_close();
		WindowManager::present_main<BorderTestController>();
	}
};

ApplicationInstance<TestApplication> uicoreapp;
