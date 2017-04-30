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

#include "UICore/Display/Render/storage_buffer.h"
#include "UICore/Core/System/comptr.h"

namespace uicore
{
	class D3DStorageBuffer : public StorageBuffer
	{
	public:
		D3DStorageBuffer(const ComPtr<ID3D11Device> &device, int size, int stride, BufferUsage usage);
		D3DStorageBuffer(const ComPtr<ID3D11Device> &device, const void *data, int size, int stride, BufferUsage usage);
		~D3DStorageBuffer();

		ComPtr<ID3D11Buffer> &get_buffer(const ComPtr<ID3D11Device> &device);
		ComPtr<ID3D11UnorderedAccessView> &get_uav(const ComPtr<ID3D11Device> &device);
		ComPtr<ID3D11ShaderResourceView> &get_srv(const ComPtr<ID3D11Device> &device);

		void upload_data(const std::shared_ptr<GraphicContext> &gc, const void *data, int size) override;
		void copy_from(const std::shared_ptr<GraphicContext> &gc, const std::shared_ptr<StagingBuffer> &buffer, int dest_pos, int src_pos, int size) override;
		void copy_to(const std::shared_ptr<GraphicContext> &gc, const std::shared_ptr<StagingBuffer> &buffer, int dest_pos, int src_pos, int size) override;

	private:
		struct DeviceHandles
		{
			DeviceHandles(const ComPtr<ID3D11Device> &device) : device(device) { }
			ComPtr<ID3D11Device> device;
			ComPtr<ID3D11Buffer> buffer;
			ComPtr<ID3D11ShaderResourceView> srv;
			ComPtr<ID3D11UnorderedAccessView> uav;
		};

		void device_destroyed(ID3D11Device *device);
		DeviceHandles &get_handles(const ComPtr<ID3D11Device> &device);

		static D3D11_MAP to_d3d_map_type(BufferAccess access);

		std::vector<std::shared_ptr<DeviceHandles> > handles;
		int size;
	};
}
