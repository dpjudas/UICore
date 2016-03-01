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
**    Harry Storbacka
*/

#pragma once

#include "UICore/Display/Render/program_object_impl.h"
#include "UICore/Display/Render/shader_object.h"
#include "UICore/Display/Render/uniform_buffer.h"
#include "UICore/Core/Math/vec4.h"
#include "UICore/Core/System/comptr.h"
#include "UICore/Core/System/databuffer.h"
#include <memory>
#include <map>

namespace uicore
{
	class D3DShaderObject;
	class D3DUniformBuffer;

	class D3DUniform
	{
	public:
		enum Type { type_sampler, type_texture, type_image };

		D3DUniform() : type(type_sampler), value(0) { for (int i = 0; i < (int)ShaderType::num_types; i++) shader_index[i] = -1; }
		D3DUniform(Type type) : type(type), value(0) { for (int i = 0; i < (int)ShaderType::num_types; i++) shader_index[i] = -1; }

		Type type;
		int shader_index[(int)ShaderType::num_types];
		int value;
	};

	class D3DUniformBlock
	{
	public:
		D3DUniformBlock() : value(0) { for (int i = 0; i < (int)ShaderType::num_types; i++) shader_index[i] = -1; }

		int shader_index[(int)ShaderType::num_types];
		int value;
	};

	class D3DStorageBlock
	{
	public:
		D3DStorageBlock() : value(0) { for (int i = 0; i < (int)ShaderType::num_types; i++) { shader_srv_index[i] = -1; shader_uav_index[i] = -1; } }

		int shader_uav_index[(int)ShaderType::num_types];
		int shader_srv_index[(int)ShaderType::num_types];
		int value;
	};

	class D3DProgramObject : public ProgramObjectImpl
	{
	public:
		D3DProgramObject(const ComPtr<ID3D11Device> &device, const ComPtr<ID3D11DeviceContext> &device_context);
		~D3DProgramObject();

		std::string info_log() const override;
		std::vector<ShaderObjectPtr> shaders() const override;
		int attribute_location(const std::string &name) const override;
		int uniform_location(const std::string &name) const override;

		int uniform_buffer_size(int block_index) const override;
		int uniform_buffer_index(const std::string &block_name) const override;
		int storage_buffer_index(const std::string &name) const override;

		DataBufferPtr &get_shader_bytecode(ShaderType shader_type);
		D3DShaderObject *get_shader_provider(ShaderType shader_type);

		struct AttributeBinding
		{
			AttributeBinding() : semantic_index(0) { }
			std::string semantic_name;
			int semantic_index;
		};
		std::map<int, AttributeBinding> attribute_bindings;

		std::vector<D3DUniform> uniforms;
		std::map<std::string, int> uniform_names;

		std::vector<D3DUniformBlock> uniform_blocks;
		std::map<std::string, int> uniform_block_names;

		std::vector<D3DStorageBlock> storage_blocks;
		std::map<std::string, int> storage_block_names;

		void attach(const ShaderObjectPtr &obj) override;
		void detach(const ShaderObjectPtr &obj) override;
		void bind_attribute_location(int index, const std::string &name) override;
		void bind_frag_data_location(int color_number, const std::string &name) override;
		bool try_link() override;
		bool validate() override;

		void set_uniform1i(int location, int) override;
		void set_uniform2i(int location, int, int) override;
		void set_uniform3i(int location, int, int, int) override;
		void set_uniform4i(int location, int, int, int, int) override;
		void set_uniformiv(int location, int size, int count, const int *data) override;
		void set_uniform1f(int location, float) override;
		void set_uniform2f(int location, float, float) override;
		void set_uniform3f(int location, float, float, float) override;
		void set_uniform4f(int location, float, float, float, float) override;
		void set_uniformfv(int location, int size, int count, const float *data) override;
		void set_uniform_matrix(int location, int size, int count, bool transpose, const float *data) override;
		void set_uniform_buffer_index(int block_index, int bind_index) override;
		void set_storage_buffer_index(int buffer_index, int bind_unit_index) override;

	private:
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> device_context;
		ShaderObjectPtr _shaders[(int)ShaderType::num_types];
	};
}
