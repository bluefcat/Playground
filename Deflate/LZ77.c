#include "LZ77.h"
#include "../Util/CatString.h"
#include <stdio.h>

Header_LZ77* create_header(unsigned short wsize, unsigned short lsize, unsigned int fsize){
	Header_LZ77* header = (Header_LZ77*)malloc(sizeof(Header_LZ77));
	header->mcode = CODE;
	header->wsize = wsize;
	header->lsize = lsize;
	header->fsize = fsize;

	return header;
};

int count_LLD(const LLD* object){
	int result = 0;
	while((object++)->literal){
		result ++;
	}

	return result;
}

int encoding_LZ77f(const char* src_name, int wsize, int lsize){
	FILE* src = fopen(src_name, "rb");
	if(src == NULL) return ERROR_FILE_NOT_EXIST;

	const char* dest_name = change_ext(src_name, "LZ77");
	FILE* dest = fopen(dest_name, "wb");
	if(dest == NULL){
		fclose(src);
		return ERROR_FILE_CANT_CREATE;
	}

	//write header about LZ77 to dest file
	fseek(src, 0, SEEK_END);
	Header_LZ77* header = create_header(wsize, lsize, ftell(src));
	fwrite(header, sizeof(Header_LZ77), 1, dest);
	free(header);

	int total_size = ftell(src); fseek(src, 0, SEEK_SET);
	LLD* temp = (LLD*)malloc(sizeof(LLD));

	BYTE* wbuf = (BYTE*)malloc(sizeof(BYTE)*(wsize+lsize+1));
	BYTE* lbuf = (BYTE*)malloc(sizeof(BYTE)*(lsize+1));
	int wp = 0, lp = 0;

	while(lp <= total_size){
		fseek(src, wp, SEEK_SET);
		fread(wbuf, sizeof(BYTE), lp-wp+lsize, src);

		BYTE distance = 0, length = 0, literal = 0;

		for(int i = 1; i <= lsize; i ++){
			fseek(src, lp, SEEK_SET);
			fread(lbuf, sizeof(BYTE), i, src);
			int p = find_pattern(wbuf, lbuf);

			if(p == -1 || p >= lp-wp) break;

			distance = lp-wp-p; length = i;
			if(lp+i == total_size){
				length --;
				break;
			}
		}
		if(lp+length >= total_size) break;
		fseek(src, lp+length, SEEK_SET);
		fread(&literal, sizeof(BYTE), 1, src);
		
		temp->distance = distance;
		temp->length = length;
		temp->literal = literal;

		fwrite(temp, sizeof(LLD), 1, dest);

		lp += length+1;
		wp += lp-wp > wsize ? lp-wp-wsize : 0;

	}

	free(temp);
	free(lbuf);
	free(wbuf);

	fclose(dest);
	fclose(src);
	return 0;
}

int decoding_LZ77f(const char* src_name, const char* dest_name){
	
	return 0;
}

LLD* encoding_LZ77(const char* str, int wsize, int lsize){
	int total_size = count_string(str);
	LLD* result = (LLD*)malloc(sizeof(LLD)*total_size);

	BYTE* wbuf = (BYTE*)malloc(sizeof(BYTE)*(wsize+lsize+1));
	BYTE* lbuf = (BYTE*)malloc(sizeof(BYTE)*(lsize+1));
	int wp = 0, lp = 0, count = 0;
	
	while(lp <= total_size){
		copy_string(wbuf, str+wp, lp-wp+lsize);

		int distance = 0, length = 0, literal = 0;

		for(int i = 1; i <= lsize; i ++){
			copy_string(lbuf, str+lp, i);
			int p = find_pattern(wbuf, lbuf);
			
			if(p == -1 || p >= lp-wp) break;

			distance = lp-wp-p; length = i;
			if(lp+i == total_size){
				length --;
				break;
			}
		}
		if(lp+length >= total_size) break;

		literal = str[lp+length];
		result[count].distance = distance;
		result[count].length = length;
		result[count].literal = literal;
		count ++;
		lp += length+1;
		wp += lp-wp > wsize ? lp-wp-wsize : 0;
	}

	free(lbuf);
	free(wbuf);
	return (LLD*)realloc(result, sizeof(LLD)*(count+1));
}

char* decoding_LZ77(LLD* object, int wsize, int lsize){
	//initialize result
	char* result = (char*)malloc(sizeof(char)*100+1);
	for(int i = 0; i <= 100; i ++) result[i] = 0;

	LLD* cursor = object;
	
	int p = 0;
	while(cursor->literal){
		//structure content
		int distance = cursor->distance;
		int length = cursor->length+1;
		char literal = (char)cursor->literal;

		for(int i = 0; i < length; i ++){
			if(i == length-1) result[p+i] = literal;
			else {
				result[p+i] = result[p-distance+i];
			}
			
		}
		p += length;

		cursor ++;
	}

	return result;
}
