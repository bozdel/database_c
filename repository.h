#ifndef REPOSITORY_H
#define REPOSITORY_H
#include "http.h"

struct dto {
	void *data;
	int size;
};

typedef struct dto (*method_func_t)(struct dto dto);

struct repository {
	method_func_t post_f; // return null?
	method_func_t put_f; // return null?
	method_func_t get_f; // return response
	method_func_t delete_f; // return null?
	char *uri;
};

struct repository init_repository(char *uri);

void free_repository(struct repository repo);

void register_method(struct repository *repo, enum method meth, method_func_t func);

#endif