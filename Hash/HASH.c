#include <stdlib.h>
#include <string.h>
#include "HASH.h"
#include "../Util/CatString.h"

#define ROTATE_R(X, N) ((((X) << (32-(N))) | ((X) >> (N))))
#define CH(X, Y, Z) (((X) & (Y)) ^ (~(X) & (Z)))
#define MA(X, Y, Z) (((X) & (Y)) ^ ((X) & (Z)) ^ ((Y) & (Z)))
#define EP0(X) (ROTATE_R(X, 2) ^ ROTATE_R(X, 13) ^ ROTATE_R(X, 22))
#define EP1(X) (ROTATE_R(X, 6) ^ ROTATE_R(X, 11) ^ ROTATE_R(X, 25))
#define SIG0(X) (ROTATE_R(X, 7) ^ ROTATE_R(X, 18) ^ ((X) >> 3))
#define SIG1(X) (ROTATE_R(X, 17) ^ ROTATE_R(X, 19) ^ ((X) >> 10))

UINT* make_W(const BYTE* input){
	UINT* W = (UINT*)malloc(sizeof(UINT)*64);
	for(int i = 0, j = 0; i < 16; i ++, j += 4){
		W[i] = (input[j] << 24) | (input[j+1] << 16) | (input[j+2] << 8) | (input[j+3]);
	}
	for(int i = 16; i < 64; i ++){
		W[i] = SIG1(W[i-2])+W[i-7]+SIG0(W[i-15])+W[i-16];
	}

	return W;
}

UINT* round_sha(UINT* H, UINT K, UINT W){
	UINT* nH = (UINT*)malloc(sizeof(UINT)*8);

	UINT maj = MA(H[0], H[1], H[2]);
	UINT ch = CH(H[4], H[5], H[6]);
	UINT s = K+EP1(H[4])+ch+H[7]+W;

	nH[0] = EP0(H[0])+maj+s;
	nH[1] = H[0]; nH[2] = H[1]; nH[3] = H[2]; 
	nH[4] = H[3]+s;
	nH[5] = H[4]; nH[6] = H[5]; nH[7] = H[6];

	return nH;
}

void SHA256(BYTE hash[], const BYTE* input){
	UINT const_H[8] = { 0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19 };

	UINT K[64] = {
		0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
	};
	
	UINT* H = (UINT*)malloc(sizeof(UINT)*8);
	for(int i = 0; i < 8; i ++) H[i] = const_H[i];
	
	UINT nH[8] = { 0, };
	Block block;

	UINT count = count_string(input);
	
	for(UINT i = 0; i < count; i ++){
		block.data[block.length] = input[i];
		block.length ++;
		if(block.length == 64){
			UINT* W = make_W(block.data);
			for(int i = 0; i < 8; i ++) nH[i] = H[i];

			for(int i = 0; i < 64; i ++) {
				UINT* tmp = round_sha(nH, K[i], W[i]);
				for(int j = 0; j < 8; j ++) nH[j] = tmp[j];
				free(tmp);
			}

			for(int i = 0; i < 8; i ++) H[i] += nH[i];
			
			block.bitlen += 512;
			block.length = 0;
			free(W);
		}
	}

	if(block.length < 56){
		block.data[block.length ++] = 0x80;
		while(block.length < 56){
			block.data[block.length ++] = 0x00;
		}
	}
	else {
		block.data[block.length ++] = 0x80;
		while(block.length < 64){
			block.data[block.length ++] = 0x00;
		}
		UINT* W = make_W(block.data);
		for(int i = 0; i < 8; i ++) nH[i] = H[i];

		for(int i = 0; i < 64; i ++) {
			UINT* tmp = round_sha(nH, K[i], W[i]);
			for(int j = 0; j < 8; j ++) nH[j] = tmp[j];
			free(tmp);
		}
		free(W);
		for(int i = 0; i < 8; i ++) H[i] += nH[i];
		memset(block.data, 0, 56);
	}

	block.bitlen += block.length * 8;
	block.data[63] = block.bitlen;
	block.data[62] = block.bitlen >> 8;
	block.data[61] = block.bitlen >> 16;
	block.data[60] = block.bitlen >> 24;
	block.data[59] = block.bitlen >> 32;
	block.data[58] = block.bitlen >> 40;
	block.data[57] = block.bitlen >> 48;
	block.data[56] = block.bitlen >> 56;

	UINT* W = make_W(block.data);
	for(int i = 0; i < 8; i ++) nH[i] = H[i];

	for(int i = 0; i < 64; i ++) {
		UINT* tmp = round_sha(nH, K[i], W[i]);
		for(int j = 0; j < 8; j ++) nH[j] = tmp[j];
		free(tmp);
	}

	for(int i = 0; i < 8; i ++) H[i] += nH[i];
	free(W);

	for(int i = 0; i < 4; i ++){
		hash[i]      = (H[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4]  = (H[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8]  = (H[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (H[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (H[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (H[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (H[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (H[7] >> (24 - i * 8)) & 0x000000ff;
	}
	
	free(H);
	return ;	
}
