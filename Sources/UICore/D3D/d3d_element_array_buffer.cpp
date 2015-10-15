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

#include "UICore/precomp.h"
#include "d3d_element_array_buffer.h"
#include "d3d_graphic_context.h"
#include "d3d_display_window.h"
#include "d3d_staging_buffer.h"
#include "UICore/D3D/d3d_target.h"

namespace uicore
{
	D3DElementArrayBuffer::D3DElementArrayBuffer(const ComPtr<ID3D11Device> &device, int size, BufferUsage usage)
		: size(size)
	{
		handles.push_back(std::shared_ptr<DeviceHandles>(new DeviceHandles(device)));

		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = size;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
		desc.StructureByteStride = 0;

		HRESULT result = handles.front()->device->CreateBuffer(&desc, 0, handles.front()->buffer.output_variable());
		D3DTarget::throw_if_failed("Unable to create element array buffer", result);
	}

	D3DElementArrayBuffer::D3DElementArrayBuffer(const ComPtr<ID3D11Device> &device, const void *data, int size, BufferUsage usage)
		: size(size)
	{
		handles.push_back(std::shared_ptr<DeviceHandles>(new DeviceHandles(device)));

		D3D11_SUBRESOURCE_DATA resource_data;
		resource_data.pSysMem = data;
		resource_data.SysMemPitch = 0;
		resource_data.SysMemSlicePitch = 0;

		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = size;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
		desc.StructureByteStride = 0;

		HRESULT result = handles.front()->device->CreateBuffer(&desc, &resource_data, handles.front()->buffer.output_variable());
		D3DTarget::throw_if_failed("Unable to create element array buffer", result);
	}

	D3DElementArrayBuffer::~D3DElementArrayBuffer()
	{
	}

	ComPtr<ID3D11Buffer> &D3DElementArrayBuffer::get_buffer(const ComPtr<ID3D11Device> &device)
	{
		if (device)
			return get_handles(device).buffer;
		else
			return handles.front()->buffer;
	}

	void D3DElementArrayBuffer::upload_data(const GraphicContextPtr &gc, const void *data, int data_size)
	{
		if (data_size != size)
			throw Exception("Upload data size does not match vertex array buffer");

		const ComPtr<ID3D11Device> &device = static_cast<D3DGraphicContext*>(gc.get())->get_window()->get_device();
		ComPtr<ID3D11DeviceContext> device_context;
		device->GetImmediateContext(device_context.output_variable());
		device_context->UpdateSubresource(get_handles(device).buffer, 0, 0, data, 0, 0);
	}

	void D3DElementArrayBuffer::copy_from(const GraphicContextPtr &gc, const StagingBufferPtr &buffer, int dest_pos, int src_pos, int copy_size)
	{
		const ComPtr<ID3D11Device> &device = static_cast<D3DGraphicContext*>(gc.get())->get_window()->get_device();
		ComPtr<ID3D11Buffer> &staging_buffer = static_cast<D3DStagingBuffer*>(buffer.get())->get_buffer(device);
		int staging_buffer_size = static_cast<D3DStagingBuffer*>(buffer.get())->get_size();

		if (copy_size == -1)
			copy_size = staging_buffer_size;

		if (dest_pos < 0 || copy_size < 0 || dest_pos + copy_size > size || src_pos < 0 || src_pos + copy_size > staging_buffer_size)
			throw Exception("Out of bounds!");

		ComPtr<ID3D11DeviceContext> device_context;
		device->GetImmediateContext(device_context.output_variable());

		D3D11_BOX box;
		box.left = src_pos;
		box.right = src_pos + copy_size;
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;
		device_context->CopySubresourceRegion(get_handles(device).buffer, 0, dest_pos, 0, 0, staging_buffer, 0, &box);
	}

	void D3DElementArrayBuffer::copy_to(const GraphicContextPtr &gc, const StagingBufferPtr &buffer, int dest_pos, int src_pos, int copy_size)
	{
		const ComPtr<ID3D11Device> &device = static_cast<D3DGraphicContext*>(gc.get())->get_window()->get_device();
		ComPtr<ID3D11Buffer> &staging_buffer = static_cast<D3DStagingBuffer*>(buffer.get())->get_buffer(device);
		int staging_buffer_size = static_cast<D3DStagingBuffer*>(buffer.get())->get_size();

		if (copy_size == -1)
			copy_size = staging_buffer_size;

		if (dest_pos < 0 || copy_size < 0 || dest_pos + copy_size > staging_buffer_size || src_pos < 0 || src_pos + copy_size > size)
			throw Exception("Out of bounds!");

		ComPtr<ID3D11DeviceContext> device_context;
		device->GetImmediateContext(device_context.output_variable());

		D3D11_BOX box;
		box.left = dest_pos;
		box.right = dest_pos + copy_size;
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;
		device_context->CopySubresourceRegion(staging_buffer, 0, src_pos, 0, 0, get_handles(device).buffer, 0, &box);
	}

	void D3DElementArrayBuffer::device_destroyed(ID3D11Device *device)
	{
		for (size_t i = 0; i < handles.size(); i++)
		{
			if (handles[i]->device.get() == device)
			{
				handles.erase(handles.begin() + i);
				return;
			}
		}
	}

	D3DElementArrayBuffer::DeviceHandles &D3DElementArrayBuffer::get_handles(const ComPtr<ID3D11Device> &device)
	{
		for (size_t i = 0; i < handles.size(); i++)
			if (handles[i]->device == device)
				return *handles[i];

		ComPtr<IDXGIResource> resource;
		HRESULT result = handles.front()->buffer->QueryInterface(__uuidof(IDXGIResource), (void**)resource.output_variable());
		D3DTarget::throw_if_failed("ID3D11Buffer.QueryInterface(IDXGIResource) failed", result);

		HANDLE handle = 0;
		result = resource->GetSharedHandle(&handle);
		D3DTarget::throw_if_failed("IDXGIResource.GetSharedHandle failed", result);

		ComPtr<ID3D11Buffer> buffer_handle;
		result = device->OpenSharedResource(handle, __uuidof(ID3D11Buffer), (void**)buffer_handle.output_variable());
		D3DTarget::throw_if_failed("ID3D11Device.OpenSharedResource failed", result);

		handles.push_back(std::shared_ptr<DeviceHandles>(new DeviceHandles(device)));
		handles.back()->buffer = buffer_handle;
		return *handles.back();
	}

	D3D11_MAP D3DElementArrayBuffer::to_d3d_map_type(BufferAccess access)
	{
		switch (access)
		{
		case access_read_only:
			return D3D11_MAP_READ;
		case access_write_only:
			return D3D11_MAP_WRITE;
		case access_write_discard:
			return D3D11_MAP_WRITE_DISCARD;
		case access_read_write:
			return D3D11_MAP_READ_WRITE;
		}
		throw Exception("Unsupported access type");
	}
}
