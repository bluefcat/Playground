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
	Header_LZ77* header = create_header(ftell(src));
	fwrite(header, sizeof(Header_LZ77), 1, dest);
	free(header);

	fseek(src, 0, SEEK_SET);

	int psize = (wsize+lsize) << 1;
	int wp = 0;
	int lp = 0;


	BYTE* wbuf = (BYTE*)malloc(sizeof(BYTE)*wsize+1);
	BYTE* lbuf = (BYTE*)malloc(sizeof(BYTE)*lsize+1);

	BYTE* fbuf = (BYTE*)malloc(sizeof(BYTE)*psize+1);
	LLD* obuf = (LLD*)malloc(sizeof(LLD)*psize);

	while(!feof(src)){
		//initialize fbuf and read file
		for(int i = 0; i < psize; i ++) fbuf[i] = 0;
		fread(fbuf+lp, sizeof(BYTE), psize-lp, src);
		int count = 0;

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

int decoding_LZ77f(const char* src_name){
	
	return 0;
}

LLD* encoding_LZ77(const char* str, int wsize, int lsize){
	int n = count_string(str);
	LLD* result = (LLD*)malloc(sizeof(LLD)*n);

	int wp = 0;

	char* wbuf = (char*)malloc(sizeof(char)*(wsize+lsize/2)+1);
	char* lbuf = (char*)malloc(sizeof(char)*lsize+1);

	int lp = 0;
	int count = 0;	
	while(lp < n){
		while(lp > wsize+wp) wp ++;
		
		//initialize wbuf and lbuf
		for(int i = 0; i < (wsize+lsize/2)+1; i ++) wbuf[i] = 0;
		for(int i = 0; i < lsize+1; i ++) lbuf[i] = 0;

		int wseach_size = lp-wp;
		copy_string(wbuf, str+wp, wseach_size);
		
		int distance = 0;
		int length = 0;

		for(int i = 1; i <= lsize; i ++){
			//initialize lbuf
			for(int j = 0; j < lsize+1; j ++) lbuf[j] = 0;
			copy_string(lbuf, str+lp, i);

			int r = find_pattern(wbuf, lbuf);

			if(r == -1) break;

			if(i+r >= count_string(wbuf)){
				for(int j = 0; j < (wsize+lsize/2)+1; j ++)wbuf[j] = 0;
				wseach_size ++;
				copy_string(wbuf, str+wp, wseach_size);
			}

			distance = (lp-wp)-r;
			length = i;	

			//about endword
			if(lp+i == n){
				length --;
				break;
			}
		}
		wseach_size = lp-wp;

		result[count].distance = distance;
		result[count].length = length;
		result[count].literal = str[lp+length];

		lp += length+1;
		count ++;
	}

	result = (LLD*)realloc(result, sizeof(LLD)*(count+1));
	result[count].literal = 0;

	free(lbuf);
	free(wbuf);

	return result;
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
