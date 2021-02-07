#pragma once
#include "../Util/CatString.h"

#pragma pack(push, 1)
typedef struct _LLD{
	unsigned int distance;
	unsigned int length;
	unsigned int literal;
} LLD;
#pragma pack(pop)

LLD* encoding_LZ77(const char* str, int wsize, int lsize);
