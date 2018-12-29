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

#include "Core/Text/string_format.h"
#include "Core/Text/text.h"
#include "Core/Text/utf8_reader.h"
#include "Core/System/singleton_bugfix.h"
#include "Core/System/comptr.h"
#include "Core/System/databuffer.h"
#include "Core/System/datetime.h"
#include "Core/System/disposable_object.h"
#include "Core/System/exception.h"
#include "Core/System/service.h"
#include "Core/System/system.h"
#include "Core/System/registry_key.h"
#include "Core/ErrorReporting/crash_reporter.h"
#include "Core/ErrorReporting/exception_dialog.h"
#include "Core/Signals/signal.h"
#include "Core/IOData/endian.h"
#include "Core/IOData/iodevice.h"
#include "Core/IOData/memory_device.h"
#include "Core/IOData/file.h"
#include "Core/IOData/path_help.h"
#include "Core/IOData/directory.h"
#include "Core/IOData/directory_scanner.h"
#include "Core/Zip/zlib_compression.h"
#include "Core/Math/angle.h"
#include "Core/Math/base64_encoder.h"
#include "Core/Math/base64_decoder.h"
#include "Core/Math/circle.h"
#include "Core/Math/color.h"
#include "Core/Math/color_hsv.h"
#include "Core/Math/color_hsl.h"
#include "Core/Math/vec2.h"
#include "Core/Math/vec3.h"
#include "Core/Math/vec4.h"
#include "Core/Math/ear_clip_triangulator.h"
#include "Core/Math/line_math.h"
#include "Core/Math/cl_math.h"
#include "Core/Math/quaternion.h"
#include "Core/Math/mat2.h"
#include "Core/Math/mat3.h"
#include "Core/Math/mat4.h"
#include "Core/Math/origin.h"
#include "Core/Math/point.h"
#include "Core/Math/pointset_math.h"
#include "Core/Math/quad.h"
#include "Core/Math/rect.h"
#include "Core/Math/rect_packer.h"
#include "Core/Math/half_float.h"
#include "Core/Math/half_float_vector.h"
#include "Core/Math/big_int.h"
#include "Core/Math/frustum_planes.h"
#include "Core/Math/intersection_test.h"
#include "Core/Math/aabb.h"
#include "Core/Math/obb.h"
#include "Core/Math/easing.h"
#include "Core/Math/pi.h"
#include "Core/Math/size.h"
#include "Core/Math/triangle_math.h"
#include "Core/Math/line.h"
#include "Core/Math/line_ray.h"
#include "Core/Math/line_segment.h"
#include "Core/Json/json_value.h"
#include "Core/Xml/xml_document.h"
#include "Core/Xml/xml_node.h"
#include "Core/Xml/xml_tokenizer.h"
#include "Core/Xml/xml_writer.h"
#include "Core/Xml/xml_token.h"
