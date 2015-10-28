
#include "precomp.h"
#include "svg_transform_scope.h"
#include "svg_attribute_reader.h"

using namespace uicore;

SvgTransformScope::SvgTransformScope(const CanvasPtr &canvas, const XmlNodePtr &e) : canvas(canvas)
{
	old_transform = canvas->transform();
	canvas->mult_transform(parse_transform(e, transform_active));
}

SvgTransformScope::SvgTransformScope(const CanvasPtr &canvas, const Mat4f &transform, bool transform_active) : canvas(canvas), transform_active(transform_active)
{
	old_transform = canvas->transform();
	canvas->mult_transform(transform);
}

SvgTransformScope::~SvgTransformScope()
{
	if (transform_active)
	{
		canvas->set_transform(old_transform);
	}
}

Mat4f SvgTransformScope::parse_transform(const XmlNodePtr &e, bool &transform_active)
{
	try
	{
		Mat4d mat = Mat4d::identity();

		SvgAttributeReader transform(e, "transform");
		while (!transform.is_end())
		{
			Mat4d t;
			if (transform.is_keyword("matrix"))
			{
				transform.get_keyword("matrix");
				transform.get_operator("(");
				t = Mat4d::identity();
				t.matrix[0] = transform.get_number();
				t.matrix[1] = transform.get_sequence_number();
				t.matrix[4] = transform.get_sequence_number();
				t.matrix[5] = transform.get_sequence_number();
				t.matrix[12] = transform.get_sequence_number();
				t.matrix[13] = transform.get_sequence_number();
				transform.get_operator(")");
			}
			else if (transform.is_keyword("translate"))
			{
				transform.get_keyword("translate");
				transform.get_operator("(");
				double tx = transform.get_number();
				if (transform.is_operator(",")) transform.get_operator(",");
				double ty = transform.is_number() ? transform.get_number() : 0.0;
				transform.get_operator(")");

				t = Mat4d::translate(tx, ty, 0.0);
			}
			else if (transform.is_keyword("scale"))
			{
				transform.get_keyword("scale");
				transform.get_operator("(");
				double sx = transform.get_number();
				if (transform.is_operator(",")) transform.get_operator(",");
				double sy = transform.is_number() ? transform.get_number() : sx;
				transform.get_operator(")");

				t = Mat4d::scale(sx, sy, 0.0);
			}
			else if (transform.is_keyword("rotate"))
			{
				transform.get_keyword("rotate");
				transform.get_operator("(");
				double a = transform.get_number() * PI / 180.0;

				t = Mat4d::identity();
				t.matrix[0] = std::cos(a);
				t.matrix[1] = std::sin(a);
				t.matrix[4] = -t.matrix[1];
				t.matrix[5] = -t.matrix[0];

				if (transform.is_operator(",")) transform.get_operator(",");
				if (transform.is_number())
				{
					double tx = transform.get_number();
					if (transform.is_operator(",")) transform.get_operator(",");
					double ty = transform.is_number() ? transform.get_number() : 0.0;
					t = Mat4d::translate(tx, ty, 0.0) * t * Mat4d::translate(-tx, -ty, 0.0);
				}

				transform.get_operator(")");
			}
			else if (transform.is_keyword("skewX"))
			{
				transform.get_keyword("skewX");
				transform.get_operator("(");
				double a = transform.get_number() * PI / 180.0;
				transform.get_operator(")");

				t = Mat4d::identity();
				t.matrix[4] = std::tan(a);
			}
			else if (transform.is_keyword("skewY"))
			{
				transform.get_keyword("skewY");
				transform.get_operator("(");
				double a = transform.get_number() * PI / 180.0;
				transform.get_operator(")");

				t = Mat4d::identity();
				t.matrix[1] = std::tan(a);
			}
			else
			{
				transform.parse_error("expected transform keyword");
			}

			mat = mat * t;

			if (transform.is_operator(","))
				transform.get_operator(",");
		}

		transform_active = true;
		return Mat4f(mat);
	}
	catch (Exception &)
	{
		transform_active = false;
		return Mat4f::identity();
	}
}
