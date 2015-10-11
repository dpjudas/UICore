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
**    Harry Storbacka
**    Kenneth Gangstoe
*/

#pragma once

#include <memory>
#include "shader_object.h"

namespace uicore
{
	class IODevice;
	typedef std::shared_ptr<IODevice> IODevicePtr;
	class GraphicContext;
	class ShaderObject;
	typedef std::shared_ptr<ShaderObject> ShaderObjectPtr;

	/// \brief Program Object
	class ProgramObject
	{
	public:
		/// \brief Constructs a ProgramObject
		static std::shared_ptr<ProgramObject> create(const GraphicContextPtr &gc);

		/// \brief Load
		static std::shared_ptr<ProgramObject> load(const GraphicContextPtr &gc, const std::string &vertex_fullname, const std::string &fragment_fullname);
		static std::shared_ptr<ProgramObject> load(const GraphicContextPtr &gc, const std::string &vertex_fullname, const std::string &geometry_fullname, const std::string &fragment_fullname);
		static std::shared_ptr<ProgramObject> load(const GraphicContextPtr &gc, const IODevicePtr &vertex_file, const IODevicePtr &fragment_file);
		static std::shared_ptr<ProgramObject> load(const GraphicContextPtr &gc, const IODevicePtr &vertex_file, const IODevicePtr &geometry_file, const IODevicePtr &fragment_file);

		/// \brief Load and link
		static std::shared_ptr<ProgramObject> load_and_link(const GraphicContextPtr &gc, const std::string &vertex_fullname, const std::string &fragment_fullname);
		static std::shared_ptr<ProgramObject> load_and_link(const GraphicContextPtr &gc, const std::string &vertex_fullname, const std::string &geometry_fullname, const std::string &fragment_fullname);
		static std::shared_ptr<ProgramObject> load_and_link(const GraphicContextPtr &gc, const IODevicePtr &vertex_file, const IODevicePtr &fragment_file);
		static std::shared_ptr<ProgramObject> load_and_link(const GraphicContextPtr &gc, const IODevicePtr &vertex_file, const IODevicePtr &geometry_file, const IODevicePtr &fragment_file);

		/// \brief Returns the shaders attached to the program object.
		virtual std::vector<ShaderObjectPtr> get_shaders() const = 0;

		/// \brief Returns the current info log for the program object.
		virtual std::string get_info_log() const = 0;

		/// \brief Returns the location of a named active attribute.
		///
		/// Returns -1 when unknown
		virtual int get_attribute_location(const std::string &name) const = 0;

		/// \brief Returns the location of a named uniform variable.
		///
		/// Returns -1 when unknown
		virtual int get_uniform_location(const std::string &name) const = 0;

		/// \brief Get the uniform block size
		///
		/// An exception is thrown of block_name was not found
		///
		/// Returns -1 when unknown
		virtual int get_uniform_buffer_size(const std::string &block_name) const = 0;

		/// \brief Get the uniform block size
		virtual int get_uniform_buffer_size(int block_index) const = 0;

		/// \brief Get the uniform block index
		///
		/// Returns -1 when the block index was not found
		virtual int get_uniform_buffer_index(const std::string &block_name) const = 0;

		/// \brief Get the storage buffer index
		///
		/// Returns -1 when the block index was not found
		virtual int get_storage_buffer_index(const std::string &name) const = 0;

		/// \brief Add shader to program object.
		virtual void attach(const ShaderObjectPtr &obj) = 0;

		/// \brief Remove shader from program object.
		virtual void detach(const ShaderObjectPtr &obj) = 0;

		/// \brief Bind attribute to specific location.
		///
		/// This function must be called before linking.
		virtual void bind_attribute_location(int index, const std::string &name) = 0;

		/// \brief Bind shader out variable a specific color buffer location.
		///
		/// This function must be called before linking.
		virtual void bind_frag_data_location(int color_number, const std::string &name) = 0;

		/// \brief Link program.
		///
		/// If the linking fails, get_info_log() will return the link log
		virtual bool try_link() = 0;

		/// \brief Link program.
		///
		/// If the linking fails an exception will be thrown
		virtual void link() = 0;

		/// \brief Validate program.
		///
		/// If the validation fails, get_info_log() will return the validation log
		virtual bool validate() = 0;

		virtual void set_uniform1i(int location, int value_a) = 0;
		virtual void set_uniform2i(int location, int value_a, int value_b) = 0;
		virtual void set_uniform3i(int location, int value_a, int value_b, int value_c) = 0;
		virtual void set_uniform4i(int location, int value_a, int value_b, int value_c, int value_d) = 0;
		virtual void set_uniformiv(int location, int size, int count, const int *data) = 0;
		void set_uniform2i(int location, const Vec2i &vec) { set_uniform2i(location, vec.x, vec.y); }
		void set_uniform3i(int location, const Vec3i &vec) { set_uniform3i(location, vec.x, vec.y, vec.z); }
		void set_uniform4i(int location, const Vec4i &vec) { set_uniform4i(location, vec.x, vec.y, vec.z, vec.w); }
		void set_uniform2s(int location, const Vec2s &vec) { set_uniform2i(location, vec.x, vec.y); }
		void set_uniform3s(int location, const Vec3s &vec) { set_uniform3i(location, vec.x, vec.y, vec.z); }
		void set_uniform4s(int location, const Vec4s &vec) { set_uniform4i(location, vec.x, vec.y, vec.z, vec.w); }
		void set_uniform2b(int location, const Vec2b &vec) { set_uniform2i(location, vec.x, vec.y); }
		void set_uniform3b(int location, const Vec3b &vec) { set_uniform3i(location, vec.x, vec.y, vec.z); }
		void set_uniform4b(int location, const Vec4b &vec) { set_uniform4i(location, vec.x, vec.y, vec.z, vec.w); }
		void set_uniformiv(int location, int count, const Vec2i *data) { set_uniformiv(location, 2, count, &data->x); }
		void set_uniformiv(int location, int count, const Vec3i *data) { set_uniformiv(location, 3, count, &data->x); }
		void set_uniformiv(int location, int count, const Vec4i *data) { set_uniformiv(location, 4, count, &data->x); }
		virtual void set_uniform1f(int location, float value_a) = 0;
		virtual void set_uniform2f(int location, float value_a, float value_b) = 0;
		virtual void set_uniform3f(int location, float value_a, float value_b, float value_c) = 0;
		virtual void set_uniform4f(int location, float value_a, float value_b, float value_c, float value_d) = 0;
		virtual void set_uniformfv(int location, int size, int count, const float *data) = 0;
		void set_uniform2f(int location, const Vec2f &vec) { set_uniform2f(location, vec.x, vec.y); }
		void set_uniform3f(int location, const Vec3f &vec) { set_uniform3f(location, vec.x, vec.y, vec.z); }
		void set_uniform4f(int location, const Vec4f &vec) { set_uniform4f(location, vec.x, vec.y, vec.z, vec.w); }
		void set_uniformfv(int location, int count, const Vec2f *data) { set_uniformfv(location, 2, count, &data->x); }
		void set_uniformfv(int location, int count, const Vec3f *data) { set_uniformfv(location, 3, count, &data->x); }
		void set_uniformfv(int location, int count, const Vec4f *data) { set_uniformfv(location, 4, count, &data->x); }
		virtual void set_uniform_matrix(int location, int size, int count, bool transpose, const float *data) = 0;
		void set_uniform_matrix(int location, const Mat2f &matrix) { set_uniform_matrix(location, 2, 1, false, matrix.matrix); }
		void set_uniform_matrix(int location, const Mat3f &matrix) { set_uniform_matrix(location, 3, 1, false, matrix.matrix); }
		void set_uniform_matrix(int location, const Mat4f &matrix) { set_uniform_matrix(location, 4, 1, false, matrix.matrix); }
		void set_uniform_matrix(int location, int count, const Mat2f *matrix) { set_uniform_matrix(location, 2, count, false, matrix->matrix); }
		void set_uniform_matrix(int location, int count, const Mat3f *matrix) { set_uniform_matrix(location, 3, count, false, matrix->matrix); }
		void set_uniform_matrix(int location, int count, const Mat4f *matrix) { set_uniform_matrix(location, 4, count, false, matrix->matrix); }

		void set_uniform1i(const std::string &name, int value_a) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform1i(loc, value_a); }
		void set_uniform2i(const std::string &name, int value_a, int value_b) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform2i(loc, value_a, value_b); }
		void set_uniform3i(const std::string &name, int value_a, int value_b, int value_c) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform3i(loc, value_a, value_b, value_c); }
		void set_uniform4i(const std::string &name, int value_a, int value_b, int value_c, int value_d) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform4i(loc, value_a, value_b, value_c, value_d); }
		void set_uniformiv(const std::string &name, int size, int count, const int *data) { int loc = get_uniform_location(name); if (loc >= 0) set_uniformiv(loc, size, count, data); }
		void set_uniform2i(const std::string &name, const Vec2i &vec) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform2i(loc, vec); }
		void set_uniform3i(const std::string &name, const Vec3i &vec) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform3i(loc, vec); }
		void set_uniform4i(const std::string &name, const Vec4i &vec) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform4i(loc, vec); }
		void set_uniform2s(const std::string &name, const Vec2s &vec) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform2s(loc, vec); }
		void set_uniform3s(const std::string &name, const Vec3s &vec) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform3s(loc, vec); }
		void set_uniform4s(const std::string &name, const Vec4s &vec) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform4s(loc, vec); }
		void set_uniform2b(const std::string &name, const Vec2b &vec) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform2b(loc, vec); }
		void set_uniform3b(const std::string &name, const Vec3b &vec) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform3b(loc, vec); }
		void set_uniform4b(const std::string &name, const Vec4b &vec) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform4b(loc, vec); }
		void set_uniformiv(const std::string &name, int count, const Vec2i *data) { int loc = get_uniform_location(name); if (loc >= 0) set_uniformiv(loc, count, data); }
		void set_uniformiv(const std::string &name, int count, const Vec3i *data) { int loc = get_uniform_location(name); if (loc >= 0) set_uniformiv(loc, count, data); }
		void set_uniformiv(const std::string &name, int count, const Vec4i *data) { int loc = get_uniform_location(name); if (loc >= 0) set_uniformiv(loc, count, data); }
		void set_uniform1f(const std::string &name, float value_a) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform1f(loc, value_a); }
		void set_uniform2f(const std::string &name, float value_a, float value_b) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform2f(loc, value_a, value_b); }
		void set_uniform3f(const std::string &name, float value_a, float value_b, float value_c) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform3f(loc, value_a, value_b, value_c); }
		void set_uniform4f(const std::string &name, float value_a, float value_b, float value_c, float value_d) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform4f(loc, value_a, value_b, value_c, value_d); }
		void set_uniformfv(const std::string &name, int size, int count, const float *data) { int loc = get_uniform_location(name); if (loc >= 0) set_uniformfv(loc, size, count, data); }
		void set_uniform2f(const std::string &name, const Vec2f &vec) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform2f(loc, vec); }
		void set_uniform3f(const std::string &name, const Vec3f &vec) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform3f(loc, vec); }
		void set_uniform4f(const std::string &name, const Vec4f &vec) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform4f(loc, vec); }
		void set_uniformfv(const std::string &name, int count, const Vec2f *data) { int loc = get_uniform_location(name); if (loc >= 0) set_uniformfv(loc, count, data); }
		void set_uniformfv(const std::string &name, int count, const Vec3f *data) { int loc = get_uniform_location(name); if (loc >= 0) set_uniformfv(loc, count, data); }
		void set_uniformfv(const std::string &name, int count, const Vec4f *data) { int loc = get_uniform_location(name); if (loc >= 0) set_uniformfv(loc, count, data); }
		void set_uniform_matrix(const std::string &name, int size, int count, bool transpose, const float *data) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform_matrix(loc, size, count, transpose, data); }
		void set_uniform_matrix(const std::string &name, const Mat2f &matrix) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform_matrix(loc, matrix); }
		void set_uniform_matrix(const std::string &name, const Mat3f &matrix) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform_matrix(loc, matrix); }
		void set_uniform_matrix(const std::string &name, const Mat4f &matrix) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform_matrix(loc, matrix); }
		void set_uniform_matrix(const std::string &name, int count, const Mat2f *matrix) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform_matrix(loc, count, matrix); }
		void set_uniform_matrix(const std::string &name, int count, const Mat3f *matrix) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform_matrix(loc, count, matrix); }
		void set_uniform_matrix(const std::string &name, int count, const Mat4f *matrix) { int loc = get_uniform_location(name); if (loc >= 0) set_uniform_matrix(loc, count, matrix); }

		virtual void set_uniform_buffer_index(const std::string &block_name, int bind_index) = 0;
		virtual void set_uniform_buffer_index(int block_index, int bind_index) = 0;

		virtual void set_storage_buffer_index(const std::string &block_name, int bind_index) = 0;
		virtual void set_storage_buffer_index(int block_index, int bind_index) = 0;
	};

	typedef std::shared_ptr<ProgramObject> ProgramObjectPtr;
}
