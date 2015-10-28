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
#include "../../Core/Math/rect.h"
#include "../../Core/Math/mat4.h"
#include "../../Core/Math/color.h"

namespace uicore
{
	class Font;
	typedef std::shared_ptr<Font> FontPtr;
	class PathImpl;
	class GlyphMetrics;
	class Canvas;
	typedef std::shared_ptr<Canvas> CanvasPtr;
	class Pen;
	class Brush;

	enum class PathFillMode
	{
		alternate,
		winding
	};

	class Path
	{
	public:
		static std::shared_ptr<Path> create();

		static std::shared_ptr<Path> line(const Pointf &start, const Pointf &end) { auto path = create(); path->add_line(start, end); return path; }
		static std::shared_ptr<Path> line(float x1, float y1, float x2, float y2) { return Path::line(Pointf(x1, y1), Pointf(x2, y2)); }

		static std::shared_ptr<Path> rect(const Rectf &box) { auto path = create(); path->add_rect(box); return path; }
		static std::shared_ptr<Path> rect(float x, float y, float width, float height) { return Path::rect(Rectf(x, y, Sizef(width, height))); }
		static std::shared_ptr<Path> rect(const Rectf &box, const uicore::Sizef &corner) { auto path = create(); path->add_rect(box, corner); return path; }

		static std::shared_ptr<Path> circle(float center_x, float center_y, float radius) { return Path::ellipse(Pointf(center_x, center_y), Sizef(radius, radius)); }
		static std::shared_ptr<Path> ellipse(float center_x, float center_y, float radius_x, float radius_y) { return Path::ellipse(Pointf(center_x, center_y), Sizef(radius_x, radius_y)); }
		static std::shared_ptr<Path> circle(const Pointf &center, float radius) { return Path::ellipse(center, Sizef(radius, radius)); }
		static std::shared_ptr<Path> ellipse(const Pointf &center, const Sizef &radius) { auto path = create(); path->add_ellipse(center, radius); return path; }

		// This function is to assist in debugging, it has not been decided if it will be removed. Don't use at the moment.
		static std::shared_ptr<Path> glyph(const CanvasPtr &canvas, const FontPtr &font, unsigned int glyph, GlyphMetrics &out_metrics);

		virtual PathFillMode fill_mode() const = 0;
		virtual void set_fill_mode(PathFillMode fill_mode) = 0;

		virtual void move_to(const Pointf &point) = 0;
		void move_to(float x, float y) { move_to(Pointf(x, y)); }
		virtual void line_to(const Pointf &point) = 0;
		void line_to(float x, float y) { line_to(Pointf(x, y)); }
		virtual void bezier_to(const Pointf &control, const Pointf &point) = 0;
		virtual void bezier_to(const Pointf &control1, const Pointf &control2, const Pointf &point) = 0;
		virtual void close() = 0;

		/// \brief Add all subpaths in path to this path
		virtual void add(const std::shared_ptr<Path> &path) = 0;

		virtual void add_line(const Pointf &start, const Pointf &end) = 0;
		void add_line(float x1, float y1, float x2, float y2) { return add_line(Pointf(x1, y1), Pointf(x2, y2)); }

		virtual void add_rect(const Rectf &box) = 0;
		void add_rect(float x, float y, float width, float height) { add_rect(Rectf(x, y, Sizef(width, height))); }
		virtual void add_rect(const Rectf &box, const uicore::Sizef &corner) = 0;

		void add_circle(float center_x, float center_y, float radius) { add_ellipse(Pointf(center_x, center_y), Sizef(radius, radius)); }
		void add_ellipse(float center_x, float center_y, float radius_x, float radius_y) { add_ellipse(Pointf(center_x, center_y), Sizef(radius_x, radius_y)); }
		void add_circle(const Pointf &center, float radius) { add_ellipse(center, Sizef(radius, radius)); }
		virtual void add_ellipse(const Pointf &center, const Sizef &radius) = 0;

		/// \brief Apply a transformation matrix on all points in this path
		virtual void apply_transform(const Mat3f &transform) = 0;

		/// \brief Strokes a path
		virtual void stroke(const CanvasPtr &canvas, const Pen &pen) = 0;

		/// \brief Fills a path
		virtual void fill(const CanvasPtr &canvas, const Brush &brush) = 0;

		/// \brief First fills a path, then strokes on top
		virtual void fill_and_stroke(const CanvasPtr &canvas, const Pen &pen, const Brush &brush) = 0;

		// \brief Make a copy of the path
		virtual std::shared_ptr<Path> clone() const = 0;
	};

	typedef std::shared_ptr<Path> PathPtr;
}
