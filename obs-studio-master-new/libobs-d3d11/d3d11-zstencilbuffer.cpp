#include "d3d11-subsystem.hpp"

void gs_zstencil_buffer::InitBuffer()
{
	HRESULT hr;

	memset(&td, 0, sizeof(td));
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = dxgiFormat;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.SampleDesc.Count = 1;
	td.Usage = D3D11_USAGE_DEFAULT;

	hr = device->device->CreateTexture2D(&td, NULL, texture.Assign());
	if (FAILED(hr))
		throw HRError("Failed to create depth stencil texture", hr);

	memset(&dsvd, 0, sizeof(dsvd));
	dsvd.Format = dxgiFormat;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	hr = device->device->CreateDepthStencilView(texture, &dsvd,
						    view.Assign());
	if (FAILED(hr))
		throw HRError("Failed to create depth stencil view", hr);
}

gs_zstencil_buffer::gs_zstencil_buffer(gs_device_t *device, uint32_t width,
				       uint32_t height,
				       gs_zstencil_format format)
	: gs_obj(device, gs_type::gs_zstencil_buffer),
	  width(width),
	  height(height),
	  format(format),
	  dxgiFormat(ConvertGSZStencilFormat(format))
{
	InitBuffer();
}
