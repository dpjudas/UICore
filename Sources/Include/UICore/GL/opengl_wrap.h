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

#include "opengl.h"
#include "opengl_defines.h"

namespace uicore
{
	#ifdef WIN32
	#define GLFUNC WINAPI
	#else
	#define GLFUNC
	#endif

	/// \brief GLFunctions
	class GLFunctions
	{
	public:
		typedef void (GLFUNC *ptr_glCullFace)(GLenum mode);
		typedef void (GLFUNC *ptr_glFrontFace)(GLenum mode);
		typedef void (GLFUNC *ptr_glHint)(GLenum target, GLenum mode);
		typedef void (GLFUNC *ptr_glLineWidth)(GLfloat width);
		typedef void (GLFUNC *ptr_glPointSize)(GLfloat size);
		typedef void (GLFUNC *ptr_glPolygonMode)(GLenum face, GLenum mode);
		typedef void (GLFUNC *ptr_glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
		typedef void (GLFUNC *ptr_glTexParameterf)(GLenum target, GLenum pname, GLfloat param);
		typedef void (GLFUNC *ptr_glTexParameterfv)(GLenum target, GLenum pname, const GLfloat *params);
		typedef void (GLFUNC *ptr_glTexParameteri)(GLenum target, GLenum pname, GLint param);
		typedef void (GLFUNC *ptr_glTexParameteriv)(GLenum target, GLenum pname, const GLint *params);
		typedef void (GLFUNC *ptr_glTexImage1D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
		typedef void (GLFUNC *ptr_glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
		typedef void (GLFUNC *ptr_glDrawBuffer)(GLenum mode);
		typedef void (GLFUNC *ptr_glClear)(GLbitfield mask);
		typedef void (GLFUNC *ptr_glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
		typedef void (GLFUNC *ptr_glClearStencil)(GLint s);
		typedef void (GLFUNC *ptr_glClearDepth)(GLdouble depth);
		typedef void (GLFUNC *ptr_glStencilMask)(GLuint mask);
		typedef void (GLFUNC *ptr_glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
		typedef void (GLFUNC *ptr_glDepthMask)(GLboolean flag);
		typedef void (GLFUNC *ptr_glDisable)(GLenum cap);
		typedef void (GLFUNC *ptr_glEnable)(GLenum cap);
		typedef void (GLFUNC *ptr_glFinish)(void);
		typedef void (GLFUNC *ptr_glFlush)(void);
		typedef void (GLFUNC *ptr_glBlendFunc)(GLenum sfactor, GLenum dfactor);
		typedef void (GLFUNC *ptr_glLogicOp)(GLenum opcode);
		typedef void (GLFUNC *ptr_glStencilFunc)(GLenum func, GLint ref, GLuint mask);
		typedef void (GLFUNC *ptr_glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
		typedef void (GLFUNC *ptr_glDepthFunc)(GLenum func);
		typedef void (GLFUNC *ptr_glPixelStoref)(GLenum pname, GLfloat param);
		typedef void (GLFUNC *ptr_glPixelStorei)(GLenum pname, GLint param);
		typedef void (GLFUNC *ptr_glReadBuffer)(GLenum mode);
		typedef void (GLFUNC *ptr_glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
		typedef void (GLFUNC *ptr_glGetBooleanv)(GLenum pname, GLboolean *params);
		typedef void (GLFUNC *ptr_glGetDoublev)(GLenum pname, GLdouble *params);
		typedef GLenum (GLFUNC *ptr_glGetError)(void);
		typedef void (GLFUNC *ptr_glGetFloatv)(GLenum pname, GLfloat *params);
		typedef void (GLFUNC *ptr_glGetIntegerv)(GLenum pname, GLint *params);
		typedef const GLubyte * (GLFUNC *ptr_glGetString)(GLenum name);
		typedef void (GLFUNC *ptr_glGetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
		typedef void (GLFUNC *ptr_glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
		typedef void (GLFUNC *ptr_glGetTexParameteriv)(GLenum target, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat *params);
		typedef void (GLFUNC *ptr_glGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint *params);
		typedef GLboolean (GLFUNC *ptr_glIsEnabled)(GLenum cap);
		typedef void (GLFUNC *ptr_glDepthRange)(GLdouble near, GLdouble far);
		typedef void (GLFUNC *ptr_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
		typedef void (GLFUNC *ptr_glDrawArrays)(GLenum mode, GLint first, GLsizei count);
		typedef void (GLFUNC *ptr_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
		typedef void (GLFUNC *ptr_glGetPointerv)(GLenum pname, GLvoid* *params);
		typedef void (GLFUNC *ptr_glPolygonOffset)(GLfloat factor, GLfloat units);
		typedef void (GLFUNC *ptr_glCopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
		typedef void (GLFUNC *ptr_glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
		typedef void (GLFUNC *ptr_glCopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
		typedef void (GLFUNC *ptr_glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
		typedef void (GLFUNC *ptr_glTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
		typedef void (GLFUNC *ptr_glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
		typedef void (GLFUNC *ptr_glBindTexture)(GLenum target, GLuint texture);
		typedef void (GLFUNC *ptr_glDeleteTextures)(GLsizei n, const GLuint *textures);
		typedef void (GLFUNC *ptr_glGenTextures)(GLsizei n, GLuint *textures);
		typedef GLboolean (GLFUNC *ptr_glIsTexture)(GLuint texture);
		typedef void (GLFUNC *ptr_glBlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
		typedef void (GLFUNC *ptr_glBlendEquation)(GLenum mode);
		typedef void (GLFUNC *ptr_glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
		typedef void (GLFUNC *ptr_glTexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
		typedef void (GLFUNC *ptr_glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
		typedef void (GLFUNC *ptr_glCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
		typedef void (GLFUNC *ptr_glActiveTexture)(GLenum texture);
		typedef void (GLFUNC *ptr_glSampleCoverage)(GLfloat value, GLboolean invert);
		typedef void (GLFUNC *ptr_glCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
		typedef void (GLFUNC *ptr_glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
		typedef void (GLFUNC *ptr_glCompressedTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
		typedef void (GLFUNC *ptr_glCompressedTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
		typedef void (GLFUNC *ptr_glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
		typedef void (GLFUNC *ptr_glCompressedTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
		typedef void (GLFUNC *ptr_glGetCompressedTexImage)(GLenum target, GLint level, GLvoid *img);
		typedef void (GLFUNC *ptr_glBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
		typedef void (GLFUNC *ptr_glMultiDrawArrays)(GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount);
		typedef void (GLFUNC *ptr_glMultiDrawElements)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid* const *indices, GLsizei drawcount);
		typedef void (GLFUNC *ptr_glPointParameterf)(GLenum pname, GLfloat param);
		typedef void (GLFUNC *ptr_glPointParameterfv)(GLenum pname, const GLfloat *params);
		typedef void (GLFUNC *ptr_glPointParameteri)(GLenum pname, GLint param);
		typedef void (GLFUNC *ptr_glPointParameteriv)(GLenum pname, const GLint *params);
		typedef void (GLFUNC *ptr_glGenQueries)(GLsizei n, GLuint *ids);
		typedef void (GLFUNC *ptr_glDeleteQueries)(GLsizei n, const GLuint *ids);
		typedef GLboolean (GLFUNC *ptr_glIsQuery)(GLuint id);
		typedef void (GLFUNC *ptr_glBeginQuery)(GLenum target, GLuint id);
		typedef void (GLFUNC *ptr_glEndQuery)(GLenum target);
		typedef void (GLFUNC *ptr_glGetQueryiv)(GLenum target, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGetQueryObjectiv)(GLuint id, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint *params);
		typedef void (GLFUNC *ptr_glBindBuffer)(GLenum target, GLuint buffer);
		typedef void (GLFUNC *ptr_glDeleteBuffers)(GLsizei n, const GLuint *buffers);
		typedef void (GLFUNC *ptr_glGenBuffers)(GLsizei n, GLuint *buffers);
		typedef GLboolean (GLFUNC *ptr_glIsBuffer)(GLuint buffer);
		typedef void (GLFUNC *ptr_glBufferData)(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
		typedef void (GLFUNC *ptr_glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
		typedef void (GLFUNC *ptr_glGetBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, GLvoid *data);
		typedef GLvoid* (GLFUNC *ptr_glMapBuffer)(GLenum target, GLenum access);
		typedef GLboolean (GLFUNC *ptr_glUnmapBuffer)(GLenum target);
		typedef void (GLFUNC *ptr_glGetBufferParameteriv)(GLenum target, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGetBufferPointerv)(GLenum target, GLenum pname, GLvoid* *params);
		typedef void (GLFUNC *ptr_glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
		typedef void (GLFUNC *ptr_glDrawBuffers)(GLsizei n, const GLenum *bufs);
		typedef void (GLFUNC *ptr_glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
		typedef void (GLFUNC *ptr_glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);
		typedef void (GLFUNC *ptr_glStencilMaskSeparate)(GLenum face, GLuint mask);
		typedef void (GLFUNC *ptr_glAttachShader)(GLuint program, GLuint shader);
		typedef void (GLFUNC *ptr_glBindAttribLocation)(GLuint program, GLuint index, const GLchar *name);
		typedef void (GLFUNC *ptr_glCompileShader)(GLuint shader);
		typedef GLuint (GLFUNC *ptr_glCreateProgram)(void);
		typedef GLuint (GLFUNC *ptr_glCreateShader)(GLenum type);
		typedef void (GLFUNC *ptr_glDeleteProgram)(GLuint program);
		typedef void (GLFUNC *ptr_glDeleteShader)(GLuint shader);
		typedef void (GLFUNC *ptr_glDetachShader)(GLuint program, GLuint shader);
		typedef void (GLFUNC *ptr_glDisableVertexAttribArray)(GLuint index);
		typedef void (GLFUNC *ptr_glEnableVertexAttribArray)(GLuint index);
		typedef void (GLFUNC *ptr_glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
		typedef void (GLFUNC *ptr_glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
		typedef void (GLFUNC *ptr_glGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *obj);
		typedef GLint (GLFUNC *ptr_glGetAttribLocation)(GLuint program, const GLchar *name);
		typedef void (GLFUNC *ptr_glGetProgramiv)(GLuint program, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
		typedef void (GLFUNC *ptr_glGetShaderiv)(GLuint shader, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
		typedef void (GLFUNC *ptr_glGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
		typedef GLint (GLFUNC *ptr_glGetUniformLocation)(GLuint program, const GLchar *name);
		typedef void (GLFUNC *ptr_glGetUniformfv)(GLuint program, GLint location, GLfloat *params);
		typedef void (GLFUNC *ptr_glGetUniformiv)(GLuint program, GLint location, GLint *params);
		typedef void (GLFUNC *ptr_glGetVertexAttribdv)(GLuint index, GLenum pname, GLdouble *params);
		typedef void (GLFUNC *ptr_glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat *params);
		typedef void (GLFUNC *ptr_glGetVertexAttribiv)(GLuint index, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGetVertexAttribPointerv)(GLuint index, GLenum pname, GLvoid* *pointer);
		typedef GLboolean (GLFUNC *ptr_glIsProgram)(GLuint program);
		typedef GLboolean (GLFUNC *ptr_glIsShader)(GLuint shader);
		typedef void (GLFUNC *ptr_glLinkProgram)(GLuint program);
		typedef void (GLFUNC *ptr_glShaderSource)(GLuint shader, GLsizei count, const GLchar* const *string, const GLint *length);
		typedef void (GLFUNC *ptr_glUseProgram)(GLuint program);
		typedef void (GLFUNC *ptr_glUniform1f)(GLint location, GLfloat v0);
		typedef void (GLFUNC *ptr_glUniform2f)(GLint location, GLfloat v0, GLfloat v1);
		typedef void (GLFUNC *ptr_glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
		typedef void (GLFUNC *ptr_glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		typedef void (GLFUNC *ptr_glUniform1i)(GLint location, GLint v0);
		typedef void (GLFUNC *ptr_glUniform2i)(GLint location, GLint v0, GLint v1);
		typedef void (GLFUNC *ptr_glUniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
		typedef void (GLFUNC *ptr_glUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
		typedef void (GLFUNC *ptr_glUniform1fv)(GLint location, GLsizei count, const GLfloat *value);
		typedef void (GLFUNC *ptr_glUniform2fv)(GLint location, GLsizei count, const GLfloat *value);
		typedef void (GLFUNC *ptr_glUniform3fv)(GLint location, GLsizei count, const GLfloat *value);
		typedef void (GLFUNC *ptr_glUniform4fv)(GLint location, GLsizei count, const GLfloat *value);
		typedef void (GLFUNC *ptr_glUniform1iv)(GLint location, GLsizei count, const GLint *value);
		typedef void (GLFUNC *ptr_glUniform2iv)(GLint location, GLsizei count, const GLint *value);
		typedef void (GLFUNC *ptr_glUniform3iv)(GLint location, GLsizei count, const GLint *value);
		typedef void (GLFUNC *ptr_glUniform4iv)(GLint location, GLsizei count, const GLint *value);
		typedef void (GLFUNC *ptr_glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glValidateProgram)(GLuint program);
		typedef void (GLFUNC *ptr_glVertexAttrib1d)(GLuint index, GLdouble x);
		typedef void (GLFUNC *ptr_glVertexAttrib1dv)(GLuint index, const GLdouble *v);
		typedef void (GLFUNC *ptr_glVertexAttrib1f)(GLuint index, GLfloat x);
		typedef void (GLFUNC *ptr_glVertexAttrib1fv)(GLuint index, const GLfloat *v);
		typedef void (GLFUNC *ptr_glVertexAttrib1s)(GLuint index, GLshort x);
		typedef void (GLFUNC *ptr_glVertexAttrib1sv)(GLuint index, const GLshort *v);
		typedef void (GLFUNC *ptr_glVertexAttrib2d)(GLuint index, GLdouble x, GLdouble y);
		typedef void (GLFUNC *ptr_glVertexAttrib2dv)(GLuint index, const GLdouble *v);
		typedef void (GLFUNC *ptr_glVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y);
		typedef void (GLFUNC *ptr_glVertexAttrib2fv)(GLuint index, const GLfloat *v);
		typedef void (GLFUNC *ptr_glVertexAttrib2s)(GLuint index, GLshort x, GLshort y);
		typedef void (GLFUNC *ptr_glVertexAttrib2sv)(GLuint index, const GLshort *v);
		typedef void (GLFUNC *ptr_glVertexAttrib3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
		typedef void (GLFUNC *ptr_glVertexAttrib3dv)(GLuint index, const GLdouble *v);
		typedef void (GLFUNC *ptr_glVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
		typedef void (GLFUNC *ptr_glVertexAttrib3fv)(GLuint index, const GLfloat *v);
		typedef void (GLFUNC *ptr_glVertexAttrib3s)(GLuint index, GLshort x, GLshort y, GLshort z);
		typedef void (GLFUNC *ptr_glVertexAttrib3sv)(GLuint index, const GLshort *v);
		typedef void (GLFUNC *ptr_glVertexAttrib4Nbv)(GLuint index, const GLbyte *v);
		typedef void (GLFUNC *ptr_glVertexAttrib4Niv)(GLuint index, const GLint *v);
		typedef void (GLFUNC *ptr_glVertexAttrib4Nsv)(GLuint index, const GLshort *v);
		typedef void (GLFUNC *ptr_glVertexAttrib4Nub)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
		typedef void (GLFUNC *ptr_glVertexAttrib4Nubv)(GLuint index, const GLubyte *v);
		typedef void (GLFUNC *ptr_glVertexAttrib4Nuiv)(GLuint index, const GLuint *v);
		typedef void (GLFUNC *ptr_glVertexAttrib4Nusv)(GLuint index, const GLushort *v);
		typedef void (GLFUNC *ptr_glVertexAttrib4bv)(GLuint index, const GLbyte *v);
		typedef void (GLFUNC *ptr_glVertexAttrib4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
		typedef void (GLFUNC *ptr_glVertexAttrib4dv)(GLuint index, const GLdouble *v);
		typedef void (GLFUNC *ptr_glVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		typedef void (GLFUNC *ptr_glVertexAttrib4fv)(GLuint index, const GLfloat *v);
		typedef void (GLFUNC *ptr_glVertexAttrib4iv)(GLuint index, const GLint *v);
		typedef void (GLFUNC *ptr_glVertexAttrib4s)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
		typedef void (GLFUNC *ptr_glVertexAttrib4sv)(GLuint index, const GLshort *v);
		typedef void (GLFUNC *ptr_glVertexAttrib4ubv)(GLuint index, const GLubyte *v);
		typedef void (GLFUNC *ptr_glVertexAttrib4uiv)(GLuint index, const GLuint *v);
		typedef void (GLFUNC *ptr_glVertexAttrib4usv)(GLuint index, const GLushort *v);
		typedef void (GLFUNC *ptr_glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
		typedef void (GLFUNC *ptr_glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glColorMaski)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
		typedef void (GLFUNC *ptr_glGetBooleani_v)(GLenum target, GLuint index, GLboolean *data);
		typedef void (GLFUNC *ptr_glGetIntegeri_v)(GLenum target, GLuint index, GLint *data);
		typedef void (GLFUNC *ptr_glEnablei)(GLenum target, GLuint index);
		typedef void (GLFUNC *ptr_glDisablei)(GLenum target, GLuint index);
		typedef GLboolean (GLFUNC *ptr_glIsEnabledi)(GLenum target, GLuint index);
		typedef void (GLFUNC *ptr_glBeginTransformFeedback)(GLenum primitiveMode);
		typedef void (GLFUNC *ptr_glEndTransformFeedback)(void);
		typedef void (GLFUNC *ptr_glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
		typedef void (GLFUNC *ptr_glBindBufferBase)(GLenum target, GLuint index, GLuint buffer);
		typedef void (GLFUNC *ptr_glTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLchar* const *varyings, GLenum bufferMode);
		typedef void (GLFUNC *ptr_glGetTransformFeedbackVarying)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
		typedef void (GLFUNC *ptr_glClampColor)(GLenum target, GLenum clamp);
		typedef void (GLFUNC *ptr_glBeginConditionalRender)(GLuint id, GLenum mode);
		typedef void (GLFUNC *ptr_glEndConditionalRender)(void);
		typedef void (GLFUNC *ptr_glVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
		typedef void (GLFUNC *ptr_glGetVertexAttribIiv)(GLuint index, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint *params);
		typedef void (GLFUNC *ptr_glVertexAttribI1i)(GLuint index, GLint x);
		typedef void (GLFUNC *ptr_glVertexAttribI2i)(GLuint index, GLint x, GLint y);
		typedef void (GLFUNC *ptr_glVertexAttribI3i)(GLuint index, GLint x, GLint y, GLint z);
		typedef void (GLFUNC *ptr_glVertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w);
		typedef void (GLFUNC *ptr_glVertexAttribI1ui)(GLuint index, GLuint x);
		typedef void (GLFUNC *ptr_glVertexAttribI2ui)(GLuint index, GLuint x, GLuint y);
		typedef void (GLFUNC *ptr_glVertexAttribI3ui)(GLuint index, GLuint x, GLuint y, GLuint z);
		typedef void (GLFUNC *ptr_glVertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
		typedef void (GLFUNC *ptr_glVertexAttribI1iv)(GLuint index, const GLint *v);
		typedef void (GLFUNC *ptr_glVertexAttribI2iv)(GLuint index, const GLint *v);
		typedef void (GLFUNC *ptr_glVertexAttribI3iv)(GLuint index, const GLint *v);
		typedef void (GLFUNC *ptr_glVertexAttribI4iv)(GLuint index, const GLint *v);
		typedef void (GLFUNC *ptr_glVertexAttribI1uiv)(GLuint index, const GLuint *v);
		typedef void (GLFUNC *ptr_glVertexAttribI2uiv)(GLuint index, const GLuint *v);
		typedef void (GLFUNC *ptr_glVertexAttribI3uiv)(GLuint index, const GLuint *v);
		typedef void (GLFUNC *ptr_glVertexAttribI4uiv)(GLuint index, const GLuint *v);
		typedef void (GLFUNC *ptr_glVertexAttribI4bv)(GLuint index, const GLbyte *v);
		typedef void (GLFUNC *ptr_glVertexAttribI4sv)(GLuint index, const GLshort *v);
		typedef void (GLFUNC *ptr_glVertexAttribI4ubv)(GLuint index, const GLubyte *v);
		typedef void (GLFUNC *ptr_glVertexAttribI4usv)(GLuint index, const GLushort *v);
		typedef void (GLFUNC *ptr_glGetUniformuiv)(GLuint program, GLint location, GLuint *params);
		typedef void (GLFUNC *ptr_glBindFragDataLocation)(GLuint program, GLuint color, const GLchar *name);
		typedef GLint (GLFUNC *ptr_glGetFragDataLocation)(GLuint program, const GLchar *name);
		typedef void (GLFUNC *ptr_glUniform1ui)(GLint location, GLuint v0);
		typedef void (GLFUNC *ptr_glUniform2ui)(GLint location, GLuint v0, GLuint v1);
		typedef void (GLFUNC *ptr_glUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);
		typedef void (GLFUNC *ptr_glUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
		typedef void (GLFUNC *ptr_glUniform1uiv)(GLint location, GLsizei count, const GLuint *value);
		typedef void (GLFUNC *ptr_glUniform2uiv)(GLint location, GLsizei count, const GLuint *value);
		typedef void (GLFUNC *ptr_glUniform3uiv)(GLint location, GLsizei count, const GLuint *value);
		typedef void (GLFUNC *ptr_glUniform4uiv)(GLint location, GLsizei count, const GLuint *value);
		typedef void (GLFUNC *ptr_glTexParameterIiv)(GLenum target, GLenum pname, const GLint *params);
		typedef void (GLFUNC *ptr_glTexParameterIuiv)(GLenum target, GLenum pname, const GLuint *params);
		typedef void (GLFUNC *ptr_glGetTexParameterIiv)(GLenum target, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGetTexParameterIuiv)(GLenum target, GLenum pname, GLuint *params);
		typedef void (GLFUNC *ptr_glClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint *value);
		typedef void (GLFUNC *ptr_glClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint *value);
		typedef void (GLFUNC *ptr_glClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat *value);
		typedef void (GLFUNC *ptr_glClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
		typedef const GLubyte * (GLFUNC *ptr_glGetStringi)(GLenum name, GLuint index);
		typedef void (GLFUNC *ptr_glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
		typedef void (GLFUNC *ptr_glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei instancecount);
		typedef void (GLFUNC *ptr_glTexBuffer)(GLenum target, GLenum internalformat, GLuint buffer);
		typedef void (GLFUNC *ptr_glPrimitiveRestartIndex)(GLuint index);
		typedef void (GLFUNC *ptr_glGetInteger64i_v)(GLenum target, GLuint index, CLint64 *data);
		typedef void (GLFUNC *ptr_glGetBufferParameteri64v)(GLenum target, GLenum pname, CLint64 *params);
		typedef void (GLFUNC *ptr_glFramebufferTexture)(GLenum target, GLenum attachment, GLuint texture, GLint level);
		typedef void (GLFUNC *ptr_glVertexAttribDivisor)(GLuint index, GLuint divisor);
		typedef void (GLFUNC *ptr_glMinSampleShading)(GLfloat value);
		typedef void (GLFUNC *ptr_glBlendEquationi)(GLuint buf, GLenum mode);
		typedef void (GLFUNC *ptr_glBlendEquationSeparatei)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
		typedef void (GLFUNC *ptr_glBlendFunci)(GLuint buf, GLenum src, GLenum dst);
		typedef void (GLFUNC *ptr_glBlendFuncSeparatei)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
		typedef GLboolean (GLFUNC *ptr_glIsRenderbuffer)(GLuint renderbuffer);
		typedef void (GLFUNC *ptr_glBindRenderbuffer)(GLenum target, GLuint renderbuffer);
		typedef void (GLFUNC *ptr_glDeleteRenderbuffers)(GLsizei n, const GLuint *renderbuffers);
		typedef void (GLFUNC *ptr_glGenRenderbuffers)(GLsizei n, GLuint *renderbuffers);
		typedef void (GLFUNC *ptr_glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
		typedef void (GLFUNC *ptr_glGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint *params);
		typedef GLboolean (GLFUNC *ptr_glIsFramebuffer)(GLuint framebuffer);
		typedef void (GLFUNC *ptr_glBindFramebuffer)(GLenum target, GLuint framebuffer);
		typedef void (GLFUNC *ptr_glDeleteFramebuffers)(GLsizei n, const GLuint *framebuffers);
		typedef void (GLFUNC *ptr_glGenFramebuffers)(GLsizei n, GLuint *framebuffers);
		typedef GLenum (GLFUNC *ptr_glCheckFramebufferStatus)(GLenum target);
		typedef void (GLFUNC *ptr_glFramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
		typedef void (GLFUNC *ptr_glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
		typedef void (GLFUNC *ptr_glFramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
		typedef void (GLFUNC *ptr_glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
		typedef void (GLFUNC *ptr_glGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGenerateMipmap)(GLenum target);
		typedef void (GLFUNC *ptr_glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
		typedef void (GLFUNC *ptr_glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
		typedef void (GLFUNC *ptr_glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
		typedef GLvoid* (GLFUNC *ptr_glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
		typedef void (GLFUNC *ptr_glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);
		typedef void (GLFUNC *ptr_glBindVertexArray)(GLuint array);
		typedef void (GLFUNC *ptr_glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
		typedef void (GLFUNC *ptr_glGenVertexArrays)(GLsizei n, GLuint *arrays);
		typedef GLboolean (GLFUNC *ptr_glIsVertexArray)(GLuint array);
		typedef void (GLFUNC *ptr_glGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar* const *uniformNames, GLuint *uniformIndices);
		typedef void (GLFUNC *ptr_glGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGetActiveUniformName)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
		typedef GLuint (GLFUNC *ptr_glGetUniformBlockIndex)(GLuint program, const GLchar *uniformBlockName);
		typedef void (GLFUNC *ptr_glGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
		typedef void (GLFUNC *ptr_glUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
		typedef void (GLFUNC *ptr_glCopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
		typedef void (GLFUNC *ptr_glDrawElementsBaseVertex)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
		typedef void (GLFUNC *ptr_glDrawRangeElementsBaseVertex)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices, GLint basevertex);
		typedef void (GLFUNC *ptr_glDrawElementsInstancedBaseVertex)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei instancecount, GLint basevertex);
		typedef void (GLFUNC *ptr_glMultiDrawElementsBaseVertex)(GLenum mode, const GLsizei *count, GLenum type, const GLvoid* const *indices, GLsizei drawcount, const GLint *basevertex);
		typedef void (GLFUNC *ptr_glProvokingVertex)(GLenum mode);
		typedef CLsync (GLFUNC *ptr_glFenceSync)(GLenum condition, GLbitfield flags);
		typedef GLboolean (GLFUNC *ptr_glIsSync)(CLsync sync);
		typedef void (GLFUNC *ptr_glDeleteSync)(CLsync sync);
		typedef GLenum (GLFUNC *ptr_glClientWaitSync)(CLsync sync, GLbitfield flags, CLuint64 timeout);
		typedef void (GLFUNC *ptr_glWaitSync)(CLsync sync, GLbitfield flags, CLuint64 timeout);
		typedef void (GLFUNC *ptr_glGetInteger64v)(GLenum pname, CLint64 *params);
		typedef void (GLFUNC *ptr_glGetSynciv)(CLsync sync, GLenum pname, GLsizei bufSize, GLsizei *length, GLint *values);
		typedef void (GLFUNC *ptr_glTexImage2DMultisample)(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
		typedef void (GLFUNC *ptr_glTexImage3DMultisample)(GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
		typedef void (GLFUNC *ptr_glGetMultisamplefv)(GLenum pname, GLuint index, GLfloat *val);
		typedef void (GLFUNC *ptr_glSampleMaski)(GLuint index, GLbitfield mask);
		typedef void (GLFUNC *ptr_glBlendEquationiARB)(GLuint buf, GLenum mode);
		typedef void (GLFUNC *ptr_glBlendEquationSeparateiARB)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
		typedef void (GLFUNC *ptr_glBlendFunciARB)(GLuint buf, GLenum src, GLenum dst);
		typedef void (GLFUNC *ptr_glBlendFuncSeparateiARB)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
		typedef void (GLFUNC *ptr_glMinSampleShadingARB)(GLfloat value);
		typedef void (GLFUNC *ptr_glNamedStringARB)(GLenum type, GLint namelen, const GLchar *name, GLint stringlen, const GLchar *string);
		typedef void (GLFUNC *ptr_glDeleteNamedStringARB)(GLint namelen, const GLchar *name);
		typedef void (GLFUNC *ptr_glCompileShaderIncludeARB)(GLuint shader, GLsizei count, const GLchar* *path, const GLint *length);
		typedef GLboolean (GLFUNC *ptr_glIsNamedStringARB)(GLint namelen, const GLchar *name);
		typedef void (GLFUNC *ptr_glGetNamedStringARB)(GLint namelen, const GLchar *name, GLsizei bufSize, GLint *stringlen, GLchar *string);
		typedef void (GLFUNC *ptr_glGetNamedStringivARB)(GLint namelen, const GLchar *name, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glBindFragDataLocationIndexed)(GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
		typedef GLint (GLFUNC *ptr_glGetFragDataIndex)(GLuint program, const GLchar *name);
		typedef void (GLFUNC *ptr_glGenSamplers)(GLsizei count, GLuint *samplers);
		typedef void (GLFUNC *ptr_glDeleteSamplers)(GLsizei count, const GLuint *samplers);
		typedef GLboolean (GLFUNC *ptr_glIsSampler)(GLuint sampler);
		typedef void (GLFUNC *ptr_glBindSampler)(GLuint unit, GLuint sampler);
		typedef void (GLFUNC *ptr_glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param);
		typedef void (GLFUNC *ptr_glSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint *param);
		typedef void (GLFUNC *ptr_glSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param);
		typedef void (GLFUNC *ptr_glSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat *param);
		typedef void (GLFUNC *ptr_glSamplerParameterIiv)(GLuint sampler, GLenum pname, const GLint *param);
		typedef void (GLFUNC *ptr_glSamplerParameterIuiv)(GLuint sampler, GLenum pname, const GLuint *param);
		typedef void (GLFUNC *ptr_glGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGetSamplerParameterIiv)(GLuint sampler, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat *params);
		typedef void (GLFUNC *ptr_glGetSamplerParameterIuiv)(GLuint sampler, GLenum pname, GLuint *params);
		typedef void (GLFUNC *ptr_glQueryCounter)(GLuint id, GLenum target);
		typedef void (GLFUNC *ptr_glGetQueryObjecti64v)(GLuint id, GLenum pname, CLint64 *params);
		typedef void (GLFUNC *ptr_glGetQueryObjectui64v)(GLuint id, GLenum pname, CLuint64 *params);
		typedef void (GLFUNC *ptr_glVertexP2ui)(GLenum type, GLuint value);
		typedef void (GLFUNC *ptr_glVertexP2uiv)(GLenum type, const GLuint *value);
		typedef void (GLFUNC *ptr_glVertexP3ui)(GLenum type, GLuint value);
		typedef void (GLFUNC *ptr_glVertexP3uiv)(GLenum type, const GLuint *value);
		typedef void (GLFUNC *ptr_glVertexP4ui)(GLenum type, GLuint value);
		typedef void (GLFUNC *ptr_glVertexP4uiv)(GLenum type, const GLuint *value);
		typedef void (GLFUNC *ptr_glTexCoordP1ui)(GLenum type, GLuint coords);
		typedef void (GLFUNC *ptr_glTexCoordP1uiv)(GLenum type, const GLuint *coords);
		typedef void (GLFUNC *ptr_glTexCoordP2ui)(GLenum type, GLuint coords);
		typedef void (GLFUNC *ptr_glTexCoordP2uiv)(GLenum type, const GLuint *coords);
		typedef void (GLFUNC *ptr_glTexCoordP3ui)(GLenum type, GLuint coords);
		typedef void (GLFUNC *ptr_glTexCoordP3uiv)(GLenum type, const GLuint *coords);
		typedef void (GLFUNC *ptr_glTexCoordP4ui)(GLenum type, GLuint coords);
		typedef void (GLFUNC *ptr_glTexCoordP4uiv)(GLenum type, const GLuint *coords);
		typedef void (GLFUNC *ptr_glMultiTexCoordP1ui)(GLenum texture, GLenum type, GLuint coords);
		typedef void (GLFUNC *ptr_glMultiTexCoordP1uiv)(GLenum texture, GLenum type, const GLuint *coords);
		typedef void (GLFUNC *ptr_glMultiTexCoordP2ui)(GLenum texture, GLenum type, GLuint coords);
		typedef void (GLFUNC *ptr_glMultiTexCoordP2uiv)(GLenum texture, GLenum type, const GLuint *coords);
		typedef void (GLFUNC *ptr_glMultiTexCoordP3ui)(GLenum texture, GLenum type, GLuint coords);
		typedef void (GLFUNC *ptr_glMultiTexCoordP3uiv)(GLenum texture, GLenum type, const GLuint *coords);
		typedef void (GLFUNC *ptr_glMultiTexCoordP4ui)(GLenum texture, GLenum type, GLuint coords);
		typedef void (GLFUNC *ptr_glMultiTexCoordP4uiv)(GLenum texture, GLenum type, const GLuint *coords);
		typedef void (GLFUNC *ptr_glNormalP3ui)(GLenum type, GLuint coords);
		typedef void (GLFUNC *ptr_glNormalP3uiv)(GLenum type, const GLuint *coords);
		typedef void (GLFUNC *ptr_glColorP3ui)(GLenum type, GLuint color);
		typedef void (GLFUNC *ptr_glColorP3uiv)(GLenum type, const GLuint *color);
		typedef void (GLFUNC *ptr_glColorP4ui)(GLenum type, GLuint color);
		typedef void (GLFUNC *ptr_glColorP4uiv)(GLenum type, const GLuint *color);
		typedef void (GLFUNC *ptr_glSecondaryColorP3ui)(GLenum type, GLuint color);
		typedef void (GLFUNC *ptr_glSecondaryColorP3uiv)(GLenum type, const GLuint *color);
		typedef void (GLFUNC *ptr_glVertexAttribP1ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
		typedef void (GLFUNC *ptr_glVertexAttribP1uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
		typedef void (GLFUNC *ptr_glVertexAttribP2ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
		typedef void (GLFUNC *ptr_glVertexAttribP2uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
		typedef void (GLFUNC *ptr_glVertexAttribP3ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
		typedef void (GLFUNC *ptr_glVertexAttribP3uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
		typedef void (GLFUNC *ptr_glVertexAttribP4ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
		typedef void (GLFUNC *ptr_glVertexAttribP4uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
		typedef void (GLFUNC *ptr_glDrawArraysIndirect)(GLenum mode, const GLvoid *indirect);
		typedef void (GLFUNC *ptr_glDrawElementsIndirect)(GLenum mode, GLenum type, const GLvoid *indirect);
		typedef void (GLFUNC *ptr_glUniform1d)(GLint location, GLdouble x);
		typedef void (GLFUNC *ptr_glUniform2d)(GLint location, GLdouble x, GLdouble y);
		typedef void (GLFUNC *ptr_glUniform3d)(GLint location, GLdouble x, GLdouble y, GLdouble z);
		typedef void (GLFUNC *ptr_glUniform4d)(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
		typedef void (GLFUNC *ptr_glUniform1dv)(GLint location, GLsizei count, const GLdouble *value);
		typedef void (GLFUNC *ptr_glUniform2dv)(GLint location, GLsizei count, const GLdouble *value);
		typedef void (GLFUNC *ptr_glUniform3dv)(GLint location, GLsizei count, const GLdouble *value);
		typedef void (GLFUNC *ptr_glUniform4dv)(GLint location, GLsizei count, const GLdouble *value);
		typedef void (GLFUNC *ptr_glUniformMatrix2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glUniformMatrix3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glUniformMatrix4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glUniformMatrix2x3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glUniformMatrix2x4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glUniformMatrix3x2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glUniformMatrix3x4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glUniformMatrix4x2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glUniformMatrix4x3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glGetUniformdv)(GLuint program, GLint location, GLdouble *params);
		typedef GLint (GLFUNC *ptr_glGetSubroutineUniformLocation)(GLuint program, GLenum shadertype, const GLchar *name);
		typedef GLuint (GLFUNC *ptr_glGetSubroutineIndex)(GLuint program, GLenum shadertype, const GLchar *name);
		typedef void (GLFUNC *ptr_glGetActiveSubroutineUniformiv)(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values);
		typedef void (GLFUNC *ptr_glGetActiveSubroutineUniformName)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
		typedef void (GLFUNC *ptr_glGetActiveSubroutineName)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei *length, GLchar *name);
		typedef void (GLFUNC *ptr_glUniformSubroutinesuiv)(GLenum shadertype, GLsizei count, const GLuint *indices);
		typedef void (GLFUNC *ptr_glGetUniformSubroutineuiv)(GLenum shadertype, GLint location, GLuint *params);
		typedef void (GLFUNC *ptr_glGetProgramStageiv)(GLuint program, GLenum shadertype, GLenum pname, GLint *values);
		typedef void (GLFUNC *ptr_glPatchParameteri)(GLenum pname, GLint value);
		typedef void (GLFUNC *ptr_glPatchParameterfv)(GLenum pname, const GLfloat *values);
		typedef void (GLFUNC *ptr_glBindTransformFeedback)(GLenum target, GLuint id);
		typedef void (GLFUNC *ptr_glDeleteTransformFeedbacks)(GLsizei n, const GLuint *ids);
		typedef void (GLFUNC *ptr_glGenTransformFeedbacks)(GLsizei n, GLuint *ids);
		typedef GLboolean (GLFUNC *ptr_glIsTransformFeedback)(GLuint id);
		typedef void (GLFUNC *ptr_glPauseTransformFeedback)(void);
		typedef void (GLFUNC *ptr_glResumeTransformFeedback)(void);
		typedef void (GLFUNC *ptr_glDrawTransformFeedback)(GLenum mode, GLuint id);
		typedef void (GLFUNC *ptr_glDrawTransformFeedbackStream)(GLenum mode, GLuint id, GLuint stream);
		typedef void (GLFUNC *ptr_glBeginQueryIndexed)(GLenum target, GLuint index, GLuint id);
		typedef void (GLFUNC *ptr_glEndQueryIndexed)(GLenum target, GLuint index);
		typedef void (GLFUNC *ptr_glGetQueryIndexediv)(GLenum target, GLuint index, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glReleaseShaderCompiler)(void);
		typedef void (GLFUNC *ptr_glShaderBinary)(GLsizei count, const GLuint *shaders, GLenum binaryformat, const GLvoid *binary, GLsizei length);
		typedef void (GLFUNC *ptr_glGetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
		typedef void (GLFUNC *ptr_glDepthRangef)(GLfloat n, GLfloat f);
		typedef void (GLFUNC *ptr_glClearDepthf)(GLfloat d);
		typedef void (GLFUNC *ptr_glGetProgramBinary)(GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, GLvoid *binary);
		typedef void (GLFUNC *ptr_glProgramBinary)(GLuint program, GLenum binaryFormat, const GLvoid *binary, GLsizei length);
		typedef void (GLFUNC *ptr_glProgramParameteri)(GLuint program, GLenum pname, GLint value);
		typedef void (GLFUNC *ptr_glUseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program);
		typedef void (GLFUNC *ptr_glActiveShaderProgram)(GLuint pipeline, GLuint program);
		typedef GLuint (GLFUNC *ptr_glCreateShaderProgramv)(GLenum type, GLsizei count, const GLchar* const *strings);
		typedef void (GLFUNC *ptr_glBindProgramPipeline)(GLuint pipeline);
		typedef void (GLFUNC *ptr_glDeleteProgramPipelines)(GLsizei n, const GLuint *pipelines);
		typedef void (GLFUNC *ptr_glGenProgramPipelines)(GLsizei n, GLuint *pipelines);
		typedef GLboolean (GLFUNC *ptr_glIsProgramPipeline)(GLuint pipeline);
		typedef void (GLFUNC *ptr_glGetProgramPipelineiv)(GLuint pipeline, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glProgramUniform1i)(GLuint program, GLint location, GLint v0);
		typedef void (GLFUNC *ptr_glProgramUniform1iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
		typedef void (GLFUNC *ptr_glProgramUniform1f)(GLuint program, GLint location, GLfloat v0);
		typedef void (GLFUNC *ptr_glProgramUniform1fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
		typedef void (GLFUNC *ptr_glProgramUniform1d)(GLuint program, GLint location, GLdouble v0);
		typedef void (GLFUNC *ptr_glProgramUniform1dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
		typedef void (GLFUNC *ptr_glProgramUniform1ui)(GLuint program, GLint location, GLuint v0);
		typedef void (GLFUNC *ptr_glProgramUniform1uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
		typedef void (GLFUNC *ptr_glProgramUniform2i)(GLuint program, GLint location, GLint v0, GLint v1);
		typedef void (GLFUNC *ptr_glProgramUniform2iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
		typedef void (GLFUNC *ptr_glProgramUniform2f)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
		typedef void (GLFUNC *ptr_glProgramUniform2fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
		typedef void (GLFUNC *ptr_glProgramUniform2d)(GLuint program, GLint location, GLdouble v0, GLdouble v1);
		typedef void (GLFUNC *ptr_glProgramUniform2dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
		typedef void (GLFUNC *ptr_glProgramUniform2ui)(GLuint program, GLint location, GLuint v0, GLuint v1);
		typedef void (GLFUNC *ptr_glProgramUniform2uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
		typedef void (GLFUNC *ptr_glProgramUniform3i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
		typedef void (GLFUNC *ptr_glProgramUniform3iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
		typedef void (GLFUNC *ptr_glProgramUniform3f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
		typedef void (GLFUNC *ptr_glProgramUniform3fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
		typedef void (GLFUNC *ptr_glProgramUniform3d)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
		typedef void (GLFUNC *ptr_glProgramUniform3dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
		typedef void (GLFUNC *ptr_glProgramUniform3ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
		typedef void (GLFUNC *ptr_glProgramUniform3uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
		typedef void (GLFUNC *ptr_glProgramUniform4i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
		typedef void (GLFUNC *ptr_glProgramUniform4iv)(GLuint program, GLint location, GLsizei count, const GLint *value);
		typedef void (GLFUNC *ptr_glProgramUniform4f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		typedef void (GLFUNC *ptr_glProgramUniform4fv)(GLuint program, GLint location, GLsizei count, const GLfloat *value);
		typedef void (GLFUNC *ptr_glProgramUniform4d)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
		typedef void (GLFUNC *ptr_glProgramUniform4dv)(GLuint program, GLint location, GLsizei count, const GLdouble *value);
		typedef void (GLFUNC *ptr_glProgramUniform4ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
		typedef void (GLFUNC *ptr_glProgramUniform4uiv)(GLuint program, GLint location, GLsizei count, const GLuint *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix2x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix3x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix2x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix4x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix3x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix4x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix2x3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix3x2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix2x4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix4x2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix3x4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glProgramUniformMatrix4x3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
		typedef void (GLFUNC *ptr_glValidateProgramPipeline)(GLuint pipeline);
		typedef void (GLFUNC *ptr_glGetProgramPipelineInfoLog)(GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
		typedef void (GLFUNC *ptr_glVertexAttribL1d)(GLuint index, GLdouble x);
		typedef void (GLFUNC *ptr_glVertexAttribL2d)(GLuint index, GLdouble x, GLdouble y);
		typedef void (GLFUNC *ptr_glVertexAttribL3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
		typedef void (GLFUNC *ptr_glVertexAttribL4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
		typedef void (GLFUNC *ptr_glVertexAttribL1dv)(GLuint index, const GLdouble *v);
		typedef void (GLFUNC *ptr_glVertexAttribL2dv)(GLuint index, const GLdouble *v);
		typedef void (GLFUNC *ptr_glVertexAttribL3dv)(GLuint index, const GLdouble *v);
		typedef void (GLFUNC *ptr_glVertexAttribL4dv)(GLuint index, const GLdouble *v);
		typedef void (GLFUNC *ptr_glVertexAttribLPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
		typedef void (GLFUNC *ptr_glGetVertexAttribLdv)(GLuint index, GLenum pname, GLdouble *params);
		typedef void (GLFUNC *ptr_glViewportArrayv)(GLuint first, GLsizei count, const GLfloat *v);
		typedef void (GLFUNC *ptr_glViewportIndexedf)(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
		typedef void (GLFUNC *ptr_glViewportIndexedfv)(GLuint index, const GLfloat *v);
		typedef void (GLFUNC *ptr_glScissorArrayv)(GLuint first, GLsizei count, const GLint *v);
		typedef void (GLFUNC *ptr_glScissorIndexed)(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
		typedef void (GLFUNC *ptr_glScissorIndexedv)(GLuint index, const GLint *v);
		typedef void (GLFUNC *ptr_glDepthRangeArrayv)(GLuint first, GLsizei count, const GLdouble *v);
		typedef void (GLFUNC *ptr_glDepthRangeIndexed)(GLuint index, GLdouble n, GLdouble f);
		typedef void (GLFUNC *ptr_glGetFloati_v)(GLenum target, GLuint index, GLfloat *data);
		typedef void (GLFUNC *ptr_glGetDoublei_v)(GLenum target, GLuint index, GLdouble *data);
		typedef CLsync (GLFUNC *ptr_glCreateSyncFromCLeventARB)(struct _cl_context * context, struct _cl_event * event, GLbitfield flags);
		typedef void (GLFUNC *ptr_glDebugMessageControlARB)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
		typedef void (GLFUNC *ptr_glDebugMessageInsertARB)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
		typedef void (GLFUNC *ptr_glDebugMessageCallbackARB)(CLDEBUGPROCARB callback, const GLvoid *userParam);
		typedef GLuint (GLFUNC *ptr_glGetDebugMessageLogARB)(GLuint count, GLsizei bufsize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
		typedef GLenum (GLFUNC *ptr_glGetGraphicsResetStatusARB)(void);
		typedef void (GLFUNC *ptr_glGetnMapdvARB)(GLenum target, GLenum query, GLsizei bufSize, GLdouble *v);
		typedef void (GLFUNC *ptr_glGetnMapfvARB)(GLenum target, GLenum query, GLsizei bufSize, GLfloat *v);
		typedef void (GLFUNC *ptr_glGetnMapivARB)(GLenum target, GLenum query, GLsizei bufSize, GLint *v);
		typedef void (GLFUNC *ptr_glGetnPixelMapfvARB)(GLenum map, GLsizei bufSize, GLfloat *values);
		typedef void (GLFUNC *ptr_glGetnPixelMapuivARB)(GLenum map, GLsizei bufSize, GLuint *values);
		typedef void (GLFUNC *ptr_glGetnPixelMapusvARB)(GLenum map, GLsizei bufSize, GLushort *values);
		typedef void (GLFUNC *ptr_glGetnPolygonStippleARB)(GLsizei bufSize, GLubyte *pattern);
		typedef void (GLFUNC *ptr_glGetnColorTableARB)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid *table);
		typedef void (GLFUNC *ptr_glGetnConvolutionFilterARB)(GLenum target, GLenum format, GLenum type, GLsizei bufSize, GLvoid *image);
		typedef void (GLFUNC *ptr_glGetnSeparableFilterARB)(GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, GLvoid *row, GLsizei columnBufSize, GLvoid *column, GLvoid *span);
		typedef void (GLFUNC *ptr_glGetnHistogramARB)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid *values);
		typedef void (GLFUNC *ptr_glGetnMinmaxARB)(GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, GLvoid *values);
		typedef void (GLFUNC *ptr_glGetnTexImageARB)(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, GLvoid *img);
		typedef void (GLFUNC *ptr_glReadnPixelsARB)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, GLvoid *data);
		typedef void (GLFUNC *ptr_glGetnCompressedTexImageARB)(GLenum target, GLint lod, GLsizei bufSize, GLvoid *img);
		typedef void (GLFUNC *ptr_glGetnUniformfvARB)(GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
		typedef void (GLFUNC *ptr_glGetnUniformivARB)(GLuint program, GLint location, GLsizei bufSize, GLint *params);
		typedef void (GLFUNC *ptr_glGetnUniformuivARB)(GLuint program, GLint location, GLsizei bufSize, GLuint *params);
		typedef void (GLFUNC *ptr_glGetnUniformdvARB)(GLuint program, GLint location, GLsizei bufSize, GLdouble *params);
		typedef void (GLFUNC *ptr_glDrawArraysInstancedBaseInstance)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
		typedef void (GLFUNC *ptr_glDrawElementsInstancedBaseInstance)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance);
		typedef void (GLFUNC *ptr_glDrawElementsInstancedBaseVertexBaseInstance)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
		typedef void (GLFUNC *ptr_glDrawTransformFeedbackInstanced)(GLenum mode, GLuint id, GLsizei instancecount);
		typedef void (GLFUNC *ptr_glDrawTransformFeedbackStreamInstanced)(GLenum mode, GLuint id, GLuint stream, GLsizei instancecount);
		typedef void (GLFUNC *ptr_glGetInternalformativ)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint *params);
		typedef void (GLFUNC *ptr_glGetActiveAtomicCounterBufferiv)(GLuint program, GLuint bufferIndex, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glBindImageTexture)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
		typedef void (GLFUNC *ptr_glMemoryBarrier)(GLbitfield barriers);
		typedef void (GLFUNC *ptr_glTexStorage1D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
		typedef void (GLFUNC *ptr_glTexStorage2D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
		typedef void (GLFUNC *ptr_glTexStorage3D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
		typedef void (GLFUNC *ptr_glTextureStorage1DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
		typedef void (GLFUNC *ptr_glTextureStorage2DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
		typedef void (GLFUNC *ptr_glTextureStorage3DEXT)(GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
		typedef void (GLFUNC *ptr_glDebugMessageControl)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
		typedef void (GLFUNC *ptr_glDebugMessageInsert)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
		typedef void (GLFUNC *ptr_glDebugMessageCallback)(CLDEBUGPROC callback, const void *userParam);
		typedef GLuint (GLFUNC *ptr_glGetDebugMessageLog)(GLuint count, GLsizei bufsize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
		typedef void (GLFUNC *ptr_glPushDebugGroup)(GLenum source, GLuint id, GLsizei length, const GLchar *message);
		typedef void (GLFUNC *ptr_glPopDebugGroup)(void);
		typedef void (GLFUNC *ptr_glObjectLabel)(GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
		typedef void (GLFUNC *ptr_glGetObjectLabel)(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label);
		typedef void (GLFUNC *ptr_glObjectPtrLabel)(const void *ptr, GLsizei length, const GLchar *label);
		typedef void (GLFUNC *ptr_glGetObjectPtrLabel)(const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);
		typedef void (GLFUNC *ptr_glClearBufferData)(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data);
		typedef void (GLFUNC *ptr_glClearBufferSubData)(GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
		typedef void (GLFUNC *ptr_glClearNamedBufferDataEXT)(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
		typedef void (GLFUNC *ptr_glClearNamedBufferSubDataEXT)(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, GLsizeiptr offset, GLsizeiptr size, const void *data);
		typedef void (GLFUNC *ptr_glDispatchCompute)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
		typedef void (GLFUNC *ptr_glDispatchComputeIndirect)(GLintptr indirect);
		typedef void (GLFUNC *ptr_glCopyImageSubData)(GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
		typedef void (GLFUNC *ptr_glFramebufferParameteri)(GLenum target, GLenum pname, GLint param);
		typedef void (GLFUNC *ptr_glGetFramebufferParameteriv)(GLenum target, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glNamedFramebufferParameteriEXT)(GLuint framebuffer, GLenum pname, GLint param);
		typedef void (GLFUNC *ptr_glGetNamedFramebufferParameterivEXT)(GLuint framebuffer, GLenum pname, GLint *params);
		typedef void (GLFUNC *ptr_glGetInternalformati64v)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, CLint64 *params);
		typedef void (GLFUNC *ptr_glInvalidateTexSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
		typedef void (GLFUNC *ptr_glInvalidateTexImage)(GLuint texture, GLint level);
		typedef void (GLFUNC *ptr_glInvalidateBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr length);
		typedef void (GLFUNC *ptr_glInvalidateBufferData)(GLuint buffer);
		typedef void (GLFUNC *ptr_glInvalidateFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments);
		typedef void (GLFUNC *ptr_glInvalidateSubFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
		typedef void (GLFUNC *ptr_glMultiDrawArraysIndirect)(GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride);
		typedef void (GLFUNC *ptr_glMultiDrawElementsIndirect)(GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride);
		typedef void (GLFUNC *ptr_glGetProgramInterfaceiv)(GLuint program, GLenum programInterface, GLenum pname, GLint *params);
		typedef GLuint (GLFUNC *ptr_glGetProgramResourceIndex)(GLuint program, GLenum programInterface, const GLchar *name);
		typedef void (GLFUNC *ptr_glGetProgramResourceName)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
		typedef void (GLFUNC *ptr_glGetProgramResourceiv)(GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei bufSize, GLsizei *length, GLint *params);
		typedef GLint (GLFUNC *ptr_glGetProgramResourceLocation)(GLuint program, GLenum programInterface, const GLchar *name);
		typedef GLint (GLFUNC *ptr_glGetProgramResourceLocationIndex)(GLuint program, GLenum programInterface, const GLchar *name);
		typedef void (GLFUNC *ptr_glShaderStorageBlockBinding)(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
		typedef void (GLFUNC *ptr_glTexBufferRange)(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
		typedef void (GLFUNC *ptr_glTextureBufferRangeEXT)(GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
		typedef void (GLFUNC *ptr_glTexStorage2DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
		typedef void (GLFUNC *ptr_glTexStorage3DMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
		typedef void (GLFUNC *ptr_glTextureStorage2DMultisampleEXT)(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
		typedef void (GLFUNC *ptr_glTextureStorage3DMultisampleEXT)(GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
		typedef void (GLFUNC *ptr_glTextureView)(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
		typedef void (GLFUNC *ptr_glBindVertexBuffer)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
		typedef void (GLFUNC *ptr_glVertexAttribFormat)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
		typedef void (GLFUNC *ptr_glVertexAttribIFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
		typedef void (GLFUNC *ptr_glVertexAttribLFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
		typedef void (GLFUNC *ptr_glVertexAttribBinding)(GLuint attribindex, GLuint bindingindex);
		typedef void (GLFUNC *ptr_glVertexBindingDivisor)(GLuint bindingindex, GLuint divisor);
		typedef void (GLFUNC *ptr_glVertexArrayBindVertexBufferEXT)(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
		typedef void (GLFUNC *ptr_glVertexArrayVertexAttribFormatEXT)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
		typedef void (GLFUNC *ptr_glVertexArrayVertexAttribIFormatEXT)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
		typedef void (GLFUNC *ptr_glVertexArrayVertexAttribLFormatEXT)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
		typedef void (GLFUNC *ptr_glVertexArrayVertexAttribBindingEXT)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
		typedef void (GLFUNC *ptr_glVertexArrayVertexBindingDivisorEXT)(GLuint vaobj, GLuint bindingindex, GLuint divisor);

		// OpenGL 4.4
		typedef void (GLFUNC *ptr_glBufferStorage)(GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
		typedef void (GLFUNC *ptr_glClearTexImage)(GLuint texture, GLint level, GLenum format, GLenum type, const void *data);
		typedef void (GLFUNC *ptr_glClearTexSubImage)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
		typedef void (GLFUNC *ptr_glBindBuffersBase)(GLenum target, GLuint first, GLsizei count, const GLuint *buffers);
		typedef void (GLFUNC *ptr_glBindBuffersRange)(GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes);
		typedef void (GLFUNC *ptr_glBindTextures)(GLuint first, GLsizei count, const GLuint *textures);
		typedef void (GLFUNC *ptr_glBindSamplers)(GLuint first, GLsizei count, const GLuint *samplers);
		typedef void (GLFUNC *ptr_glBindImageTextures)(GLuint first, GLsizei count, const GLuint *textures);
		typedef void (GLFUNC *ptr_glBindVertexBuffers)(GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);

		// For Legacy OpenGL (For GL1 target)
		typedef GLvoid (GLFUNC *ptr_glClientActiveTextureARB)(GLenum);
	#ifdef WIN32
		DECLARE_HANDLE(HPBUFFERARB);
		typedef HPBUFFERARB (GLFUNC *ptr_wglCreatePbufferARB)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
		typedef HDC (GLFUNC *ptr_wglGetPbufferDCARB)(HPBUFFERARB hPbuffer);
		typedef int (GLFUNC *ptr_wglReleasePbufferDCARB)(HPBUFFERARB hPbuffer, HDC hDC);
		typedef BOOL (GLFUNC *ptr_wglDestroyPbufferARB)(HPBUFFERARB hPbuffer);
		typedef BOOL (GLFUNC *ptr_wglQueryPbufferARB)(HPBUFFERARB hPbuffer, int iAttribute, int *piValue);
	#endif

		ptr_glCullFace cullFace;
		ptr_glFrontFace frontFace;
		ptr_glHint hint;
		ptr_glLineWidth lineWidth;
		ptr_glPointSize pointSize;
		ptr_glPolygonMode polygonMode;
		ptr_glScissor scissor;
		ptr_glTexParameterf texParameterf;
		ptr_glTexParameterfv texParameterfv;
		ptr_glTexParameteri texParameteri;
		ptr_glTexParameteriv texParameteriv;
		ptr_glTexImage1D texImage1D;
		ptr_glTexImage2D texImage2D;
		ptr_glDrawBuffer drawBuffer;
		ptr_glClear clear;
		ptr_glClearColor clearColor;
		ptr_glClearStencil clearStencil;
		ptr_glClearDepth clearDepth;
		ptr_glStencilMask stencilMask;
		ptr_glColorMask colorMask;
		ptr_glDepthMask depthMask;
		ptr_glDisable disable;
		ptr_glEnable enable;
		ptr_glFinish finish;
		ptr_glFlush flush;
		ptr_glBlendFunc blendFunc;
		ptr_glLogicOp logicOp;
		ptr_glStencilFunc stencilFunc;
		ptr_glStencilOp stencilOp;
		ptr_glDepthFunc depthFunc;
		ptr_glPixelStoref pixelStoref;
		ptr_glPixelStorei pixelStorei;
		ptr_glReadBuffer readBuffer;
		ptr_glReadPixels readPixels;
		ptr_glGetBooleanv getBooleanv;
		ptr_glGetDoublev getDoublev;
		ptr_glGetError getError;
		ptr_glGetFloatv getFloatv;
		ptr_glGetIntegerv getIntegerv;
		ptr_glGetString getString;
		ptr_glGetTexImage getTexImage;
		ptr_glGetTexParameterfv getTexParameterfv;
		ptr_glGetTexParameteriv getTexParameteriv;
		ptr_glGetTexLevelParameterfv getTexLevelParameterfv;
		ptr_glGetTexLevelParameteriv getTexLevelParameteriv;
		ptr_glIsEnabled isEnabled;
		ptr_glDepthRange depthRange;
		ptr_glViewport viewport;
		ptr_glDrawArrays drawArrays;
		ptr_glDrawElements drawElements;
		ptr_glGetPointerv getPointerv;
		ptr_glPolygonOffset polygonOffset;
		ptr_glCopyTexImage1D copyTexImage1D;
		ptr_glCopyTexImage2D copyTexImage2D;
		ptr_glCopyTexSubImage1D copyTexSubImage1D;
		ptr_glCopyTexSubImage2D copyTexSubImage2D;
		ptr_glTexSubImage1D texSubImage1D;
		ptr_glTexSubImage2D texSubImage2D;
		ptr_glBindTexture bindTexture;
		ptr_glDeleteTextures deleteTextures;
		ptr_glGenTextures genTextures;
		ptr_glIsTexture isTexture;
		ptr_glBlendColor blendColor;
		ptr_glBlendEquation blendEquation;
		ptr_glDrawRangeElements drawRangeElements;
		ptr_glTexImage3D texImage3D;
		ptr_glTexSubImage3D texSubImage3D;
		ptr_glCopyTexSubImage3D copyTexSubImage3D;
		ptr_glActiveTexture activeTexture;
		ptr_glSampleCoverage sampleCoverage;
		ptr_glCompressedTexImage3D compressedTexImage3D;
		ptr_glCompressedTexImage2D compressedTexImage2D;
		ptr_glCompressedTexImage1D compressedTexImage1D;
		ptr_glCompressedTexSubImage3D compressedTexSubImage3D;
		ptr_glCompressedTexSubImage2D compressedTexSubImage2D;
		ptr_glCompressedTexSubImage1D compressedTexSubImage1D;
		ptr_glGetCompressedTexImage getCompressedTexImage;
		ptr_glBlendFuncSeparate blendFuncSeparate;
		ptr_glMultiDrawArrays multiDrawArrays;
		ptr_glMultiDrawElements multiDrawElements;
		ptr_glPointParameterf pointParameterf;
		ptr_glPointParameterfv pointParameterfv;
		ptr_glPointParameteri pointParameteri;
		ptr_glPointParameteriv pointParameteriv;
		ptr_glGenQueries genQueries;
		ptr_glDeleteQueries deleteQueries;
		ptr_glIsQuery isQuery;
		ptr_glBeginQuery beginQuery;
		ptr_glEndQuery endQuery;
		ptr_glGetQueryiv getQueryiv;
		ptr_glGetQueryObjectiv getQueryObjectiv;
		ptr_glGetQueryObjectuiv getQueryObjectuiv;
		ptr_glBindBuffer bindBuffer;
		ptr_glDeleteBuffers deleteBuffers;
		ptr_glGenBuffers genBuffers;
		ptr_glIsBuffer isBuffer;
		ptr_glBufferData bufferData;
		ptr_glBufferSubData bufferSubData;
		ptr_glGetBufferSubData getBufferSubData;
		ptr_glMapBuffer mapBuffer;
		ptr_glUnmapBuffer unmapBuffer;
		ptr_glGetBufferParameteriv getBufferParameteriv;
		ptr_glGetBufferPointerv getBufferPointerv;
		ptr_glBlendEquationSeparate blendEquationSeparate;
		ptr_glDrawBuffers drawBuffers;
		ptr_glStencilOpSeparate stencilOpSeparate;
		ptr_glStencilFuncSeparate stencilFuncSeparate;
		ptr_glStencilMaskSeparate stencilMaskSeparate;
		ptr_glAttachShader attachShader;
		ptr_glBindAttribLocation bindAttribLocation;
		ptr_glCompileShader compileShader;
		ptr_glCreateProgram createProgram;
		ptr_glCreateShader createShader;
		ptr_glDeleteProgram deleteProgram;
		ptr_glDeleteShader deleteShader;
		ptr_glDetachShader detachShader;
		ptr_glDisableVertexAttribArray disableVertexAttribArray;
		ptr_glEnableVertexAttribArray enableVertexAttribArray;
		ptr_glGetActiveAttrib getActiveAttrib;
		ptr_glGetActiveUniform getActiveUniform;
		ptr_glGetAttachedShaders getAttachedShaders;
		ptr_glGetAttribLocation getAttribLocation;
		ptr_glGetProgramiv getProgramiv;
		ptr_glGetProgramInfoLog getProgramInfoLog;
		ptr_glGetShaderiv getShaderiv;
		ptr_glGetShaderInfoLog getShaderInfoLog;
		ptr_glGetShaderSource getShaderSource;
		ptr_glGetUniformLocation getUniformLocation;
		ptr_glGetUniformfv getUniformfv;
		ptr_glGetUniformiv getUniformiv;
		ptr_glGetVertexAttribdv getVertexAttribdv;
		ptr_glGetVertexAttribfv getVertexAttribfv;
		ptr_glGetVertexAttribiv getVertexAttribiv;
		ptr_glGetVertexAttribPointerv getVertexAttribPointerv;
		ptr_glIsProgram isProgram;
		ptr_glIsShader isShader;
		ptr_glLinkProgram linkProgram;
		ptr_glShaderSource shaderSource;
		ptr_glUseProgram useProgram;
		ptr_glUniform1f uniform1f;
		ptr_glUniform2f uniform2f;
		ptr_glUniform3f uniform3f;
		ptr_glUniform4f uniform4f;
		ptr_glUniform1i uniform1i;
		ptr_glUniform2i uniform2i;
		ptr_glUniform3i uniform3i;
		ptr_glUniform4i uniform4i;
		ptr_glUniform1fv uniform1fv;
		ptr_glUniform2fv uniform2fv;
		ptr_glUniform3fv uniform3fv;
		ptr_glUniform4fv uniform4fv;
		ptr_glUniform1iv uniform1iv;
		ptr_glUniform2iv uniform2iv;
		ptr_glUniform3iv uniform3iv;
		ptr_glUniform4iv uniform4iv;
		ptr_glUniformMatrix2fv uniformMatrix2fv;
		ptr_glUniformMatrix3fv uniformMatrix3fv;
		ptr_glUniformMatrix4fv uniformMatrix4fv;
		ptr_glValidateProgram validateProgram;
		ptr_glVertexAttrib1d vertexAttrib1d;
		ptr_glVertexAttrib1dv vertexAttrib1dv;
		ptr_glVertexAttrib1f vertexAttrib1f;
		ptr_glVertexAttrib1fv vertexAttrib1fv;
		ptr_glVertexAttrib1s vertexAttrib1s;
		ptr_glVertexAttrib1sv vertexAttrib1sv;
		ptr_glVertexAttrib2d vertexAttrib2d;
		ptr_glVertexAttrib2dv vertexAttrib2dv;
		ptr_glVertexAttrib2f vertexAttrib2f;
		ptr_glVertexAttrib2fv vertexAttrib2fv;
		ptr_glVertexAttrib2s vertexAttrib2s;
		ptr_glVertexAttrib2sv vertexAttrib2sv;
		ptr_glVertexAttrib3d vertexAttrib3d;
		ptr_glVertexAttrib3dv vertexAttrib3dv;
		ptr_glVertexAttrib3f vertexAttrib3f;
		ptr_glVertexAttrib3fv vertexAttrib3fv;
		ptr_glVertexAttrib3s vertexAttrib3s;
		ptr_glVertexAttrib3sv vertexAttrib3sv;
		ptr_glVertexAttrib4Nbv vertexAttrib4Nbv;
		ptr_glVertexAttrib4Niv vertexAttrib4Niv;
		ptr_glVertexAttrib4Nsv vertexAttrib4Nsv;
		ptr_glVertexAttrib4Nub vertexAttrib4Nub;
		ptr_glVertexAttrib4Nubv vertexAttrib4Nubv;
		ptr_glVertexAttrib4Nuiv vertexAttrib4Nuiv;
		ptr_glVertexAttrib4Nusv vertexAttrib4Nusv;
		ptr_glVertexAttrib4bv vertexAttrib4bv;
		ptr_glVertexAttrib4d vertexAttrib4d;
		ptr_glVertexAttrib4dv vertexAttrib4dv;
		ptr_glVertexAttrib4f vertexAttrib4f;
		ptr_glVertexAttrib4fv vertexAttrib4fv;
		ptr_glVertexAttrib4iv vertexAttrib4iv;
		ptr_glVertexAttrib4s vertexAttrib4s;
		ptr_glVertexAttrib4sv vertexAttrib4sv;
		ptr_glVertexAttrib4ubv vertexAttrib4ubv;
		ptr_glVertexAttrib4uiv vertexAttrib4uiv;
		ptr_glVertexAttrib4usv vertexAttrib4usv;
		ptr_glVertexAttribPointer vertexAttribPointer;
		ptr_glUniformMatrix2x3fv uniformMatrix2x3fv;
		ptr_glUniformMatrix3x2fv uniformMatrix3x2fv;
		ptr_glUniformMatrix2x4fv uniformMatrix2x4fv;
		ptr_glUniformMatrix4x2fv uniformMatrix4x2fv;
		ptr_glUniformMatrix3x4fv uniformMatrix3x4fv;
		ptr_glUniformMatrix4x3fv uniformMatrix4x3fv;
		ptr_glColorMaski colorMaski;
		ptr_glGetBooleani_v getBooleani_v;
		ptr_glGetIntegeri_v getIntegeri_v;
		ptr_glEnablei enablei;
		ptr_glDisablei disablei;
		ptr_glIsEnabledi isEnabledi;
		ptr_glBeginTransformFeedback beginTransformFeedback;
		ptr_glEndTransformFeedback endTransformFeedback;
		ptr_glBindBufferRange bindBufferRange;
		ptr_glBindBufferBase bindBufferBase;
		ptr_glTransformFeedbackVaryings transformFeedbackVaryings;
		ptr_glGetTransformFeedbackVarying getTransformFeedbackVarying;
		ptr_glClampColor clampColor;
		ptr_glBeginConditionalRender beginConditionalRender;
		ptr_glEndConditionalRender endConditionalRender;
		ptr_glVertexAttribIPointer vertexAttribIPointer;
		ptr_glGetVertexAttribIiv getVertexAttribIiv;
		ptr_glGetVertexAttribIuiv getVertexAttribIuiv;
		ptr_glVertexAttribI1i vertexAttribI1i;
		ptr_glVertexAttribI2i vertexAttribI2i;
		ptr_glVertexAttribI3i vertexAttribI3i;
		ptr_glVertexAttribI4i vertexAttribI4i;
		ptr_glVertexAttribI1ui vertexAttribI1ui;
		ptr_glVertexAttribI2ui vertexAttribI2ui;
		ptr_glVertexAttribI3ui vertexAttribI3ui;
		ptr_glVertexAttribI4ui vertexAttribI4ui;
		ptr_glVertexAttribI1iv vertexAttribI1iv;
		ptr_glVertexAttribI2iv vertexAttribI2iv;
		ptr_glVertexAttribI3iv vertexAttribI3iv;
		ptr_glVertexAttribI4iv vertexAttribI4iv;
		ptr_glVertexAttribI1uiv vertexAttribI1uiv;
		ptr_glVertexAttribI2uiv vertexAttribI2uiv;
		ptr_glVertexAttribI3uiv vertexAttribI3uiv;
		ptr_glVertexAttribI4uiv vertexAttribI4uiv;
		ptr_glVertexAttribI4bv vertexAttribI4bv;
		ptr_glVertexAttribI4sv vertexAttribI4sv;
		ptr_glVertexAttribI4ubv vertexAttribI4ubv;
		ptr_glVertexAttribI4usv vertexAttribI4usv;
		ptr_glGetUniformuiv getUniformuiv;
		ptr_glBindFragDataLocation bindFragDataLocation;
		ptr_glGetFragDataLocation getFragDataLocation;
		ptr_glUniform1ui uniform1ui;
		ptr_glUniform2ui uniform2ui;
		ptr_glUniform3ui uniform3ui;
		ptr_glUniform4ui uniform4ui;
		ptr_glUniform1uiv uniform1uiv;
		ptr_glUniform2uiv uniform2uiv;
		ptr_glUniform3uiv uniform3uiv;
		ptr_glUniform4uiv uniform4uiv;
		ptr_glTexParameterIiv texParameterIiv;
		ptr_glTexParameterIuiv texParameterIuiv;
		ptr_glGetTexParameterIiv getTexParameterIiv;
		ptr_glGetTexParameterIuiv getTexParameterIuiv;
		ptr_glClearBufferiv clearBufferiv;
		ptr_glClearBufferuiv clearBufferuiv;
		ptr_glClearBufferfv clearBufferfv;
		ptr_glClearBufferfi clearBufferfi;
		ptr_glGetStringi getStringi;
		ptr_glDrawArraysInstanced drawArraysInstanced;
		ptr_glDrawElementsInstanced drawElementsInstanced;
		ptr_glTexBuffer texBuffer;
		ptr_glPrimitiveRestartIndex primitiveRestartIndex;
		ptr_glGetInteger64i_v getInteger64i_v;
		ptr_glGetBufferParameteri64v getBufferParameteri64v;
		ptr_glFramebufferTexture framebufferTexture;
		ptr_glVertexAttribDivisor vertexAttribDivisor;
		ptr_glMinSampleShading minSampleShading;
		ptr_glBlendEquationi blendEquationi;
		ptr_glBlendEquationSeparatei blendEquationSeparatei;
		ptr_glBlendFunci blendFunci;
		ptr_glBlendFuncSeparatei blendFuncSeparatei;
		ptr_glIsRenderbuffer isRenderbuffer;
		ptr_glBindRenderbuffer bindRenderbuffer;
		ptr_glDeleteRenderbuffers deleteRenderbuffers;
		ptr_glGenRenderbuffers genRenderbuffers;
		ptr_glRenderbufferStorage renderbufferStorage;
		ptr_glGetRenderbufferParameteriv getRenderbufferParameteriv;
		ptr_glIsFramebuffer isFramebuffer;
		ptr_glBindFramebuffer bindFramebuffer;
		ptr_glDeleteFramebuffers deleteFramebuffers;
		ptr_glGenFramebuffers genFramebuffers;
		ptr_glCheckFramebufferStatus checkFramebufferStatus;
		ptr_glFramebufferTexture1D framebufferTexture1D;
		ptr_glFramebufferTexture2D framebufferTexture2D;
		ptr_glFramebufferTexture3D framebufferTexture3D;
		ptr_glFramebufferRenderbuffer framebufferRenderbuffer;
		ptr_glGetFramebufferAttachmentParameteriv getFramebufferAttachmentParameteriv;
		ptr_glGenerateMipmap generateMipmap;
		ptr_glBlitFramebuffer blitFramebuffer;
		ptr_glRenderbufferStorageMultisample renderbufferStorageMultisample;
		ptr_glFramebufferTextureLayer framebufferTextureLayer;
		ptr_glMapBufferRange mapBufferRange;
		ptr_glFlushMappedBufferRange flushMappedBufferRange;
		ptr_glBindVertexArray bindVertexArray;
		ptr_glDeleteVertexArrays deleteVertexArrays;
		ptr_glGenVertexArrays genVertexArrays;
		ptr_glIsVertexArray isVertexArray;
		ptr_glGetUniformIndices getUniformIndices;
		ptr_glGetActiveUniformsiv getActiveUniformsiv;
		ptr_glGetActiveUniformName getActiveUniformName;
		ptr_glGetUniformBlockIndex getUniformBlockIndex;
		ptr_glGetActiveUniformBlockiv getActiveUniformBlockiv;
		ptr_glGetActiveUniformBlockName getActiveUniformBlockName;
		ptr_glUniformBlockBinding uniformBlockBinding;
		ptr_glCopyBufferSubData copyBufferSubData;
		ptr_glDrawElementsBaseVertex drawElementsBaseVertex;
		ptr_glDrawRangeElementsBaseVertex drawRangeElementsBaseVertex;
		ptr_glDrawElementsInstancedBaseVertex drawElementsInstancedBaseVertex;
		ptr_glMultiDrawElementsBaseVertex multiDrawElementsBaseVertex;
		ptr_glProvokingVertex provokingVertex;
		ptr_glFenceSync fenceSync;
		ptr_glIsSync isSync;
		ptr_glDeleteSync deleteSync;
		ptr_glClientWaitSync clientWaitSync;
		ptr_glWaitSync waitSync;
		ptr_glGetInteger64v getInteger64v;
		ptr_glGetSynciv getSynciv;
		ptr_glTexImage2DMultisample texImage2DMultisample;
		ptr_glTexImage3DMultisample texImage3DMultisample;
		ptr_glGetMultisamplefv getMultisamplefv;
		ptr_glSampleMaski sampleMaski;
		ptr_glBlendEquationiARB blendEquationiARB;
		ptr_glBlendEquationSeparateiARB blendEquationSeparateiARB;
		ptr_glBlendFunciARB blendFunciARB;
		ptr_glBlendFuncSeparateiARB blendFuncSeparateiARB;
		ptr_glMinSampleShadingARB minSampleShadingARB;
		ptr_glNamedStringARB namedStringARB;
		ptr_glDeleteNamedStringARB deleteNamedStringARB;
		ptr_glCompileShaderIncludeARB compileShaderIncludeARB;
		ptr_glIsNamedStringARB isNamedStringARB;
		ptr_glGetNamedStringARB getNamedStringARB;
		ptr_glGetNamedStringivARB getNamedStringivARB;
		ptr_glBindFragDataLocationIndexed bindFragDataLocationIndexed;
		ptr_glGetFragDataIndex getFragDataIndex;
		ptr_glGenSamplers genSamplers;
		ptr_glDeleteSamplers deleteSamplers;
		ptr_glIsSampler isSampler;
		ptr_glBindSampler bindSampler;
		ptr_glSamplerParameteri samplerParameteri;
		ptr_glSamplerParameteriv samplerParameteriv;
		ptr_glSamplerParameterf samplerParameterf;
		ptr_glSamplerParameterfv samplerParameterfv;
		ptr_glSamplerParameterIiv samplerParameterIiv;
		ptr_glSamplerParameterIuiv samplerParameterIuiv;
		ptr_glGetSamplerParameteriv getSamplerParameteriv;
		ptr_glGetSamplerParameterIiv getSamplerParameterIiv;
		ptr_glGetSamplerParameterfv getSamplerParameterfv;
		ptr_glGetSamplerParameterIuiv getSamplerParameterIuiv;
		ptr_glQueryCounter queryCounter;
		ptr_glGetQueryObjecti64v getQueryObjecti64v;
		ptr_glGetQueryObjectui64v getQueryObjectui64v;
		ptr_glVertexP2ui vertexP2ui;
		ptr_glVertexP2uiv vertexP2uiv;
		ptr_glVertexP3ui vertexP3ui;
		ptr_glVertexP3uiv vertexP3uiv;
		ptr_glVertexP4ui vertexP4ui;
		ptr_glVertexP4uiv vertexP4uiv;
		ptr_glTexCoordP1ui texCoordP1ui;
		ptr_glTexCoordP1uiv texCoordP1uiv;
		ptr_glTexCoordP2ui texCoordP2ui;
		ptr_glTexCoordP2uiv texCoordP2uiv;
		ptr_glTexCoordP3ui texCoordP3ui;
		ptr_glTexCoordP3uiv texCoordP3uiv;
		ptr_glTexCoordP4ui texCoordP4ui;
		ptr_glTexCoordP4uiv texCoordP4uiv;
		ptr_glMultiTexCoordP1ui multiTexCoordP1ui;
		ptr_glMultiTexCoordP1uiv multiTexCoordP1uiv;
		ptr_glMultiTexCoordP2ui multiTexCoordP2ui;
		ptr_glMultiTexCoordP2uiv multiTexCoordP2uiv;
		ptr_glMultiTexCoordP3ui multiTexCoordP3ui;
		ptr_glMultiTexCoordP3uiv multiTexCoordP3uiv;
		ptr_glMultiTexCoordP4ui multiTexCoordP4ui;
		ptr_glMultiTexCoordP4uiv multiTexCoordP4uiv;
		ptr_glNormalP3ui normalP3ui;
		ptr_glNormalP3uiv normalP3uiv;
		ptr_glColorP3ui colorP3ui;
		ptr_glColorP3uiv colorP3uiv;
		ptr_glColorP4ui colorP4ui;
		ptr_glColorP4uiv colorP4uiv;
		ptr_glSecondaryColorP3ui secondaryColorP3ui;
		ptr_glSecondaryColorP3uiv secondaryColorP3uiv;
		ptr_glVertexAttribP1ui vertexAttribP1ui;
		ptr_glVertexAttribP1uiv vertexAttribP1uiv;
		ptr_glVertexAttribP2ui vertexAttribP2ui;
		ptr_glVertexAttribP2uiv vertexAttribP2uiv;
		ptr_glVertexAttribP3ui vertexAttribP3ui;
		ptr_glVertexAttribP3uiv vertexAttribP3uiv;
		ptr_glVertexAttribP4ui vertexAttribP4ui;
		ptr_glVertexAttribP4uiv vertexAttribP4uiv;
		ptr_glDrawArraysIndirect drawArraysIndirect;
		ptr_glDrawElementsIndirect drawElementsIndirect;
		ptr_glUniform1d uniform1d;
		ptr_glUniform2d uniform2d;
		ptr_glUniform3d uniform3d;
		ptr_glUniform4d uniform4d;
		ptr_glUniform1dv uniform1dv;
		ptr_glUniform2dv uniform2dv;
		ptr_glUniform3dv uniform3dv;
		ptr_glUniform4dv uniform4dv;
		ptr_glUniformMatrix2dv uniformMatrix2dv;
		ptr_glUniformMatrix3dv uniformMatrix3dv;
		ptr_glUniformMatrix4dv uniformMatrix4dv;
		ptr_glUniformMatrix2x3dv uniformMatrix2x3dv;
		ptr_glUniformMatrix2x4dv uniformMatrix2x4dv;
		ptr_glUniformMatrix3x2dv uniformMatrix3x2dv;
		ptr_glUniformMatrix3x4dv uniformMatrix3x4dv;
		ptr_glUniformMatrix4x2dv uniformMatrix4x2dv;
		ptr_glUniformMatrix4x3dv uniformMatrix4x3dv;
		ptr_glGetUniformdv getUniformdv;
		ptr_glGetSubroutineUniformLocation getSubroutineUniformLocation;
		ptr_glGetSubroutineIndex getSubroutineIndex;
		ptr_glGetActiveSubroutineUniformiv getActiveSubroutineUniformiv;
		ptr_glGetActiveSubroutineUniformName getActiveSubroutineUniformName;
		ptr_glGetActiveSubroutineName getActiveSubroutineName;
		ptr_glUniformSubroutinesuiv uniformSubroutinesuiv;
		ptr_glGetUniformSubroutineuiv getUniformSubroutineuiv;
		ptr_glGetProgramStageiv getProgramStageiv;
		ptr_glPatchParameteri patchParameteri;
		ptr_glPatchParameterfv patchParameterfv;
		ptr_glBindTransformFeedback bindTransformFeedback;
		ptr_glDeleteTransformFeedbacks deleteTransformFeedbacks;
		ptr_glGenTransformFeedbacks genTransformFeedbacks;
		ptr_glIsTransformFeedback isTransformFeedback;
		ptr_glPauseTransformFeedback pauseTransformFeedback;
		ptr_glResumeTransformFeedback resumeTransformFeedback;
		ptr_glDrawTransformFeedback drawTransformFeedback;
		ptr_glDrawTransformFeedbackStream drawTransformFeedbackStream;
		ptr_glBeginQueryIndexed beginQueryIndexed;
		ptr_glEndQueryIndexed endQueryIndexed;
		ptr_glGetQueryIndexediv getQueryIndexediv;
		ptr_glReleaseShaderCompiler releaseShaderCompiler;
		ptr_glShaderBinary shaderBinary;
		ptr_glGetShaderPrecisionFormat getShaderPrecisionFormat;
		ptr_glDepthRangef depthRangef;
		ptr_glClearDepthf clearDepthf;
		ptr_glGetProgramBinary getProgramBinary;
		ptr_glProgramBinary programBinary;
		ptr_glProgramParameteri programParameteri;
		ptr_glUseProgramStages useProgramStages;
		ptr_glActiveShaderProgram activeShaderProgram;
		ptr_glCreateShaderProgramv createShaderProgramv;
		ptr_glBindProgramPipeline bindProgramPipeline;
		ptr_glDeleteProgramPipelines deleteProgramPipelines;
		ptr_glGenProgramPipelines genProgramPipelines;
		ptr_glIsProgramPipeline isProgramPipeline;
		ptr_glGetProgramPipelineiv getProgramPipelineiv;
		ptr_glProgramUniform1i programUniform1i;
		ptr_glProgramUniform1iv programUniform1iv;
		ptr_glProgramUniform1f programUniform1f;
		ptr_glProgramUniform1fv programUniform1fv;
		ptr_glProgramUniform1d programUniform1d;
		ptr_glProgramUniform1dv programUniform1dv;
		ptr_glProgramUniform1ui programUniform1ui;
		ptr_glProgramUniform1uiv programUniform1uiv;
		ptr_glProgramUniform2i programUniform2i;
		ptr_glProgramUniform2iv programUniform2iv;
		ptr_glProgramUniform2f programUniform2f;
		ptr_glProgramUniform2fv programUniform2fv;
		ptr_glProgramUniform2d programUniform2d;
		ptr_glProgramUniform2dv programUniform2dv;
		ptr_glProgramUniform2ui programUniform2ui;
		ptr_glProgramUniform2uiv programUniform2uiv;
		ptr_glProgramUniform3i programUniform3i;
		ptr_glProgramUniform3iv programUniform3iv;
		ptr_glProgramUniform3f programUniform3f;
		ptr_glProgramUniform3fv programUniform3fv;
		ptr_glProgramUniform3d programUniform3d;
		ptr_glProgramUniform3dv programUniform3dv;
		ptr_glProgramUniform3ui programUniform3ui;
		ptr_glProgramUniform3uiv programUniform3uiv;
		ptr_glProgramUniform4i programUniform4i;
		ptr_glProgramUniform4iv programUniform4iv;
		ptr_glProgramUniform4f programUniform4f;
		ptr_glProgramUniform4fv programUniform4fv;
		ptr_glProgramUniform4d programUniform4d;
		ptr_glProgramUniform4dv programUniform4dv;
		ptr_glProgramUniform4ui programUniform4ui;
		ptr_glProgramUniform4uiv programUniform4uiv;
		ptr_glProgramUniformMatrix2fv programUniformMatrix2fv;
		ptr_glProgramUniformMatrix3fv programUniformMatrix3fv;
		ptr_glProgramUniformMatrix4fv programUniformMatrix4fv;
		ptr_glProgramUniformMatrix2dv programUniformMatrix2dv;
		ptr_glProgramUniformMatrix3dv programUniformMatrix3dv;
		ptr_glProgramUniformMatrix4dv programUniformMatrix4dv;
		ptr_glProgramUniformMatrix2x3fv programUniformMatrix2x3fv;
		ptr_glProgramUniformMatrix3x2fv programUniformMatrix3x2fv;
		ptr_glProgramUniformMatrix2x4fv programUniformMatrix2x4fv;
		ptr_glProgramUniformMatrix4x2fv programUniformMatrix4x2fv;
		ptr_glProgramUniformMatrix3x4fv programUniformMatrix3x4fv;
		ptr_glProgramUniformMatrix4x3fv programUniformMatrix4x3fv;
		ptr_glProgramUniformMatrix2x3dv programUniformMatrix2x3dv;
		ptr_glProgramUniformMatrix3x2dv programUniformMatrix3x2dv;
		ptr_glProgramUniformMatrix2x4dv programUniformMatrix2x4dv;
		ptr_glProgramUniformMatrix4x2dv programUniformMatrix4x2dv;
		ptr_glProgramUniformMatrix3x4dv programUniformMatrix3x4dv;
		ptr_glProgramUniformMatrix4x3dv programUniformMatrix4x3dv;
		ptr_glValidateProgramPipeline validateProgramPipeline;
		ptr_glGetProgramPipelineInfoLog getProgramPipelineInfoLog;
		ptr_glVertexAttribL1d vertexAttribL1d;
		ptr_glVertexAttribL2d vertexAttribL2d;
		ptr_glVertexAttribL3d vertexAttribL3d;
		ptr_glVertexAttribL4d vertexAttribL4d;
		ptr_glVertexAttribL1dv vertexAttribL1dv;
		ptr_glVertexAttribL2dv vertexAttribL2dv;
		ptr_glVertexAttribL3dv vertexAttribL3dv;
		ptr_glVertexAttribL4dv vertexAttribL4dv;
		ptr_glVertexAttribLPointer vertexAttribLPointer;
		ptr_glGetVertexAttribLdv getVertexAttribLdv;
		ptr_glViewportArrayv viewportArrayv;
		ptr_glViewportIndexedf viewportIndexedf;
		ptr_glViewportIndexedfv viewportIndexedfv;
		ptr_glScissorArrayv scissorArrayv;
		ptr_glScissorIndexed scissorIndexed;
		ptr_glScissorIndexedv scissorIndexedv;
		ptr_glDepthRangeArrayv depthRangeArrayv;
		ptr_glDepthRangeIndexed depthRangeIndexed;
		ptr_glGetFloati_v getFloati_v;
		ptr_glGetDoublei_v getDoublei_v;
		ptr_glCreateSyncFromCLeventARB createSyncFromCLeventARB;
		ptr_glDebugMessageControlARB debugMessageControlARB;
		ptr_glDebugMessageInsertARB debugMessageInsertARB;
		ptr_glDebugMessageCallbackARB debugMessageCallbackARB;
		ptr_glGetDebugMessageLogARB getDebugMessageLogARB;
		ptr_glGetGraphicsResetStatusARB getGraphicsResetStatusARB;
		ptr_glGetnMapdvARB getnMapdvARB;
		ptr_glGetnMapfvARB getnMapfvARB;
		ptr_glGetnMapivARB getnMapivARB;
		ptr_glGetnPixelMapfvARB getnPixelMapfvARB;
		ptr_glGetnPixelMapuivARB getnPixelMapuivARB;
		ptr_glGetnPixelMapusvARB getnPixelMapusvARB;
		ptr_glGetnPolygonStippleARB getnPolygonStippleARB;
		ptr_glGetnColorTableARB getnColorTableARB;
		ptr_glGetnConvolutionFilterARB getnConvolutionFilterARB;
		ptr_glGetnSeparableFilterARB getnSeparableFilterARB;
		ptr_glGetnHistogramARB getnHistogramARB;
		ptr_glGetnMinmaxARB getnMinmaxARB;
		ptr_glGetnTexImageARB getnTexImageARB;
		ptr_glReadnPixelsARB readnPixelsARB;
		ptr_glGetnCompressedTexImageARB getnCompressedTexImageARB;
		ptr_glGetnUniformfvARB getnUniformfvARB;
		ptr_glGetnUniformivARB getnUniformivARB;
		ptr_glGetnUniformuivARB getnUniformuivARB;
		ptr_glGetnUniformdvARB getnUniformdvARB;
		ptr_glDrawArraysInstancedBaseInstance drawArraysInstancedBaseInstance;
		ptr_glDrawElementsInstancedBaseInstance drawElementsInstancedBaseInstance;
		ptr_glDrawElementsInstancedBaseVertexBaseInstance drawElementsInstancedBaseVertexBaseInstance;
		ptr_glDrawTransformFeedbackInstanced drawTransformFeedbackInstanced;
		ptr_glDrawTransformFeedbackStreamInstanced drawTransformFeedbackStreamInstanced;
		ptr_glGetInternalformativ getInternalformativ;
		ptr_glGetActiveAtomicCounterBufferiv getActiveAtomicCounterBufferiv;
		ptr_glBindImageTexture bindImageTexture;
		ptr_glMemoryBarrier memoryBarrier;
		ptr_glTexStorage1D texStorage1D;
		ptr_glTexStorage2D texStorage2D;
		ptr_glTexStorage3D texStorage3D;
		ptr_glTextureStorage1DEXT textureStorage1DEXT;
		ptr_glTextureStorage2DEXT textureStorage2DEXT;
		ptr_glTextureStorage3DEXT textureStorage3DEXT;
		ptr_glDebugMessageControl debugMessageControl;
		ptr_glDebugMessageInsert debugMessageInsert;
		ptr_glDebugMessageCallback debugMessageCallback;
		ptr_glGetDebugMessageLog getDebugMessageLog;
		ptr_glPushDebugGroup pushDebugGroup;
		ptr_glPopDebugGroup popDebugGroup;
		ptr_glObjectLabel objectLabel;
		ptr_glGetObjectLabel getObjectLabel;
		ptr_glObjectPtrLabel objectPtrLabel;
		ptr_glGetObjectPtrLabel getObjectPtrLabel;
		ptr_glClearBufferData clearBufferData;
		ptr_glClearBufferSubData clearBufferSubData;
		ptr_glClearNamedBufferDataEXT clearNamedBufferDataEXT;
		ptr_glClearNamedBufferSubDataEXT clearNamedBufferSubDataEXT;
		ptr_glDispatchCompute dispatchCompute;
		ptr_glDispatchComputeIndirect dispatchComputeIndirect;
		ptr_glCopyImageSubData copyImageSubData;
		ptr_glFramebufferParameteri framebufferParameteri;
		ptr_glGetFramebufferParameteriv getFramebufferParameteriv;
		ptr_glNamedFramebufferParameteriEXT namedFramebufferParameteriEXT;
		ptr_glGetNamedFramebufferParameterivEXT getNamedFramebufferParameterivEXT;
		ptr_glGetInternalformati64v getInternalformati64v;
		ptr_glInvalidateTexSubImage invalidateTexSubImage;
		ptr_glInvalidateTexImage invalidateTexImage;
		ptr_glInvalidateBufferSubData invalidateBufferSubData;
		ptr_glInvalidateBufferData invalidateBufferData;
		ptr_glInvalidateFramebuffer invalidateFramebuffer;
		ptr_glInvalidateSubFramebuffer invalidateSubFramebuffer;
		ptr_glMultiDrawArraysIndirect multiDrawArraysIndirect;
		ptr_glMultiDrawElementsIndirect multiDrawElementsIndirect;
		ptr_glGetProgramInterfaceiv getProgramInterfaceiv;
		ptr_glGetProgramResourceIndex getProgramResourceIndex;
		ptr_glGetProgramResourceName getProgramResourceName;
		ptr_glGetProgramResourceiv getProgramResourceiv;
		ptr_glGetProgramResourceLocation getProgramResourceLocation;
		ptr_glGetProgramResourceLocationIndex getProgramResourceLocationIndex;
		ptr_glShaderStorageBlockBinding shaderStorageBlockBinding;
		ptr_glTexBufferRange texBufferRange;
		ptr_glTextureBufferRangeEXT textureBufferRangeEXT;
		ptr_glTexStorage2DMultisample texStorage2DMultisample;
		ptr_glTexStorage3DMultisample texStorage3DMultisample;
		ptr_glTextureStorage2DMultisampleEXT textureStorage2DMultisampleEXT;
		ptr_glTextureStorage3DMultisampleEXT textureStorage3DMultisampleEXT;
		ptr_glTextureView textureView;
		ptr_glBindVertexBuffer bindVertexBuffer;
		ptr_glVertexAttribFormat vertexAttribFormat;
		ptr_glVertexAttribIFormat vertexAttribIFormat;
		ptr_glVertexAttribLFormat vertexAttribLFormat;
		ptr_glVertexAttribBinding vertexAttribBinding;
		ptr_glVertexBindingDivisor vertexBindingDivisor;
		ptr_glVertexArrayBindVertexBufferEXT vertexArrayBindVertexBufferEXT;
		ptr_glVertexArrayVertexAttribFormatEXT vertexArrayVertexAttribFormatEXT;
		ptr_glVertexArrayVertexAttribIFormatEXT vertexArrayVertexAttribIFormatEXT;
		ptr_glVertexArrayVertexAttribLFormatEXT vertexArrayVertexAttribLFormatEXT;
		ptr_glVertexArrayVertexAttribBindingEXT vertexArrayVertexAttribBindingEXT;
		ptr_glVertexArrayVertexBindingDivisorEXT vertexArrayVertexBindingDivisorEXT;

		// OpenGL 4.4
		ptr_glBufferStorage BufferStorage;
		ptr_glClearTexImage ClearTexImage;
		ptr_glClearTexSubImage ClearTexSubImage;
		ptr_glBindBuffersBase BindBuffersBase;
		ptr_glBindBuffersRange BindBuffersRange;
		ptr_glBindTextures BindTextures;
		ptr_glBindSamplers BindSamplers;
		ptr_glBindImageTextures BindImageTextures;
		ptr_glBindVertexBuffers BindVertexBuffers;

		// For Legacy OpenGL (For GL1 target)
		ptr_glClientActiveTextureARB clientActiveTexture;
	#ifdef WIN32
		ptr_wglCreatePbufferARB wglCreatePbufferARB;
		ptr_wglGetPbufferDCARB wglGetPbufferDCARB;
		ptr_wglReleasePbufferDCARB wglReleasePbufferDCARB;
		ptr_wglDestroyPbufferARB wglDestroyPbufferARB;
		ptr_wglQueryPbufferARB wglQueryPbufferARB;
	#endif

	};

	#define glCullFace uicore::OpenGL::functions->cullFace
	#define glFrontFace uicore::OpenGL::functions->frontFace
	#define glHint uicore::OpenGL::functions->hint
	#define glLineWidth uicore::OpenGL::functions->lineWidth
	#define glPointSize uicore::OpenGL::functions->pointSize
	#define glPolygonMode uicore::OpenGL::functions->polygonMode
	#define glScissor uicore::OpenGL::functions->scissor
	#define glTexParameterf uicore::OpenGL::functions->texParameterf
	#define glTexParameterfv uicore::OpenGL::functions->texParameterfv
	#define glTexParameteri uicore::OpenGL::functions->texParameteri
	#define glTexParameteriv uicore::OpenGL::functions->texParameteriv
	#define glTexImage1D uicore::OpenGL::functions->texImage1D
	#define glTexImage2D uicore::OpenGL::functions->texImage2D
	#define glDrawBuffer uicore::OpenGL::functions->drawBuffer
	#define glClear uicore::OpenGL::functions->clear
	#define glClearColor uicore::OpenGL::functions->clearColor
	#define glClearStencil uicore::OpenGL::functions->clearStencil
	#define glClearDepth uicore::OpenGL::functions->clearDepth
	#define glStencilMask uicore::OpenGL::functions->stencilMask
	#define glColorMask uicore::OpenGL::functions->colorMask
	#define glDepthMask uicore::OpenGL::functions->depthMask
	#define glDisable uicore::OpenGL::functions->disable
	#define glEnable uicore::OpenGL::functions->enable
	#define glFinish uicore::OpenGL::functions->finish
	#define glFlush uicore::OpenGL::functions->flush
	#define glBlendFunc uicore::OpenGL::functions->blendFunc
	#define glLogicOp uicore::OpenGL::functions->logicOp
	#define glStencilFunc uicore::OpenGL::functions->stencilFunc
	#define glStencilOp uicore::OpenGL::functions->stencilOp
	#define glDepthFunc uicore::OpenGL::functions->depthFunc
	#define glPixelStoref uicore::OpenGL::functions->pixelStoref
	#define glPixelStorei uicore::OpenGL::functions->pixelStorei
	#define glReadBuffer uicore::OpenGL::functions->readBuffer
	#define glReadPixels uicore::OpenGL::functions->readPixels
	#define glGetBooleanv uicore::OpenGL::functions->getBooleanv
	#define glGetDoublev uicore::OpenGL::functions->getDoublev
	#define glGetError uicore::OpenGL::functions->getError
	#define glGetFloatv uicore::OpenGL::functions->getFloatv
	#define glGetIntegerv uicore::OpenGL::functions->getIntegerv
	#define glGetString uicore::OpenGL::functions->getString
	#define glGetTexImage uicore::OpenGL::functions->getTexImage
	#define glGetTexParameterfv uicore::OpenGL::functions->getTexParameterfv
	#define glGetTexParameteriv uicore::OpenGL::functions->getTexParameteriv
	#define glGetTexLevelParameterfv uicore::OpenGL::functions->getTexLevelParameterfv
	#define glGetTexLevelParameteriv uicore::OpenGL::functions->getTexLevelParameteriv
	#define glIsEnabled uicore::OpenGL::functions->isEnabled
	#define glDepthRange uicore::OpenGL::functions->depthRange
	#define glViewport uicore::OpenGL::functions->viewport
	#define glDrawArrays uicore::OpenGL::functions->drawArrays
	#define glDrawElements uicore::OpenGL::functions->drawElements
	#define glGetPointerv uicore::OpenGL::functions->getPointerv
	#define glPolygonOffset uicore::OpenGL::functions->polygonOffset
	#define glCopyTexImage1D uicore::OpenGL::functions->copyTexImage1D
	#define glCopyTexImage2D uicore::OpenGL::functions->copyTexImage2D
	#define glCopyTexSubImage1D uicore::OpenGL::functions->copyTexSubImage1D
	#define glCopyTexSubImage2D uicore::OpenGL::functions->copyTexSubImage2D
	#define glTexSubImage1D uicore::OpenGL::functions->texSubImage1D
	#define glTexSubImage2D uicore::OpenGL::functions->texSubImage2D
	#define glBindTexture uicore::OpenGL::functions->bindTexture
	#define glDeleteTextures uicore::OpenGL::functions->deleteTextures
	#define glGenTextures uicore::OpenGL::functions->genTextures
	#define glIsTexture uicore::OpenGL::functions->isTexture
	#define glBlendColor uicore::OpenGL::functions->blendColor
	#define glBlendEquation uicore::OpenGL::functions->blendEquation
	#define glDrawRangeElements uicore::OpenGL::functions->drawRangeElements
	#define glTexImage3D uicore::OpenGL::functions->texImage3D
	#define glTexSubImage3D uicore::OpenGL::functions->texSubImage3D
	#define glCopyTexSubImage3D uicore::OpenGL::functions->copyTexSubImage3D
	#define glActiveTexture uicore::OpenGL::functions->activeTexture
	#define glSampleCoverage uicore::OpenGL::functions->sampleCoverage
	#define glCompressedTexImage3D uicore::OpenGL::functions->compressedTexImage3D
	#define glCompressedTexImage2D uicore::OpenGL::functions->compressedTexImage2D
	#define glCompressedTexImage1D uicore::OpenGL::functions->compressedTexImage1D
	#define glCompressedTexSubImage3D uicore::OpenGL::functions->compressedTexSubImage3D
	#define glCompressedTexSubImage2D uicore::OpenGL::functions->compressedTexSubImage2D
	#define glCompressedTexSubImage1D uicore::OpenGL::functions->compressedTexSubImage1D
	#define glGetCompressedTexImage uicore::OpenGL::functions->getCompressedTexImage
	#define glBlendFuncSeparate uicore::OpenGL::functions->blendFuncSeparate
	#define glMultiDrawArrays uicore::OpenGL::functions->multiDrawArrays
	#define glMultiDrawElements uicore::OpenGL::functions->multiDrawElements
	#define glPointParameterf uicore::OpenGL::functions->pointParameterf
	#define glPointParameterfv uicore::OpenGL::functions->pointParameterfv
	#define glPointParameteri uicore::OpenGL::functions->pointParameteri
	#define glPointParameteriv uicore::OpenGL::functions->pointParameteriv
	#define glGenQueries uicore::OpenGL::functions->genQueries
	#define glDeleteQueries uicore::OpenGL::functions->deleteQueries
	#define glIsQuery uicore::OpenGL::functions->isQuery
	#define glBeginQuery uicore::OpenGL::functions->beginQuery
	#define glEndQuery uicore::OpenGL::functions->endQuery
	#define glGetQueryiv uicore::OpenGL::functions->getQueryiv
	#define glGetQueryObjectiv uicore::OpenGL::functions->getQueryObjectiv
	#define glGetQueryObjectuiv uicore::OpenGL::functions->getQueryObjectuiv
	#define glBindBuffer uicore::OpenGL::functions->bindBuffer
	#define glDeleteBuffers uicore::OpenGL::functions->deleteBuffers
	#define glGenBuffers uicore::OpenGL::functions->genBuffers
	#define glIsBuffer uicore::OpenGL::functions->isBuffer
	#define glBufferData uicore::OpenGL::functions->bufferData
	#define glBufferSubData uicore::OpenGL::functions->bufferSubData
	#define glGetBufferSubData uicore::OpenGL::functions->getBufferSubData
	#define glMapBuffer uicore::OpenGL::functions->mapBuffer
	#define glUnmapBuffer uicore::OpenGL::functions->unmapBuffer
	#define glGetBufferParameteriv uicore::OpenGL::functions->getBufferParameteriv
	#define glGetBufferPointerv uicore::OpenGL::functions->getBufferPointerv
	#define glBlendEquationSeparate uicore::OpenGL::functions->blendEquationSeparate
	#define glDrawBuffers uicore::OpenGL::functions->drawBuffers
	#define glStencilOpSeparate uicore::OpenGL::functions->stencilOpSeparate
	#define glStencilFuncSeparate uicore::OpenGL::functions->stencilFuncSeparate
	#define glStencilMaskSeparate uicore::OpenGL::functions->stencilMaskSeparate
	#define glAttachShader uicore::OpenGL::functions->attachShader
	#define glBindAttribLocation uicore::OpenGL::functions->bindAttribLocation
	#define glCompileShader uicore::OpenGL::functions->compileShader
	#define glCreateProgram uicore::OpenGL::functions->createProgram
	#define glCreateShader uicore::OpenGL::functions->createShader
	#define glDeleteProgram uicore::OpenGL::functions->deleteProgram
	#define glDeleteShader uicore::OpenGL::functions->deleteShader
	#define glDetachShader uicore::OpenGL::functions->detachShader
	#define glDisableVertexAttribArray uicore::OpenGL::functions->disableVertexAttribArray
	#define glEnableVertexAttribArray uicore::OpenGL::functions->enableVertexAttribArray
	#define glGetActiveAttrib uicore::OpenGL::functions->getActiveAttrib
	#define glGetActiveUniform uicore::OpenGL::functions->getActiveUniform
	#define glGetAttachedShaders uicore::OpenGL::functions->getAttachedShaders
	#define glGetAttribLocation uicore::OpenGL::functions->getAttribLocation
	#define glGetProgramiv uicore::OpenGL::functions->getProgramiv
	#define glGetProgramInfoLog uicore::OpenGL::functions->getProgramInfoLog
	#define glGetShaderiv uicore::OpenGL::functions->getShaderiv
	#define glGetShaderInfoLog uicore::OpenGL::functions->getShaderInfoLog
	#define glGetShaderSource uicore::OpenGL::functions->getShaderSource
	#define glGetUniformLocation uicore::OpenGL::functions->getUniformLocation
	#define glGetUniformfv uicore::OpenGL::functions->getUniformfv
	#define glGetUniformiv uicore::OpenGL::functions->getUniformiv
	#define glGetVertexAttribdv uicore::OpenGL::functions->getVertexAttribdv
	#define glGetVertexAttribfv uicore::OpenGL::functions->getVertexAttribfv
	#define glGetVertexAttribiv uicore::OpenGL::functions->getVertexAttribiv
	#define glGetVertexAttribPointerv uicore::OpenGL::functions->getVertexAttribPointerv
	#define glIsProgram uicore::OpenGL::functions->isProgram
	#define glIsShader uicore::OpenGL::functions->isShader
	#define glLinkProgram uicore::OpenGL::functions->linkProgram
	#define glShaderSource uicore::OpenGL::functions->shaderSource
	#define glUseProgram uicore::OpenGL::functions->useProgram
	#define glUniform1f uicore::OpenGL::functions->uniform1f
	#define glUniform2f uicore::OpenGL::functions->uniform2f
	#define glUniform3f uicore::OpenGL::functions->uniform3f
	#define glUniform4f uicore::OpenGL::functions->uniform4f
	#define glUniform1i uicore::OpenGL::functions->uniform1i
	#define glUniform2i uicore::OpenGL::functions->uniform2i
	#define glUniform3i uicore::OpenGL::functions->uniform3i
	#define glUniform4i uicore::OpenGL::functions->uniform4i
	#define glUniform1fv uicore::OpenGL::functions->uniform1fv
	#define glUniform2fv uicore::OpenGL::functions->uniform2fv
	#define glUniform3fv uicore::OpenGL::functions->uniform3fv
	#define glUniform4fv uicore::OpenGL::functions->uniform4fv
	#define glUniform1iv uicore::OpenGL::functions->uniform1iv
	#define glUniform2iv uicore::OpenGL::functions->uniform2iv
	#define glUniform3iv uicore::OpenGL::functions->uniform3iv
	#define glUniform4iv uicore::OpenGL::functions->uniform4iv
	#define glUniformMatrix2fv uicore::OpenGL::functions->uniformMatrix2fv
	#define glUniformMatrix3fv uicore::OpenGL::functions->uniformMatrix3fv
	#define glUniformMatrix4fv uicore::OpenGL::functions->uniformMatrix4fv
	#define glValidateProgram uicore::OpenGL::functions->validateProgram
	#define glVertexAttrib1d uicore::OpenGL::functions->vertexAttrib1d
	#define glVertexAttrib1dv uicore::OpenGL::functions->vertexAttrib1dv
	#define glVertexAttrib1f uicore::OpenGL::functions->vertexAttrib1f
	#define glVertexAttrib1fv uicore::OpenGL::functions->vertexAttrib1fv
	#define glVertexAttrib1s uicore::OpenGL::functions->vertexAttrib1s
	#define glVertexAttrib1sv uicore::OpenGL::functions->vertexAttrib1sv
	#define glVertexAttrib2d uicore::OpenGL::functions->vertexAttrib2d
	#define glVertexAttrib2dv uicore::OpenGL::functions->vertexAttrib2dv
	#define glVertexAttrib2f uicore::OpenGL::functions->vertexAttrib2f
	#define glVertexAttrib2fv uicore::OpenGL::functions->vertexAttrib2fv
	#define glVertexAttrib2s uicore::OpenGL::functions->vertexAttrib2s
	#define glVertexAttrib2sv uicore::OpenGL::functions->vertexAttrib2sv
	#define glVertexAttrib3d uicore::OpenGL::functions->vertexAttrib3d
	#define glVertexAttrib3dv uicore::OpenGL::functions->vertexAttrib3dv
	#define glVertexAttrib3f uicore::OpenGL::functions->vertexAttrib3f
	#define glVertexAttrib3fv uicore::OpenGL::functions->vertexAttrib3fv
	#define glVertexAttrib3s uicore::OpenGL::functions->vertexAttrib3s
	#define glVertexAttrib3sv uicore::OpenGL::functions->vertexAttrib3sv
	#define glVertexAttrib4Nbv uicore::OpenGL::functions->vertexAttrib4Nbv
	#define glVertexAttrib4Niv uicore::OpenGL::functions->vertexAttrib4Niv
	#define glVertexAttrib4Nsv uicore::OpenGL::functions->vertexAttrib4Nsv
	#define glVertexAttrib4Nub uicore::OpenGL::functions->vertexAttrib4Nub
	#define glVertexAttrib4Nubv uicore::OpenGL::functions->vertexAttrib4Nubv
	#define glVertexAttrib4Nuiv uicore::OpenGL::functions->vertexAttrib4Nuiv
	#define glVertexAttrib4Nusv uicore::OpenGL::functions->vertexAttrib4Nusv
	#define glVertexAttrib4bv uicore::OpenGL::functions->vertexAttrib4bv
	#define glVertexAttrib4d uicore::OpenGL::functions->vertexAttrib4d
	#define glVertexAttrib4dv uicore::OpenGL::functions->vertexAttrib4dv
	#define glVertexAttrib4f uicore::OpenGL::functions->vertexAttrib4f
	#define glVertexAttrib4fv uicore::OpenGL::functions->vertexAttrib4fv
	#define glVertexAttrib4iv uicore::OpenGL::functions->vertexAttrib4iv
	#define glVertexAttrib4s uicore::OpenGL::functions->vertexAttrib4s
	#define glVertexAttrib4sv uicore::OpenGL::functions->vertexAttrib4sv
	#define glVertexAttrib4ubv uicore::OpenGL::functions->vertexAttrib4ubv
	#define glVertexAttrib4uiv uicore::OpenGL::functions->vertexAttrib4uiv
	#define glVertexAttrib4usv uicore::OpenGL::functions->vertexAttrib4usv
	#define glVertexAttribPointer uicore::OpenGL::functions->vertexAttribPointer
	#define glUniformMatrix2x3fv uicore::OpenGL::functions->uniformMatrix2x3fv
	#define glUniformMatrix3x2fv uicore::OpenGL::functions->uniformMatrix3x2fv
	#define glUniformMatrix2x4fv uicore::OpenGL::functions->uniformMatrix2x4fv
	#define glUniformMatrix4x2fv uicore::OpenGL::functions->uniformMatrix4x2fv
	#define glUniformMatrix3x4fv uicore::OpenGL::functions->uniformMatrix3x4fv
	#define glUniformMatrix4x3fv uicore::OpenGL::functions->uniformMatrix4x3fv
	#define glColorMaski uicore::OpenGL::functions->colorMaski
	#define glGetBooleani_v uicore::OpenGL::functions->getBooleani_v
	#define glGetIntegeri_v uicore::OpenGL::functions->getIntegeri_v
	#define glEnablei uicore::OpenGL::functions->enablei
	#define glDisablei uicore::OpenGL::functions->disablei
	#define glIsEnabledi uicore::OpenGL::functions->isEnabledi
	#define glBeginTransformFeedback uicore::OpenGL::functions->beginTransformFeedback
	#define glEndTransformFeedback uicore::OpenGL::functions->endTransformFeedback
	#define glBindBufferRange uicore::OpenGL::functions->bindBufferRange
	#define glBindBufferBase uicore::OpenGL::functions->bindBufferBase
	#define glTransformFeedbackVaryings uicore::OpenGL::functions->transformFeedbackVaryings
	#define glGetTransformFeedbackVarying uicore::OpenGL::functions->getTransformFeedbackVarying
	#define glClampColor uicore::OpenGL::functions->clampColor
	#define glBeginConditionalRender uicore::OpenGL::functions->beginConditionalRender
	#define glEndConditionalRender uicore::OpenGL::functions->endConditionalRender
	#define glVertexAttribIPointer uicore::OpenGL::functions->vertexAttribIPointer
	#define glGetVertexAttribIiv uicore::OpenGL::functions->getVertexAttribIiv
	#define glGetVertexAttribIuiv uicore::OpenGL::functions->getVertexAttribIuiv
	#define glVertexAttribI1i uicore::OpenGL::functions->vertexAttribI1i
	#define glVertexAttribI2i uicore::OpenGL::functions->vertexAttribI2i
	#define glVertexAttribI3i uicore::OpenGL::functions->vertexAttribI3i
	#define glVertexAttribI4i uicore::OpenGL::functions->vertexAttribI4i
	#define glVertexAttribI1ui uicore::OpenGL::functions->vertexAttribI1ui
	#define glVertexAttribI2ui uicore::OpenGL::functions->vertexAttribI2ui
	#define glVertexAttribI3ui uicore::OpenGL::functions->vertexAttribI3ui
	#define glVertexAttribI4ui uicore::OpenGL::functions->vertexAttribI4ui
	#define glVertexAttribI1iv uicore::OpenGL::functions->vertexAttribI1iv
	#define glVertexAttribI2iv uicore::OpenGL::functions->vertexAttribI2iv
	#define glVertexAttribI3iv uicore::OpenGL::functions->vertexAttribI3iv
	#define glVertexAttribI4iv uicore::OpenGL::functions->vertexAttribI4iv
	#define glVertexAttribI1uiv uicore::OpenGL::functions->vertexAttribI1uiv
	#define glVertexAttribI2uiv uicore::OpenGL::functions->vertexAttribI2uiv
	#define glVertexAttribI3uiv uicore::OpenGL::functions->vertexAttribI3uiv
	#define glVertexAttribI4uiv uicore::OpenGL::functions->vertexAttribI4uiv
	#define glVertexAttribI4bv uicore::OpenGL::functions->vertexAttribI4bv
	#define glVertexAttribI4sv uicore::OpenGL::functions->vertexAttribI4sv
	#define glVertexAttribI4ubv uicore::OpenGL::functions->vertexAttribI4ubv
	#define glVertexAttribI4usv uicore::OpenGL::functions->vertexAttribI4usv
	#define glGetUniformuiv uicore::OpenGL::functions->getUniformuiv
	#define glBindFragDataLocation uicore::OpenGL::functions->bindFragDataLocation
	#define glGetFragDataLocation uicore::OpenGL::functions->getFragDataLocation
	#define glUniform1ui uicore::OpenGL::functions->uniform1ui
	#define glUniform2ui uicore::OpenGL::functions->uniform2ui
	#define glUniform3ui uicore::OpenGL::functions->uniform3ui
	#define glUniform4ui uicore::OpenGL::functions->uniform4ui
	#define glUniform1uiv uicore::OpenGL::functions->uniform1uiv
	#define glUniform2uiv uicore::OpenGL::functions->uniform2uiv
	#define glUniform3uiv uicore::OpenGL::functions->uniform3uiv
	#define glUniform4uiv uicore::OpenGL::functions->uniform4uiv
	#define glTexParameterIiv uicore::OpenGL::functions->texParameterIiv
	#define glTexParameterIuiv uicore::OpenGL::functions->texParameterIuiv
	#define glGetTexParameterIiv uicore::OpenGL::functions->getTexParameterIiv
	#define glGetTexParameterIuiv uicore::OpenGL::functions->getTexParameterIuiv
	#define glClearBufferiv uicore::OpenGL::functions->clearBufferiv
	#define glClearBufferuiv uicore::OpenGL::functions->clearBufferuiv
	#define glClearBufferfv uicore::OpenGL::functions->clearBufferfv
	#define glClearBufferfi uicore::OpenGL::functions->clearBufferfi
	#define glGetStringi uicore::OpenGL::functions->getStringi
	#define glDrawArraysInstanced uicore::OpenGL::functions->drawArraysInstanced
	#define glDrawElementsInstanced uicore::OpenGL::functions->drawElementsInstanced
	#define glTexBuffer uicore::OpenGL::functions->texBuffer
	#define glPrimitiveRestartIndex uicore::OpenGL::functions->primitiveRestartIndex
	#define glGetInteger64i_v uicore::OpenGL::functions->getInteger64i_v
	#define glGetBufferParameteri64v uicore::OpenGL::functions->getBufferParameteri64v
	#define glFramebufferTexture uicore::OpenGL::functions->framebufferTexture
	#define glVertexAttribDivisor uicore::OpenGL::functions->vertexAttribDivisor
	#define glMinSampleShading uicore::OpenGL::functions->minSampleShading
	#define glBlendEquationi uicore::OpenGL::functions->blendEquationi
	#define glBlendEquationSeparatei uicore::OpenGL::functions->blendEquationSeparatei
	#define glBlendFunci uicore::OpenGL::functions->blendFunci
	#define glBlendFuncSeparatei uicore::OpenGL::functions->blendFuncSeparatei
	#define glIsRenderbuffer uicore::OpenGL::functions->isRenderbuffer
	#define glBindRenderbuffer uicore::OpenGL::functions->bindRenderbuffer
	#define glDeleteRenderbuffers uicore::OpenGL::functions->deleteRenderbuffers
	#define glGenRenderbuffers uicore::OpenGL::functions->genRenderbuffers
	#define glRenderbufferStorage uicore::OpenGL::functions->renderbufferStorage
	#define glGetRenderbufferParameteriv uicore::OpenGL::functions->getRenderbufferParameteriv
	#define glIsFramebuffer uicore::OpenGL::functions->isFramebuffer
	#define glBindFramebuffer uicore::OpenGL::functions->bindFramebuffer
	#define glDeleteFramebuffers uicore::OpenGL::functions->deleteFramebuffers
	#define glGenFramebuffers uicore::OpenGL::functions->genFramebuffers
	#define glCheckFramebufferStatus uicore::OpenGL::functions->checkFramebufferStatus
	#define glFramebufferTexture1D uicore::OpenGL::functions->framebufferTexture1D
	#define glFramebufferTexture2D uicore::OpenGL::functions->framebufferTexture2D
	#define glFramebufferTexture3D uicore::OpenGL::functions->framebufferTexture3D
	#define glFramebufferRenderbuffer uicore::OpenGL::functions->framebufferRenderbuffer
	#define glGetFramebufferAttachmentParameteriv uicore::OpenGL::functions->getFramebufferAttachmentParameteriv
	#define glGenerateMipmap uicore::OpenGL::functions->generateMipmap
	#define glBlitFramebuffer uicore::OpenGL::functions->blitFramebuffer
	#define glRenderbufferStorageMultisample uicore::OpenGL::functions->renderbufferStorageMultisample
	#define glFramebufferTextureLayer uicore::OpenGL::functions->framebufferTextureLayer
	#define glMapBufferRange uicore::OpenGL::functions->mapBufferRange
	#define glFlushMappedBufferRange uicore::OpenGL::functions->flushMappedBufferRange
	#define glBindVertexArray uicore::OpenGL::functions->bindVertexArray
	#define glDeleteVertexArrays uicore::OpenGL::functions->deleteVertexArrays
	#define glGenVertexArrays uicore::OpenGL::functions->genVertexArrays
	#define glIsVertexArray uicore::OpenGL::functions->isVertexArray
	#define glGetUniformIndices uicore::OpenGL::functions->getUniformIndices
	#define glGetActiveUniformsiv uicore::OpenGL::functions->getActiveUniformsiv
	#define glGetActiveUniformName uicore::OpenGL::functions->getActiveUniformName
	#define glGetUniformBlockIndex uicore::OpenGL::functions->getUniformBlockIndex
	#define glGetActiveUniformBlockiv uicore::OpenGL::functions->getActiveUniformBlockiv
	#define glGetActiveUniformBlockName uicore::OpenGL::functions->getActiveUniformBlockName
	#define glUniformBlockBinding uicore::OpenGL::functions->uniformBlockBinding
	#define glCopyBufferSubData uicore::OpenGL::functions->copyBufferSubData
	#define glDrawElementsBaseVertex uicore::OpenGL::functions->drawElementsBaseVertex
	#define glDrawRangeElementsBaseVertex uicore::OpenGL::functions->drawRangeElementsBaseVertex
	#define glDrawElementsInstancedBaseVertex uicore::OpenGL::functions->drawElementsInstancedBaseVertex
	#define glMultiDrawElementsBaseVertex uicore::OpenGL::functions->multiDrawElementsBaseVertex
	#define glProvokingVertex uicore::OpenGL::functions->provokingVertex
	#define glFenceSync uicore::OpenGL::functions->fenceSync
	#define glIsSync uicore::OpenGL::functions->isSync
	#define glDeleteSync uicore::OpenGL::functions->deleteSync
	#define glClientWaitSync uicore::OpenGL::functions->clientWaitSync
	#define glWaitSync uicore::OpenGL::functions->waitSync
	#define glGetInteger64v uicore::OpenGL::functions->getInteger64v
	#define glGetSynciv uicore::OpenGL::functions->getSynciv
	#define glTexImage2DMultisample uicore::OpenGL::functions->texImage2DMultisample
	#define glTexImage3DMultisample uicore::OpenGL::functions->texImage3DMultisample
	#define glGetMultisamplefv uicore::OpenGL::functions->getMultisamplefv
	#define glSampleMaski uicore::OpenGL::functions->sampleMaski
	#define glBlendEquationiARB uicore::OpenGL::functions->blendEquationiARB
	#define glBlendEquationSeparateiARB uicore::OpenGL::functions->blendEquationSeparateiARB
	#define glBlendFunciARB uicore::OpenGL::functions->blendFunciARB
	#define glBlendFuncSeparateiARB uicore::OpenGL::functions->blendFuncSeparateiARB
	#define glMinSampleShadingARB uicore::OpenGL::functions->minSampleShadingARB
	#define glNamedStringARB uicore::OpenGL::functions->namedStringARB
	#define glDeleteNamedStringARB uicore::OpenGL::functions->deleteNamedStringARB
	#define glCompileShaderIncludeARB uicore::OpenGL::functions->compileShaderIncludeARB
	#define glIsNamedStringARB uicore::OpenGL::functions->isNamedStringARB
	#define glGetNamedStringARB uicore::OpenGL::functions->getNamedStringARB
	#define glGetNamedStringivARB uicore::OpenGL::functions->getNamedStringivARB
	#define glBindFragDataLocationIndexed uicore::OpenGL::functions->bindFragDataLocationIndexed
	#define glGetFragDataIndex uicore::OpenGL::functions->getFragDataIndex
	#define glGenSamplers uicore::OpenGL::functions->genSamplers
	#define glDeleteSamplers uicore::OpenGL::functions->deleteSamplers
	#define glIsSampler uicore::OpenGL::functions->isSampler
	#define glBindSampler uicore::OpenGL::functions->bindSampler
	#define glSamplerParameteri uicore::OpenGL::functions->samplerParameteri
	#define glSamplerParameteriv uicore::OpenGL::functions->samplerParameteriv
	#define glSamplerParameterf uicore::OpenGL::functions->samplerParameterf
	#define glSamplerParameterfv uicore::OpenGL::functions->samplerParameterfv
	#define glSamplerParameterIiv uicore::OpenGL::functions->samplerParameterIiv
	#define glSamplerParameterIuiv uicore::OpenGL::functions->samplerParameterIuiv
	#define glGetSamplerParameteriv uicore::OpenGL::functions->getSamplerParameteriv
	#define glGetSamplerParameterIiv uicore::OpenGL::functions->getSamplerParameterIiv
	#define glGetSamplerParameterfv uicore::OpenGL::functions->getSamplerParameterfv
	#define glGetSamplerParameterIuiv uicore::OpenGL::functions->getSamplerParameterIuiv
	#define glQueryCounter uicore::OpenGL::functions->queryCounter
	#define glGetQueryObjecti64v uicore::OpenGL::functions->getQueryObjecti64v
	#define glGetQueryObjectui64v uicore::OpenGL::functions->getQueryObjectui64v
	#define glVertexP2ui uicore::OpenGL::functions->vertexP2ui
	#define glVertexP2uiv uicore::OpenGL::functions->vertexP2uiv
	#define glVertexP3ui uicore::OpenGL::functions->vertexP3ui
	#define glVertexP3uiv uicore::OpenGL::functions->vertexP3uiv
	#define glVertexP4ui uicore::OpenGL::functions->vertexP4ui
	#define glVertexP4uiv uicore::OpenGL::functions->vertexP4uiv
	#define glTexCoordP1ui uicore::OpenGL::functions->texCoordP1ui
	#define glTexCoordP1uiv uicore::OpenGL::functions->texCoordP1uiv
	#define glTexCoordP2ui uicore::OpenGL::functions->texCoordP2ui
	#define glTexCoordP2uiv uicore::OpenGL::functions->texCoordP2uiv
	#define glTexCoordP3ui uicore::OpenGL::functions->texCoordP3ui
	#define glTexCoordP3uiv uicore::OpenGL::functions->texCoordP3uiv
	#define glTexCoordP4ui uicore::OpenGL::functions->texCoordP4ui
	#define glTexCoordP4uiv uicore::OpenGL::functions->texCoordP4uiv
	#define glMultiTexCoordP1ui uicore::OpenGL::functions->multiTexCoordP1ui
	#define glMultiTexCoordP1uiv uicore::OpenGL::functions->multiTexCoordP1uiv
	#define glMultiTexCoordP2ui uicore::OpenGL::functions->multiTexCoordP2ui
	#define glMultiTexCoordP2uiv uicore::OpenGL::functions->multiTexCoordP2uiv
	#define glMultiTexCoordP3ui uicore::OpenGL::functions->multiTexCoordP3ui
	#define glMultiTexCoordP3uiv uicore::OpenGL::functions->multiTexCoordP3uiv
	#define glMultiTexCoordP4ui uicore::OpenGL::functions->multiTexCoordP4ui
	#define glMultiTexCoordP4uiv uicore::OpenGL::functions->multiTexCoordP4uiv
	#define glNormalP3ui uicore::OpenGL::functions->normalP3ui
	#define glNormalP3uiv uicore::OpenGL::functions->normalP3uiv
	#define glColorP3ui uicore::OpenGL::functions->colorP3ui
	#define glColorP3uiv uicore::OpenGL::functions->colorP3uiv
	#define glColorP4ui uicore::OpenGL::functions->colorP4ui
	#define glColorP4uiv uicore::OpenGL::functions->colorP4uiv
	#define glSecondaryColorP3ui uicore::OpenGL::functions->secondaryColorP3ui
	#define glSecondaryColorP3uiv uicore::OpenGL::functions->secondaryColorP3uiv
	#define glVertexAttribP1ui uicore::OpenGL::functions->vertexAttribP1ui
	#define glVertexAttribP1uiv uicore::OpenGL::functions->vertexAttribP1uiv
	#define glVertexAttribP2ui uicore::OpenGL::functions->vertexAttribP2ui
	#define glVertexAttribP2uiv uicore::OpenGL::functions->vertexAttribP2uiv
	#define glVertexAttribP3ui uicore::OpenGL::functions->vertexAttribP3ui
	#define glVertexAttribP3uiv uicore::OpenGL::functions->vertexAttribP3uiv
	#define glVertexAttribP4ui uicore::OpenGL::functions->vertexAttribP4ui
	#define glVertexAttribP4uiv uicore::OpenGL::functions->vertexAttribP4uiv
	#define glDrawArraysIndirect uicore::OpenGL::functions->drawArraysIndirect
	#define glDrawElementsIndirect uicore::OpenGL::functions->drawElementsIndirect
	#define glUniform1d uicore::OpenGL::functions->uniform1d
	#define glUniform2d uicore::OpenGL::functions->uniform2d
	#define glUniform3d uicore::OpenGL::functions->uniform3d
	#define glUniform4d uicore::OpenGL::functions->uniform4d
	#define glUniform1dv uicore::OpenGL::functions->uniform1dv
	#define glUniform2dv uicore::OpenGL::functions->uniform2dv
	#define glUniform3dv uicore::OpenGL::functions->uniform3dv
	#define glUniform4dv uicore::OpenGL::functions->uniform4dv
	#define glUniformMatrix2dv uicore::OpenGL::functions->uniformMatrix2dv
	#define glUniformMatrix3dv uicore::OpenGL::functions->uniformMatrix3dv
	#define glUniformMatrix4dv uicore::OpenGL::functions->uniformMatrix4dv
	#define glUniformMatrix2x3dv uicore::OpenGL::functions->uniformMatrix2x3dv
	#define glUniformMatrix2x4dv uicore::OpenGL::functions->uniformMatrix2x4dv
	#define glUniformMatrix3x2dv uicore::OpenGL::functions->uniformMatrix3x2dv
	#define glUniformMatrix3x4dv uicore::OpenGL::functions->uniformMatrix3x4dv
	#define glUniformMatrix4x2dv uicore::OpenGL::functions->uniformMatrix4x2dv
	#define glUniformMatrix4x3dv uicore::OpenGL::functions->uniformMatrix4x3dv
	#define glGetUniformdv uicore::OpenGL::functions->getUniformdv
	#define glGetSubroutineUniformLocation uicore::OpenGL::functions->getSubroutineUniformLocation
	#define glGetSubroutineIndex uicore::OpenGL::functions->getSubroutineIndex
	#define glGetActiveSubroutineUniformiv uicore::OpenGL::functions->getActiveSubroutineUniformiv
	#define glGetActiveSubroutineUniformName uicore::OpenGL::functions->getActiveSubroutineUniformName
	#define glGetActiveSubroutineName uicore::OpenGL::functions->getActiveSubroutineName
	#define glUniformSubroutinesuiv uicore::OpenGL::functions->uniformSubroutinesuiv
	#define glGetUniformSubroutineuiv uicore::OpenGL::functions->getUniformSubroutineuiv
	#define glGetProgramStageiv uicore::OpenGL::functions->getProgramStageiv
	#define glPatchParameteri uicore::OpenGL::functions->patchParameteri
	#define glPatchParameterfv uicore::OpenGL::functions->patchParameterfv
	#define glBindTransformFeedback uicore::OpenGL::functions->bindTransformFeedback
	#define glDeleteTransformFeedbacks uicore::OpenGL::functions->deleteTransformFeedbacks
	#define glGenTransformFeedbacks uicore::OpenGL::functions->genTransformFeedbacks
	#define glIsTransformFeedback uicore::OpenGL::functions->isTransformFeedback
	#define glPauseTransformFeedback uicore::OpenGL::functions->pauseTransformFeedback
	#define glResumeTransformFeedback uicore::OpenGL::functions->resumeTransformFeedback
	#define glDrawTransformFeedback uicore::OpenGL::functions->drawTransformFeedback
	#define glDrawTransformFeedbackStream uicore::OpenGL::functions->drawTransformFeedbackStream
	#define glBeginQueryIndexed uicore::OpenGL::functions->beginQueryIndexed
	#define glEndQueryIndexed uicore::OpenGL::functions->endQueryIndexed
	#define glGetQueryIndexediv uicore::OpenGL::functions->getQueryIndexediv
	#define glReleaseShaderCompiler uicore::OpenGL::functions->releaseShaderCompiler
	#define glShaderBinary uicore::OpenGL::functions->shaderBinary
	#define glGetShaderPrecisionFormat uicore::OpenGL::functions->getShaderPrecisionFormat
	#define glDepthRangef uicore::OpenGL::functions->depthRangef
	#define glClearDepthf uicore::OpenGL::functions->clearDepthf
	#define glGetProgramBinary uicore::OpenGL::functions->getProgramBinary
	#define glProgramBinary uicore::OpenGL::functions->programBinary
	#define glProgramParameteri uicore::OpenGL::functions->programParameteri
	#define glUseProgramStages uicore::OpenGL::functions->useProgramStages
	#define glActiveShaderProgram uicore::OpenGL::functions->activeShaderProgram
	#define glCreateShaderProgramv uicore::OpenGL::functions->createShaderProgramv
	#define glBindProgramPipeline uicore::OpenGL::functions->bindProgramPipeline
	#define glDeleteProgramPipelines uicore::OpenGL::functions->deleteProgramPipelines
	#define glGenProgramPipelines uicore::OpenGL::functions->genProgramPipelines
	#define glIsProgramPipeline uicore::OpenGL::functions->isProgramPipeline
	#define glGetProgramPipelineiv uicore::OpenGL::functions->getProgramPipelineiv
	#define glProgramUniform1i uicore::OpenGL::functions->programUniform1i
	#define glProgramUniform1iv uicore::OpenGL::functions->programUniform1iv
	#define glProgramUniform1f uicore::OpenGL::functions->programUniform1f
	#define glProgramUniform1fv uicore::OpenGL::functions->programUniform1fv
	#define glProgramUniform1d uicore::OpenGL::functions->programUniform1d
	#define glProgramUniform1dv uicore::OpenGL::functions->programUniform1dv
	#define glProgramUniform1ui uicore::OpenGL::functions->programUniform1ui
	#define glProgramUniform1uiv uicore::OpenGL::functions->programUniform1uiv
	#define glProgramUniform2i uicore::OpenGL::functions->programUniform2i
	#define glProgramUniform2iv uicore::OpenGL::functions->programUniform2iv
	#define glProgramUniform2f uicore::OpenGL::functions->programUniform2f
	#define glProgramUniform2fv uicore::OpenGL::functions->programUniform2fv
	#define glProgramUniform2d uicore::OpenGL::functions->programUniform2d
	#define glProgramUniform2dv uicore::OpenGL::functions->programUniform2dv
	#define glProgramUniform2ui uicore::OpenGL::functions->programUniform2ui
	#define glProgramUniform2uiv uicore::OpenGL::functions->programUniform2uiv
	#define glProgramUniform3i uicore::OpenGL::functions->programUniform3i
	#define glProgramUniform3iv uicore::OpenGL::functions->programUniform3iv
	#define glProgramUniform3f uicore::OpenGL::functions->programUniform3f
	#define glProgramUniform3fv uicore::OpenGL::functions->programUniform3fv
	#define glProgramUniform3d uicore::OpenGL::functions->programUniform3d
	#define glProgramUniform3dv uicore::OpenGL::functions->programUniform3dv
	#define glProgramUniform3ui uicore::OpenGL::functions->programUniform3ui
	#define glProgramUniform3uiv uicore::OpenGL::functions->programUniform3uiv
	#define glProgramUniform4i uicore::OpenGL::functions->programUniform4i
	#define glProgramUniform4iv uicore::OpenGL::functions->programUniform4iv
	#define glProgramUniform4f uicore::OpenGL::functions->programUniform4f
	#define glProgramUniform4fv uicore::OpenGL::functions->programUniform4fv
	#define glProgramUniform4d uicore::OpenGL::functions->programUniform4d
	#define glProgramUniform4dv uicore::OpenGL::functions->programUniform4dv
	#define glProgramUniform4ui uicore::OpenGL::functions->programUniform4ui
	#define glProgramUniform4uiv uicore::OpenGL::functions->programUniform4uiv
	#define glProgramUniformMatrix2fv uicore::OpenGL::functions->programUniformMatrix2fv
	#define glProgramUniformMatrix3fv uicore::OpenGL::functions->programUniformMatrix3fv
	#define glProgramUniformMatrix4fv uicore::OpenGL::functions->programUniformMatrix4fv
	#define glProgramUniformMatrix2dv uicore::OpenGL::functions->programUniformMatrix2dv
	#define glProgramUniformMatrix3dv uicore::OpenGL::functions->programUniformMatrix3dv
	#define glProgramUniformMatrix4dv uicore::OpenGL::functions->programUniformMatrix4dv
	#define glProgramUniformMatrix2x3fv uicore::OpenGL::functions->programUniformMatrix2x3fv
	#define glProgramUniformMatrix3x2fv uicore::OpenGL::functions->programUniformMatrix3x2fv
	#define glProgramUniformMatrix2x4fv uicore::OpenGL::functions->programUniformMatrix2x4fv
	#define glProgramUniformMatrix4x2fv uicore::OpenGL::functions->programUniformMatrix4x2fv
	#define glProgramUniformMatrix3x4fv uicore::OpenGL::functions->programUniformMatrix3x4fv
	#define glProgramUniformMatrix4x3fv uicore::OpenGL::functions->programUniformMatrix4x3fv
	#define glProgramUniformMatrix2x3dv uicore::OpenGL::functions->programUniformMatrix2x3dv
	#define glProgramUniformMatrix3x2dv uicore::OpenGL::functions->programUniformMatrix3x2dv
	#define glProgramUniformMatrix2x4dv uicore::OpenGL::functions->programUniformMatrix2x4dv
	#define glProgramUniformMatrix4x2dv uicore::OpenGL::functions->programUniformMatrix4x2dv
	#define glProgramUniformMatrix3x4dv uicore::OpenGL::functions->programUniformMatrix3x4dv
	#define glProgramUniformMatrix4x3dv uicore::OpenGL::functions->programUniformMatrix4x3dv
	#define glValidateProgramPipeline uicore::OpenGL::functions->validateProgramPipeline
	#define glGetProgramPipelineInfoLog uicore::OpenGL::functions->getProgramPipelineInfoLog
	#define glVertexAttribL1d uicore::OpenGL::functions->vertexAttribL1d
	#define glVertexAttribL2d uicore::OpenGL::functions->vertexAttribL2d
	#define glVertexAttribL3d uicore::OpenGL::functions->vertexAttribL3d
	#define glVertexAttribL4d uicore::OpenGL::functions->vertexAttribL4d
	#define glVertexAttribL1dv uicore::OpenGL::functions->vertexAttribL1dv
	#define glVertexAttribL2dv uicore::OpenGL::functions->vertexAttribL2dv
	#define glVertexAttribL3dv uicore::OpenGL::functions->vertexAttribL3dv
	#define glVertexAttribL4dv uicore::OpenGL::functions->vertexAttribL4dv
	#define glVertexAttribLPointer uicore::OpenGL::functions->vertexAttribLPointer
	#define glGetVertexAttribLdv uicore::OpenGL::functions->getVertexAttribLdv
	#define glViewportArrayv uicore::OpenGL::functions->viewportArrayv
	#define glViewportIndexedf uicore::OpenGL::functions->viewportIndexedf
	#define glViewportIndexedfv uicore::OpenGL::functions->viewportIndexedfv
	#define glScissorArrayv uicore::OpenGL::functions->scissorArrayv
	#define glScissorIndexed uicore::OpenGL::functions->scissorIndexed
	#define glScissorIndexedv uicore::OpenGL::functions->scissorIndexedv
	#define glDepthRangeArrayv uicore::OpenGL::functions->depthRangeArrayv
	#define glDepthRangeIndexed uicore::OpenGL::functions->depthRangeIndexed
	#define glGetFloati_v uicore::OpenGL::functions->getFloati_v
	#define glGetDoublei_v uicore::OpenGL::functions->getDoublei_v
	#define glCreateSyncFromCLeventARB uicore::OpenGL::functions->createSyncFromCLeventARB
	#define glDebugMessageControlARB uicore::OpenGL::functions->debugMessageControlARB
	#define glDebugMessageInsertARB uicore::OpenGL::functions->debugMessageInsertARB
	#define glDebugMessageCallbackARB uicore::OpenGL::functions->debugMessageCallbackARB
	#define glGetDebugMessageLogARB uicore::OpenGL::functions->getDebugMessageLogARB
	#define glGetGraphicsResetStatusARB uicore::OpenGL::functions->getGraphicsResetStatusARB
	#define glGetnMapdvARB uicore::OpenGL::functions->getnMapdvARB
	#define glGetnMapfvARB uicore::OpenGL::functions->getnMapfvARB
	#define glGetnMapivARB uicore::OpenGL::functions->getnMapivARB
	#define glGetnPixelMapfvARB uicore::OpenGL::functions->getnPixelMapfvARB
	#define glGetnPixelMapuivARB uicore::OpenGL::functions->getnPixelMapuivARB
	#define glGetnPixelMapusvARB uicore::OpenGL::functions->getnPixelMapusvARB
	#define glGetnPolygonStippleARB uicore::OpenGL::functions->getnPolygonStippleARB
	#define glGetnColorTableARB uicore::OpenGL::functions->getnColorTableARB
	#define glGetnConvolutionFilterARB uicore::OpenGL::functions->getnConvolutionFilterARB
	#define glGetnSeparableFilterARB uicore::OpenGL::functions->getnSeparableFilterARB
	#define glGetnHistogramARB uicore::OpenGL::functions->getnHistogramARB
	#define glGetnMinmaxARB uicore::OpenGL::functions->getnMinmaxARB
	#define glGetnTexImageARB uicore::OpenGL::functions->getnTexImageARB
	#define glReadnPixelsARB uicore::OpenGL::functions->readnPixelsARB
	#define glGetnCompressedTexImageARB uicore::OpenGL::functions->getnCompressedTexImageARB
	#define glGetnUniformfvARB uicore::OpenGL::functions->getnUniformfvARB
	#define glGetnUniformivARB uicore::OpenGL::functions->getnUniformivARB
	#define glGetnUniformuivARB uicore::OpenGL::functions->getnUniformuivARB
	#define glGetnUniformdvARB uicore::OpenGL::functions->getnUniformdvARB
	#define glDrawArraysInstancedBaseInstance uicore::OpenGL::functions->drawArraysInstancedBaseInstance
	#define glDrawElementsInstancedBaseInstance uicore::OpenGL::functions->drawElementsInstancedBaseInstance
	#define glDrawElementsInstancedBaseVertexBaseInstance uicore::OpenGL::functions->drawElementsInstancedBaseVertexBaseInstance
	#define glDrawTransformFeedbackInstanced uicore::OpenGL::functions->drawTransformFeedbackInstanced
	#define glDrawTransformFeedbackStreamInstanced uicore::OpenGL::functions->drawTransformFeedbackStreamInstanced
	#define glGetInternalformativ uicore::OpenGL::functions->getInternalformativ
	#define glGetActiveAtomicCounterBufferiv uicore::OpenGL::functions->getActiveAtomicCounterBufferiv
	#define glBindImageTexture uicore::OpenGL::functions->bindImageTexture
	#define glMemoryBarrier uicore::OpenGL::functions->memoryBarrier
	#define glTexStorage1D uicore::OpenGL::functions->texStorage1D
	#define glTexStorage2D uicore::OpenGL::functions->texStorage2D
	#define glTexStorage3D uicore::OpenGL::functions->texStorage3D
	#define glTextureStorage1DEXT uicore::OpenGL::functions->textureStorage1DEXT
	#define glTextureStorage2DEXT uicore::OpenGL::functions->textureStorage2DEXT
	#define glTextureStorage3DEXT uicore::OpenGL::functions->textureStorage3DEXT
	#define glDebugMessageControl uicore::OpenGL::functions->debugMessageControl
	#define glDebugMessageInsert uicore::OpenGL::functions->debugMessageInsert
	#define glDebugMessageCallback uicore::OpenGL::functions->debugMessageCallback
	#define glGetDebugMessageLog uicore::OpenGL::functions->getDebugMessageLog
	#define glPushDebugGroup uicore::OpenGL::functions->pushDebugGroup
	#define glPopDebugGroup uicore::OpenGL::functions->popDebugGroup
	#define glObjectLabel uicore::OpenGL::functions->objectLabel
	#define glGetObjectLabel uicore::OpenGL::functions->getObjectLabel
	#define glObjectPtrLabel uicore::OpenGL::functions->objectPtrLabel
	#define glGetObjectPtrLabel uicore::OpenGL::functions->getObjectPtrLabel
	#define glClearBufferData uicore::OpenGL::functions->clearBufferData
	#define glClearBufferSubData uicore::OpenGL::functions->clearBufferSubData
	#define glClearNamedBufferDataEXT uicore::OpenGL::functions->clearNamedBufferDataEXT
	#define glClearNamedBufferSubDataEXT uicore::OpenGL::functions->clearNamedBufferSubDataEXT
	#define glDispatchCompute uicore::OpenGL::functions->dispatchCompute
	#define glDispatchComputeIndirect uicore::OpenGL::functions->dispatchComputeIndirect
	#define glCopyImageSubData uicore::OpenGL::functions->copyImageSubData
	#define glFramebufferParameteri uicore::OpenGL::functions->framebufferParameteri
	#define glGetFramebufferParameteriv uicore::OpenGL::functions->getFramebufferParameteriv
	#define glNamedFramebufferParameteriEXT uicore::OpenGL::functions->namedFramebufferParameteriEXT
	#define glGetNamedFramebufferParameterivEXT uicore::OpenGL::functions->getNamedFramebufferParameterivEXT
	#define glGetInternalformati64v uicore::OpenGL::functions->getInternalformati64v
	#define glInvalidateTexSubImage uicore::OpenGL::functions->invalidateTexSubImage
	#define glInvalidateTexImage uicore::OpenGL::functions->invalidateTexImage
	#define glInvalidateBufferSubData uicore::OpenGL::functions->invalidateBufferSubData
	#define glInvalidateBufferData uicore::OpenGL::functions->invalidateBufferData
	#define glInvalidateFramebuffer uicore::OpenGL::functions->invalidateFramebuffer
	#define glInvalidateSubFramebuffer uicore::OpenGL::functions->invalidateSubFramebuffer
	#define glMultiDrawArraysIndirect uicore::OpenGL::functions->multiDrawArraysIndirect
	#define glMultiDrawElementsIndirect uicore::OpenGL::functions->multiDrawElementsIndirect
	#define glGetProgramInterfaceiv uicore::OpenGL::functions->getProgramInterfaceiv
	#define glGetProgramResourceIndex uicore::OpenGL::functions->getProgramResourceIndex
	#define glGetProgramResourceName uicore::OpenGL::functions->getProgramResourceName
	#define glGetProgramResourceiv uicore::OpenGL::functions->getProgramResourceiv
	#define glGetProgramResourceLocation uicore::OpenGL::functions->getProgramResourceLocation
	#define glGetProgramResourceLocationIndex uicore::OpenGL::functions->getProgramResourceLocationIndex
	#define glShaderStorageBlockBinding uicore::OpenGL::functions->shaderStorageBlockBinding
	#define glTexBufferRange uicore::OpenGL::functions->texBufferRange
	#define glTextureBufferRangeEXT uicore::OpenGL::functions->textureBufferRangeEXT
	#define glTexStorage2DMultisample uicore::OpenGL::functions->texStorage2DMultisample
	#define glTexStorage3DMultisample uicore::OpenGL::functions->texStorage3DMultisample
	#define glTextureStorage2DMultisampleEXT uicore::OpenGL::functions->textureStorage2DMultisampleEXT
	#define glTextureStorage3DMultisampleEXT uicore::OpenGL::functions->textureStorage3DMultisampleEXT
	#define glTextureView uicore::OpenGL::functions->textureView
	#define glBindVertexBuffer uicore::OpenGL::functions->bindVertexBuffer
	#define glVertexAttribFormat uicore::OpenGL::functions->vertexAttribFormat
	#define glVertexAttribIFormat uicore::OpenGL::functions->vertexAttribIFormat
	#define glVertexAttribLFormat uicore::OpenGL::functions->vertexAttribLFormat
	#define glVertexAttribBinding uicore::OpenGL::functions->vertexAttribBinding
	#define glVertexBindingDivisor uicore::OpenGL::functions->vertexBindingDivisor
	#define glVertexArrayBindVertexBufferEXT uicore::OpenGL::functions->vertexArrayBindVertexBufferEXT
	#define glVertexArrayVertexAttribFormatEXT uicore::OpenGL::functions->vertexArrayVertexAttribFormatEXT
	#define glVertexArrayVertexAttribIFormatEXT uicore::OpenGL::functions->vertexArrayVertexAttribIFormatEXT
	#define glVertexArrayVertexAttribLFormatEXT uicore::OpenGL::functions->vertexArrayVertexAttribLFormatEXT
	#define glVertexArrayVertexAttribBindingEXT uicore::OpenGL::functions->vertexArrayVertexAttribBindingEXT
	#define glVertexArrayVertexBindingDivisorEXT uicore::OpenGL::functions->vertexArrayVertexBindingDivisorEXT

	// OpenGL 4.4
	#define glBufferStorage uicore::OpenGL::functions->BufferStorage
	#define glClearTexImage uicore::OpenGL::functions->ClearTexImage
	#define glClearTexSubImage uicore::OpenGL::functions->ClearTexSubImage
	#define glBindBuffersBase uicore::OpenGL::functions->BindBuffersBase
	#define glBindBuffersRange uicore::OpenGL::functions->BindBuffersRange
	#define glBindTextures uicore::OpenGL::functions->BindTextures
	#define glBindSamplers uicore::OpenGL::functions->BindSamplers
	#define glBindImageTextures uicore::OpenGL::functions->BindImageTextures
	#define glBindVertexBuffers uicore::OpenGL::functions->BindVertexBuffers

	// For Legacy OpenGL (For GL1 target)
	#define glClientActiveTexture uicore::OpenGL::functions->clientActiveTexture
	#ifdef WIN32
	#define glWglCreatePbufferARB uicore::OpenGL::functions->wglCreatePbufferARB
	#define glWglGetPbufferDCARB uicore::OpenGL::functions->wglGetPbufferDCARB
	#define glWglReleasePbufferDCARB uicore::OpenGL::functions->wglReleasePbufferDCARB
	#define glWglDestroyPbufferARB uicore::OpenGL::functions->wglDestroyPbufferARB
	#define glWglQueryPbufferARB uicore::OpenGL::functions->wglQueryPbufferARB
	#endif
}
