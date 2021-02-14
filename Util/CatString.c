#include "CatString.h"

char* change_ext(const char* src, const char* ext){
	// s_file is length of path
	// s_ext is position of file ext
	int s_file = find_pattern(src, "/") + 1;
	int s_ext = find_pattern(src+s_file, ".");
	int ext_length = count_string(ext);

	int result_length = s_file+count_string(src+s_file)-s_ext+ext_length;
	char* result = (char*)malloc(sizeof(char)*result_length+1);
	for(int i = 0; i <= result_length; i ++) result[i] = 0;

	// copy path to result
	// copy filename to result within "."
	// copy ext to result
	copy_string(result, src, s_file);
	copy_string(result+s_file, src+s_file, s_ext+1);
	copy_string(result+s_file+s_ext+1, ext, ext_length);

	return result;
}

unsigned int count_string(const char* str){
	unsigned int result = 0;
	while(*(str++)){
		result ++;
	}
	return result ;
}

void shift_string(char* str, int pos){
	if(pos == 0) return;

	//position range [0, sizeof(str))
	int length = count_string(str);
	for(int i = 0; i < pos; i ++) str[i] = str[i+pos];
	str[length - pos] = 0;
	
	return ;
}

void copy_string(char* dest, const char* src, int size){
	if(size == 0) return ;

	int i = 0;
	for(i = 0; i < size && *src; i ++) dest[i] = src[i];
	dest[i] = 0;
	return ;
}

int* compute_prefix(const char* pat){
	unsigned int m = count_string(pat);

	int* pi = (int*)malloc(sizeof(int)*(m));
	for(int i = 0; i < m; i ++) pi[i] = 0;

	int k = 0;
	for(int q = 1; q < m; q ++){
		while(k > 0 && pat[k] != pat[q]) k = pi[k-1];
		if(pat[k] == pat[q]) k ++;

		pi[q] = k;	
	}
	
	return pi;
}

int find_pattern(const char* str, const char* pat){
	int result = -1;

	int n = count_string(str);
	int m = count_string(pat);
	if(n == 0 || m == 0) return n;

	int* pi = compute_prefix(pat);

	int q = 0;
	for(int i = 0; i < n; i ++){
		while(q > 0 && pat[q] != str[i]) q = pi[q-1];
		if(pat[q] == str[i]) q ++;

		if(q == m){
			result = i-m+1;
			q = pi[q];
			break;
		}
	}
	free(pi);

	return result;
}
