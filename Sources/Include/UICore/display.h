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
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Kenneth Gangstoe
**    Harry Storbacka
*/

#pragma once

#include "Display/display_target.h"
#include "Display/screen_info.h"
#include "Display/2D/canvas.h"
#include "Display/2D/color.h"
#include "Display/2D/color_hsv.h"
#include "Display/2D/color_hsl.h"
#include "Display/2D/gradient.h"
#include "Display/2D/image.h"
#include "Display/2D/path.h"
#include "Display/2D/pen.h"
#include "Display/2D/brush.h"
#include "Display/2D/subtexture.h"
#include "Display/2D/texture_group.h"
#include "Display/2D/span_layout.h"
#include "Display/System/run_loop.h"
#include "Display/System/timer.h"
#include "Display/System/detect_hang.h"
#include "Display/Font/font_family.h"
#include "Display/Font/font.h"
#include "Display/Font/font_description.h"
#include "Display/Font/font_metrics.h"
#include "Display/Font/glyph_metrics.h"
#include "Display/Image/pixel_buffer.h"
#include "Display/Image/pixel_buffer_lock.h"
#include "Display/Image/pixel_buffer_help.h"
#include "Display/Image/pixel_buffer_set.h"
#include "Display/Image/icon_set.h"
#include "Display/Image/perlin_noise.h"
#include "Display/Image/image_import_description.h"
#include "Display/Image/pixel_converter.h"
#include "Display/ImageFormats/jpeg_format.h"
#include "Display/ImageFormats/png_format.h"
#include "Display/ImageFormats/image_file.h"
#include "Display/ImageFormats/image_file_type.h"
#include "Display/ImageFormats/image_file_type_register.h"
#include "Display/ImageFormats/targa_format.h"
#include "Display/ImageFormats/dds_format.h"
#include "Display/Render/blend_state_description.h"
#include "Display/Render/depth_stencil_state_description.h"
#include "Display/Render/rasterizer_state_description.h"
#include "Display/Render/element_array_buffer.h"
#include "Display/Render/element_array_vector.h"
#include "Display/Render/transfer_buffer.h"
#include "Display/Render/transfer_vector.h"
#include "Display/Render/frame_buffer.h"
#include "Display/Render/graphic_context.h"
#include "Display/Render/occlusion_query.h"
#include "Display/Render/primitives_array.h"
#include "Display/Render/program_object.h"
#include "Display/Render/uniform_buffer.h"
#include "Display/Render/uniform_vector.h"
#include "Display/Render/storage_buffer.h"
#include "Display/Render/storage_vector.h"
#include "Display/Render/render_batcher.h"
#include "Display/Render/render_buffer.h"
#include "Display/Render/shader_object.h"
#include "Display/Render/shared_gc_data.h"
#include "Display/Render/texture.h"
#include "Display/Render/transfer_texture.h"
#include "Display/Render/texture_1d.h"
#include "Display/Render/texture_1d_array.h"
#include "Display/Render/texture_2d.h"
#include "Display/Render/texture_2d_array.h"
#include "Display/Render/texture_3d.h"
#include "Display/Render/texture_cube.h"
#include "Display/Render/texture_cube_array.h"
#include "Display/Render/vertex_array_buffer.h"
#include "Display/Render/vertex_array_vector.h"
#include "Display/Window/cursor.h"
#include "Display/Window/cursor_description.h"
#include "Display/Window/display_window.h"
#include "Display/Window/display_window_description.h"
#include "Display/Window/input_code.h"
#include "Display/Window/input_device.h"
#include "Display/Window/input_event.h"
#include "Display/Window/keys.h"
