/*
**  UICore
**  Copyright (c) 1997-2015 The UICore Team
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
**  Note: Some of the libraries UICore may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#pragma once

#include "UI/Controller/window_controller.h"
#include "UI/Controller/window_manager.h"
#include "UI/Events/activation_change_event.h"
#include "UI/Events/close_event.h"
#include "UI/Events/event.h"
#include "UI/Events/focus_change_event.h"
#include "UI/Events/key_event.h"
#include "UI/Events/pointer_event.h"
#include "UI/Events/resize_event.h"
#include "UI/Events/input_code.h"
#include "UI/Events/keys.h"
#include "UI/StandardViews/row_view.h"
#include "UI/StandardViews/column_view.h"
#include "UI/StandardViews/button_view.h"
#include "UI/StandardViews/image_view.h"
#include "UI/StandardViews/label_view.h"
#include "UI/StandardViews/progress_view.h"
#include "UI/StandardViews/scroll_view.h"
#include "UI/StandardViews/scrollbar_view.h"
#include "UI/StandardViews/slider_view.h"
#include "UI/StandardViews/text_block_view.h"
#include "UI/StandardViews/text_field_view.h"
#include "UI/StandardViews/text_area_view.h"
#include "UI/StandardViews/checkbox_view.h"
#include "UI/StandardViews/radiobutton_view.h"
#include "UI/StandardViews/spin_view.h"
#include "UI/StandardViews/listbox_view.h"
#include "UI/Image/image_source.h"
#include "UI/Style/style.h"
#include "UI/Style/style_cascade.h"
#include "UI/Style/style_dimension.h"
#include "UI/Style/style_get_value.h"
#include "UI/Style/style_property_parser.h"
#include "UI/Style/style_set_image.h"
#include "UI/Style/style_set_value.h"
#include "UI/Style/style_token.h"
#include "UI/Style/style_tokenizer.h"
#include "UI/Style/style_value_type.h"
#include "UI/SystemDialogs/open_file_dialog.h"
#include "UI/SystemDialogs/save_file_dialog.h"
#include "UI/SystemDialogs/folder_browse_dialog.h"
#include "UI/View/focus_policy.h"
#include "UI/View/view.h"
#include "UI/View/view_geometry.h"
#include "UI/View/view_action.h"
#include "UI/View/cursor_description.h"
#include "UI/UIThread/ui_thread.h"
