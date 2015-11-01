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

#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include "UICore/core.h"
#include "UICore/display.h"
#ifdef WIN32
#include "UICore/d3d.h"
#endif
#include "UICore/gl.h"
#include "UICore/ui.h"
#include "UICore/network.h"
#include "UICore/application.h"

#if defined(_MSC_VER)
	#if !defined(_DEBUG)
		#if defined(DLL)
			#pragma comment(lib, "uicore-dll.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "uicore-static-mtdll.lib")
		#else
			#pragma comment(lib, "uicore-static-mt.lib")
		#endif
	#else
		#if defined(DLL)
			#pragma comment(lib, "uicore-dll-debug.lib")
		#elif defined(_DLL)
			#pragma comment(lib, "uicore-static-mtdll-debug.lib")
		#else
			#pragma comment(lib, "uicore-static-mt-debug.lib")
		#endif
	#endif
	#pragma comment(lib, "winmm.lib")
	#pragma comment(lib, "advapi32.lib")
	#pragma comment(lib, "gdi32.lib")
	#pragma comment(lib, "OpenGL32.lib")
	#pragma comment(lib, "GLU32.lib")
	#pragma comment(lib, "ws2_32.lib")
	#pragma comment(lib, "iphlpapi.lib")
#endif

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif
