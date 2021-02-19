#define TYPEMALLOC(TYPE, TARGET, N) switch(TYPE){	\
	case INT:										\
	(TARGET) = malloc(sizeof(int*)*(N));			\
	break;											\
	case CHAR:										\
	(TARGET) = malloc(sizeof(char*)*(N));			\
	break;											\
}													\

#define FOR(ITER, CONTAIN) for(	(CONTAIN)->begin((ITER), (CONTAIN));			\
								(CONTAIN)->end((ITER), (CONTAIN));				\
								(CONTAIN)->next((ITER), (CONTAIN)))				\

typedef enum TYPE{
	INT = 0x00,
	CHAR = 0x10,
}TYPE;

typedef struct _iter{
	int idx;
	void* data;
	TYPE type;
}iter;

typedef struct _container{
	int capacity;
	int size;
	TYPE type;
	void* data;
	void (*begin)(iter*, struct _container*);
	void (*next)(iter*, struct _container*);
	int (*end)(iter*, struct _container*);
} container;

container* create_container(int, TYPE);
void delete_container(container*);
void begin_container(iter*, container*);
void next_container(iter*, container*);
int end_container(iter*, container*);