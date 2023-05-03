
#pragma once

static inline uint64_t util_mul_div64(uint64_t num, uint64_t mul, uint64_t div)
{
	const uint64_t rem = num % div;

	return (num / div) * mul + (rem * mul) / div;
}
