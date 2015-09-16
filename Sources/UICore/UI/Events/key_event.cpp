
#include "UICore/precomp.h"
#include "UICore/UI/Events/key_event.h"
#include "UICore/UI/View/view.h"

namespace uicore
{
	Pointf KeyEvent::pointer_pos(View *view) const
	{
		return view->from_root_pos(_pointer_pos);
	}
}
