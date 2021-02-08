#include "LZ77.h"
#include "../Util/CatString.h"
#include <stdio.h>

Header_LZ77* create_header(unsigned int file_size){
	Header_LZ77* header = (Header_LZ77*)malloc(sizeof(Header_LZ77));
	header->magic_code = CODE;
	header->file_size = file_size;

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
	int file_size = ftell(src);
	Header_LZ77* header = create_header(ftell(src));
	fwrite(header, sizeof(Header_LZ77), 1, dest);
	free(header);

	fseek(src, 0, SEEK_SET);

	int psize = (wsize+lsize) << 1;
	int wp = 0;
	int lp = 0;


	BYTE* wbuf = (BYTE*)malloc(sizeof(BYTE)*(wsize+1));
	BYTE* lbuf = (BYTE*)malloc(sizeof(BYTE)*(lsize+1));

	BYTE* fbuf = (BYTE*)malloc(sizeof(BYTE)*(psize+1));
	LLD* obuf = (LLD*)malloc(sizeof(LLD)*psize);


	while(!feof(src)){
		//initialize fbuf and read file
		for(int i = 0; i < psize; i ++) fbuf[i] = 0;
		fread(fbuf+lp, sizeof(BYTE), psize-lp, src);
		int count = 0;
		printf("[%d/%d]\n", file_size, ftell(src));
		//here encoding code
		while(lp < psize){
			//if can't read as much as lsize in fbuf, read again.
			if(lp + lsize > psize && !feof(src)){
				//
				copy_string(fbuf, fbuf+lp, wsize);
				for(int i = wsize; i <= psize; i ++) fbuf[i] = 0;

				wp = 0; lp = wsize;
				break;
			}

			unsigned int distance = 0;
			unsigned int length = 0;
			BYTE literal = 0;

			//lp-wp <= wsize;
			int wsearch_size = lp-wp;
			copy_string(wbuf, fbuf+wp,  wsearch_size);

			//find word in lbuf what long matcher word in wbuf
			for(int i = 1; i <= lsize; i ++){
				//initailize lbuf 
				for(int j = 0; j <= lsize; j ++) lbuf[j] = 0;
				copy_string(lbuf, fbuf+lp, i);

				int r = find_pattern(wbuf, lbuf);
				if(r == -1) break;

				if(i+r >= count_string(wbuf)){
					for(int j = 0; j < (psize >> 2); j ++) wbuf[j] = 0;
					wsearch_size ++;
					copy_string(wbuf, fbuf+wp, wsearch_size);
				}

				distance = (lp-wp)-r;
				length = i;

				//about endword
				if(lp+i == psize){
					length --;
					break;
				}
			}
			if(!fbuf[lp+length]) break;
			
			//printf("(%d %d \"%2.x\")[%d/%d], wbuf : [%d(+%d)/%d], lbuf : [%d/%d]\n", 
			//distance, length, fbuf[lp+length], psize, count, wsize, lsize, wsearch_size, lsize, length);
			
			obuf[count].distance = distance;
			obuf[count].length = length;
			obuf[count].literal = fbuf[lp+length];

			wsearch_size = lp-wp;

			count ++;
			lp += length+1;
			while(lp > wsize+wp) wp ++;
		}

		//write encoding content
		int obuf_length = count_LLD(obuf);
		fwrite(obuf, sizeof(LLD), obuf_length, dest);
		for(int i = 0; i < obuf_length; i ++){
			obuf[i].distance = 0;
			obuf[i].length = 0;
			obuf[i].literal = 0;
		}
	}

	free(obuf);
	free(fbuf);

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

	BYTE* wbuf = (BYTE*)malloc(sizeof(BYTE)*(wsize+lsize+1));
	BYTE* lbuf = (BYTE*)malloc(sizeof(BYTE)*(lsize+1));
	int wp = 0, lp = 0;
	
	while(lp <= total_size){
		copy_string(wbuf, str+wp, lp-wp+lsize);

		int distance = 0, length = 0, literal = 0;

		for(int i = 1; i <= lsize; i ++){
			copy_string(lbuf, str+lp, i);
			int p = find_pattern(wbuf, lbuf);
			
			if(p == -1 || p >= lp-wp) break;

			distance = lp-wp-p;
			length = i;
			if(lp+i == total_size){
				length --;
				break;
			}
		}
		if(lp+length >= total_size) break;

		literal = str[lp+length];
		printf("(%d %d \"%c\")\n", distance, length, literal);

		
		lp += length+1;
		wp += lp-wp > wsize ? lp-wp-wsize : 0;
	}

	free(lbuf);
	free(wbuf);
	return NULL;
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
