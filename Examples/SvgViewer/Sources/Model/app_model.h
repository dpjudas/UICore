
#pragma once

class AppModel
{
public:
	static AppModel *instance();

	std::string svg_filename = "Resources/tiger.svg";
};
