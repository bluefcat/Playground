#include "LZ77.h"
#include <stdio.h>
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

char* decoding_LZ77(const LLD* object, int wsize, int lsize){
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