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
**    Mark Page
*/


#pragma once

#include <memory>

namespace uicore
{
	class GraphicContext;
	class OpenGLTargetProvider;
	class OpenGLTarget_Impl;
	class OpenGLContextDescription;

	/// \brief Display target for uicoreDisplay.
	class OpenGLTarget
	{
	public:
		/// \brief Returns true if this display target is the current target
		///
		/// This may change after a display window has been created
		static bool is_current();

		static OpenGLContextDescription description();

		/// \brief Set this display target to be the current target
		static void set_current();

		static void set_description(OpenGLContextDescription &desc);

		/// \brief Get the opengl version
		///
		/// \param version_major = On Return: Major
		/// \param version_minor = On Return: Minor
		static void opengl_version(const std::shared_ptr<GraphicContext> &gc, int &version_major, int &version_minor);

		/// \brief Get the opengl version
		///
		/// \param version_major = On Return: Major
		/// \param version_minor = On Return: Minor
		/// \param version_release = On Return: Release
		static void opengl_version(const std::shared_ptr<GraphicContext> &gc, int &version_major, int &version_minor, int &version_release);

		/// \brief Get the opengl shading language version
		///
		/// \param version_major = On Return: Major
		/// \param version_minor = On Return: Minor
		static void opengl_shading_language_version(const std::shared_ptr<GraphicContext> &gc, int &version_major, int &version_minor);

		/// \brief Get the opengl renderer string
		static std::string renderer_string(const std::shared_ptr<GraphicContext> &gc);

		/// \brief Get the opengl vendor string
		static std::string vendor_string(const std::shared_ptr<GraphicContext> &gc);

		/// \brief Get the list of opengl extensions.
		static std::vector<std::string> extensions(const std::shared_ptr<GraphicContext> &gc);

		/// \brief Set OpenGL context used by this GraphicContext to be active
		static void set_active_context(const std::shared_ptr<GraphicContext> &gc);
	};
}
