
#pragma once

class BorderTestController : public uicore::WindowController
{
public:
	BorderTestController();

	std::shared_ptr<uicore::RowView> root = std::make_shared<uicore::RowView>();
};
