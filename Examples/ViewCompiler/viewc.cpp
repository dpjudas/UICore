
#include <uicore.h>
#include <iostream>

using namespace uicore;

int main(int argc, char **argv)
{
	try
	{
		std::string source = File::read_all_text("main_window.view");
		std::string code = ViewCompiler::compile(source);
		File::write_all_text("main_window.h", code);
	}
	catch (const Exception &e)
	{
		std::cout << e.message << std::endl;
		return 1;
	}
	return 0;
}
