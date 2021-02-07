#pragma once
#include <stdlib.h>

unsigned int count_string(const char* str);
void copy_string(char* dest, const char* src, int size);

int* compute_prefix(const char* pat);
int find_pattern(const char* str, const char* pat);
