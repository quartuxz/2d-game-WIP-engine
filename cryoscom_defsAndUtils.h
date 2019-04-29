#pragma once
#define CRYOSCOM_DEBUG false

inline int fast_atou(const char* str)
{
	int val = 0;
	while (*str) {
		val = val * 10 + (*str++ - '0');
	}
	return val;
}