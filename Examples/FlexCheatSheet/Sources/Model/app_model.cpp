
#include "precomp.h"
#include "app_model.h"

using namespace uicore;

AppModel *AppModel::instance()
{
	static AppModel model;
	return &model;
}
