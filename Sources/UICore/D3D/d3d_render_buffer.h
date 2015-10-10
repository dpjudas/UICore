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
*/

#pragma once

#include "UICore/Display/Render/render_buffer.h"
#include "UICore/Display/Image/pixel_buffer.h"
#include "UICore/Core/System/comptr.h"

namespace uicore
{
	class D3DRenderBuffer : public RenderBuffer
	{
	public:
		D3DRenderBuffer(const ComPtr<ID3D11Device> &device, int width, int height, TextureFormat texture_format, int multisample_samples);
		~D3DRenderBuffer();

		ComPtr<ID3D11Texture2D> &get_texture(const ComPtr<ID3D11Device> &device);
		ComPtr<ID3D11RenderTargetView> create_rtv(const ComPtr<ID3D11Device> &device);
		ComPtr<ID3D11DepthStencilView> create_dsv(const ComPtr<ID3D11Device> &device);

		Size size() const override { return _size; }

	private:
		struct DeviceHandles
		{
			DeviceHandles(const ComPtr<ID3D11Device> &device) : device(device) { }

			ComPtr<ID3D11Device> device;
			ComPtr<ID3D11Texture2D> texture;
		};

		void device_destroyed(ID3D11Device *device);
		DeviceHandles &get_handles(const ComPtr<ID3D11Device> &device);

		Size _size;
		std::vector<std::shared_ptr<DeviceHandles> > handles;
	};
}
