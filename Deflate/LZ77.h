#pragma once
#define ERROR_FILE_NOT_EXIST 0x01
#define ERROR_FILE_CANT_CREATE 0x02

#define CODE 0x5678
#define BYTE unsigned char

#pragma pack(push, 1)
typedef struct _Header_LZ77{
	unsigned short magic_code;
	unsigned int file_size;
} Header_LZ77;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct _LLD{
	unsigned int distance;
	unsigned int length;
	BYTE literal;
} LLD;
#pragma pack(pop)

Header_LZ77* create_header(unsigned int file_size);
int count_LLD(const LLD* object);

int encoding_LZ77f(const char* src_name, int wsize, int lsize);
int decoding_LZ77f(const char* src_name);

LLD* encoding_LZ77(const char* str, int wsize, int lsize);
char* decoding_LZ77(LLD* object, int wsize, int lsize);
