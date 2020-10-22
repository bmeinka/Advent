#ifndef CORE_LIST
#define CORE_LIST

typedef struct list_node {
	void *item;
	struct list_node *next;
} *List;

#endif
