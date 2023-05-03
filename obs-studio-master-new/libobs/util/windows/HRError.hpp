#pragma once

struct HRError {
	const char *str;
	HRESULT hr;

	inline HRError(const char *str, HRESULT hr) : str(str), hr(hr) {}
};
