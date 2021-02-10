#pragma once
#include <stdlib.h>

char* change_ext(const char* src, const char* ext);
unsigned int count_string(const char* str);
void shift_string(char* str, int pos);
void copy_string(char* dest, const char* src, int size);

int* compute_prefix(const char* pat);
int find_pattern(const char* str, const char* pat);
