#pragma once
#define BYTE unsigned char
#define UINT unsigned int

typedef struct _Block{
	BYTE data[64];
	UINT length;
	unsigned long long bitlen;
} Block;

UINT* make_W(const BYTE* input);
UINT* round_sha(UINT* H, UINT K, UINT W); 
void SHA256(BYTE hash[], const BYTE* input);
