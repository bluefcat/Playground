#include <stdio.h>
#include <stdlib.h>

container* create_container(int capacity, TYPE type){
	container* result = (container*)malloc(sizeof(container));
	result->capacity = capacity;
	result->size = 0;
	result->type = type;
	TYPEMALLOC(type, result->data, capacity);

	result->begin = begin_container;
	result->next = next_container;
	result->end = end_container;

	return result;
}

void delete_container(container* target){
	free(target->data);
	free(target);
	return ;
}

void begin_container(iter* it, container* target){

	it->idx = 0;
	it->type = target->type;
	it->data = target->data+0;

	return ;
}

void next_container(iter* it, container* target){
	it->idx ++;

	switch(it->type){
		case INT:
		it->data = (void*)(((int*)target->data)+(it->idx));
		break;
		case CHAR:
		it->data = (void*)(((char*)target->data)+(it->idx));
		break;
	}
	
	return ;
}

int end_container(iter* it, container* target){
	return it->idx != target->capacity;
}