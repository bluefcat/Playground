#include "CatString.h"

unsigned int count_string(const char* str){
	unsigned int result = 0;
	while(*(str++)){
		result ++;
	}
	return result ;
}

int* compute_prefix(const char* pat){
	unsigned int m = count_string(pat);

	int* pi = (int*)malloc(sizeof(int)*(m));
	for(int i = 0; i < m; i ++) pi[i] = 0;

	int k = 0;
	for(int q = 1; q < m; q ++){
		while(k > 0 && pat[k] != pat[q]) k = pi[k];
		if(pat[k] == pat[q]) k ++;

		pi[q] = k;	
	}
	
	return pi;
}

int find_pattern(const char* str, const char* pat){
	int result = -1;

	int n = count_string(str);
	int m = count_string(pat);
	int* pi = compute_prefix(pat);

	int q = 0;
	for(int i = 0; i < n; i ++){
		while(q > 0 && pat[q] != str[i]) q = pi[q];
		if(pat[q] == str[i]) q ++;

		if(q == m){
			result = i-m+1;
			q = pi[q];
		}
	}
	free(pi);

	return result;
}
