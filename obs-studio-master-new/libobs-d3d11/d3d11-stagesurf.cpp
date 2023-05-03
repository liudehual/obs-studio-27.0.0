
#include "d3d11-subsystem.hpp"

gs_stage_surface::gs_stage_surface(gs_device_t *device, uint32_t width,
				   uint32_t height, gs_color_format colorFormat)
	: gs_obj(device, gs_type::gs_stage_surface),
	  width(width),
	  height(height),
	  format(colorFormat),
	  dxgiFormat(ConvertGSTextureFormatView(colorFormat))
{
	HRESULT hr;

	memset(&td, 0, sizeof(td));
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = dxgiFormat;
	td.SampleDesc.Count = 1;
	td.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	td.Usage = D3D11_USAGE_STAGING;

	hr = device->device->CreateTexture2D(&td, NULL, texture.Assign());
	if (FAILED(hr))
		throw HRError("Failed to create staging surface", hr);
}

gs_stage_surface::gs_stage_surface(gs_device_t *device, uint32_t width,
				   uint32_t height)
	: gs_obj(device, gs_type::gs_stage_surface),
	  width(width),
	  height(height),
	  format(GS_UNKNOWN),
	  dxgiFormat(DXGI_FORMAT_NV12)
{
	HRESULT hr;

	memset(&td, 0, sizeof(td));
	td.Width = width;
	td.Height = height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = dxgiFormat;
	td.SampleDesc.Count = 1;
	td.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	td.Usage = D3D11_USAGE_STAGING;

	hr = device->device->CreateTexture2D(&td, NULL, texture.Assign());
	if (FAILED(hr))
		throw HRError("Failed to create staging surface", hr);
}
