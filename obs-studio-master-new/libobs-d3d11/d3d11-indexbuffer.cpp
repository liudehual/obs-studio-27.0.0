
#include "d3d11-subsystem.hpp"

void gs_index_buffer::InitBuffer()
{
	HRESULT hr;

	memset(&bd, 0, sizeof(bd));
	memset(&srd, 0, sizeof(srd));

	bd.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.ByteWidth = UINT(indexSize * num);
	srd.pSysMem = indices.data;

	hr = device->device->CreateBuffer(&bd, &srd, indexBuffer.Assign());
	if (FAILED(hr))
		throw HRError("Failed to create buffer", hr);
}

gs_index_buffer::gs_index_buffer(gs_device_t *device, enum gs_index_type type,
				 void *indices, size_t num, uint32_t flags)
	: gs_obj(device, gs_type::gs_index_buffer),
	  dynamic((flags & GS_DYNAMIC) != 0),
	  type(type),
	  num(num),
	  indices(indices)
{
	switch (type) {
	case GS_UNSIGNED_SHORT:
		indexSize = 2;
		break;
	case GS_UNSIGNED_LONG:
		indexSize = 4;
		break;
	}

	InitBuffer();
}
