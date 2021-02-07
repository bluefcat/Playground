#include "LZ77.h"

LLD* compress_LZ77(const char* str, int wsize, int lsize){
	int n = count_string(str);
	LLD* result = (LLD*)malloc(sizeof(LLD)*n);

	int wp = 0;

	char* wbuf = (char*)malloc(sizeof(char)*wsize+1);
	char* lbuf = (char*)malloc(sizeof(char)*lsize+1);

	int lp = 0;
	int count = 0;	
	while(lp < n){
		while(lp > wsize+wp) wp ++;
		
		//initialize wbuf and lbuf
		for(int i = 0; i < wsize+1; i ++) wbuf[i] = 0;
		for(int i = 0; i < lsize+1; i ++) lbuf[i] = 0;

		copy_string(wbuf, str+wp, lp-wp);
		
		int distance = 0;
		int length = 0;

		for(int i = 1; i < lsize; i ++){
			//initialize lbuf
			for(int j = 0; j < lsize+1; j ++) lbuf[j] = 0;
			copy_string(lbuf, str+lp, i);

			int r = find_pattern(wbuf, lbuf);
			if(r == -1) break;

			distance = (lp-wp)-r;
			length = i;	

			//about endword
			if(lp+i == n){
				length --;
				break;
			}
		}

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
