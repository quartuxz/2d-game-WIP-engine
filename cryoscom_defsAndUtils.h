#pragma once
#define MAKING_LEVELS false
#if MAKING_LEVELS
	#define CRYOSCOM_DEBUG true
#else
	#define CRYOSCOM_DEBUG false
#endif
#define ADD_PLAYER_VELOCITY_TO_BULLET false
#define MULTITHREADED_SCRIPTING_AND_MESSAGING true



class MockMutex {
public:
	void lock(){}
	void unlock(){}
};

inline int fast_atou(const char* str)
{
	int val = 0;
	while (*str) {
		val = val * 10 + (*str++ - '0');
	}
	return val;
}