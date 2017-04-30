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

#include "UICore/Display/2D/path.h"
#include <vector>

namespace uicore
{
	enum class PathCommand
	{
		line,
		quadradic,
		cubic
	};

	class PathSubpath
	{
	public:
		PathSubpath() : points(1) { }

		std::vector<Pointf> points;
		std::vector<PathCommand> commands;
		bool closed = false;
	};

	class PathImpl : public Path
	{
	public:
		PathImpl();
		PathImpl(const PathImpl &other) { *this = other; }

		using Path::move_to;

		PathFillMode fill_mode() const override { return _fill_mode; }
		void set_fill_mode(PathFillMode fill_mode) override { _fill_mode = fill_mode; }

		void move_to(const Pointf &point) override;
		void line_to(const Pointf &point) override;
		void bezier_to(const Pointf &control, const Pointf &point) override;
		void bezier_to(const Pointf &control1, const Pointf &control2, const Pointf &point) override;
		void close() override;

		void add(const std::shared_ptr<Path> &path) override;
		void add_line(const Pointf &start, const Pointf &end) override;
		void add_rect(const Rectf &box) override;
		void add_rect(const Rectf &box, const uicore::Sizef &corner) override;
		void add_ellipse(const Pointf &center, const Sizef &radius) override;

		void apply_transform(const Mat3f &transform) override;

		void stroke(const std::shared_ptr<Canvas> &canvas, const Pen &pen) override;
		void fill(const std::shared_ptr<Canvas> &canvas, const Brush &brush) override;
		void fill_and_stroke(const std::shared_ptr<Canvas> &canvas, const Pen &pen, const Brush &brush) override;

		std::shared_ptr<Path> clone() const override;

		PathFillMode _fill_mode = PathFillMode::alternate;
		std::vector<PathSubpath> _subpaths;
	};
}
