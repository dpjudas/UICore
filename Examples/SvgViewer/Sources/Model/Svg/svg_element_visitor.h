
#pragma once

class SvgElementVisitor
{
public:
	void visit(const uicore::XmlNodePtr &e);

protected:
	virtual void a(const uicore::XmlNodePtr &e) { }
	virtual void altGlyph(const uicore::XmlNodePtr &e) { }
	virtual void altGlyphDef(const uicore::XmlNodePtr &e) { }
	virtual void altGlyphItem(const uicore::XmlNodePtr &e) { }
	virtual void animate(const uicore::XmlNodePtr &e) { }
	virtual void animateColor(const uicore::XmlNodePtr &e) { }
	virtual void animateMotion(const uicore::XmlNodePtr &e) { }
	virtual void animateTransform(const uicore::XmlNodePtr &e) { }
	virtual void circle(const uicore::XmlNodePtr &e) { }
	virtual void clipPath(const uicore::XmlNodePtr &e) { }
	virtual void color_profile(const uicore::XmlNodePtr &e) { }
	virtual void cursor(const uicore::XmlNodePtr &e) { }
	virtual void defs(const uicore::XmlNodePtr &e) { }
	virtual void desc(const uicore::XmlNodePtr &e) { }
	virtual void ellipse(const uicore::XmlNodePtr &e) { }
	virtual void feBlend(const uicore::XmlNodePtr &e) { }
	virtual void feColorMatrix(const uicore::XmlNodePtr &e) { }
	virtual void feComponentTransfer(const uicore::XmlNodePtr &e) { }
	virtual void feComposite(const uicore::XmlNodePtr &e) { }
	virtual void feConvolveMatrix(const uicore::XmlNodePtr &e) { }
	virtual void feDiffuseLighting(const uicore::XmlNodePtr &e) { }
	virtual void feDisplacementMap(const uicore::XmlNodePtr &e) { }
	virtual void feDistantLight(const uicore::XmlNodePtr &e) { }
	virtual void feFlood(const uicore::XmlNodePtr &e) { }
	virtual void feFuncA(const uicore::XmlNodePtr &e) { }
	virtual void feFuncB(const uicore::XmlNodePtr &e) { }
	virtual void feFuncG(const uicore::XmlNodePtr &e) { }
	virtual void feFuncR(const uicore::XmlNodePtr &e) { }
	virtual void feGaussianBlur(const uicore::XmlNodePtr &e) { }
	virtual void feImage(const uicore::XmlNodePtr &e) { }
	virtual void feMerge(const uicore::XmlNodePtr &e) { }
	virtual void feMergeNode(const uicore::XmlNodePtr &e) { }
	virtual void feMorphology(const uicore::XmlNodePtr &e) { }
	virtual void feOffset(const uicore::XmlNodePtr &e) { }
	virtual void fePointLight(const uicore::XmlNodePtr &e) { }
	virtual void feSpecularLighting(const uicore::XmlNodePtr &e) { }
	virtual void feSpotLight(const uicore::XmlNodePtr &e) { }
	virtual void feTile(const uicore::XmlNodePtr &e) { }
	virtual void feTurbulence(const uicore::XmlNodePtr &e) { }
	virtual void filter(const uicore::XmlNodePtr &e) { }
	virtual void font(const uicore::XmlNodePtr &e) { }
	virtual void font_face(const uicore::XmlNodePtr &e) { }
	virtual void font_face_format(const uicore::XmlNodePtr &e) { }
	virtual void font_face_name(const uicore::XmlNodePtr &e) { }
	virtual void font_face_src(const uicore::XmlNodePtr &e) { }
	virtual void font_face_uri(const uicore::XmlNodePtr &e) { }
	virtual void foreignObject(const uicore::XmlNodePtr &e) { }
	virtual void g(const uicore::XmlNodePtr &e) { }
	virtual void glyph(const uicore::XmlNodePtr &e) { }
	virtual void glyphRef(const uicore::XmlNodePtr &e) { }
	virtual void hkern(const uicore::XmlNodePtr &e) { }
	virtual void image(const uicore::XmlNodePtr &e) { }
	virtual void line(const uicore::XmlNodePtr &e) { }
	virtual void linearGradient(const uicore::XmlNodePtr &e) { }
	virtual void marker(const uicore::XmlNodePtr &e) { }
	virtual void mask(const uicore::XmlNodePtr &e) { }
	virtual void metadata(const uicore::XmlNodePtr &e) { }
	virtual void missing_glyph(const uicore::XmlNodePtr &e) { }
	virtual void mpath(const uicore::XmlNodePtr &e) { }
	virtual void path(const uicore::XmlNodePtr &e) { }
	virtual void pattern(const uicore::XmlNodePtr &e) { }
	virtual void polygon(const uicore::XmlNodePtr &e) { }
	virtual void polyline(const uicore::XmlNodePtr &e) { }
	virtual void radialGradient(const uicore::XmlNodePtr &e) { }
	virtual void rect(const uicore::XmlNodePtr &e) { }
	virtual void script(const uicore::XmlNodePtr &e) { }
	virtual void set(const uicore::XmlNodePtr &e) { }
	virtual void stop(const uicore::XmlNodePtr &e) { }
	virtual void style(const uicore::XmlNodePtr &e) { }
	virtual void svg(const uicore::XmlNodePtr &e) { }
	virtual void switch_(const uicore::XmlNodePtr &e) { }
	virtual void symbol(const uicore::XmlNodePtr &e) { }
	virtual void text(const uicore::XmlNodePtr &e) { }
	virtual void textPath(const uicore::XmlNodePtr &e) { }
	virtual void title(const uicore::XmlNodePtr &e) { }
	virtual void tref(const uicore::XmlNodePtr &e) { }
	virtual void tspan(const uicore::XmlNodePtr &e) { }
	virtual void use(const uicore::XmlNodePtr &e) { }
	virtual void view(const uicore::XmlNodePtr &e) { }
	virtual void vkern(const uicore::XmlNodePtr &e) { }
};
