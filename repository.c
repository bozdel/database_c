#include <stdlib.h>
#include <string.h>
#include "repository.h"

struct dto dummy_method(struct dto dto) {
	struct dto ret;
	ret.data = "hello, this functionality not implemented yet\n";
	ret.size = strlen(ret.data);
	return ret;
}

struct repository init_repository(char *uri) {
	struct repository repo;
	/*repo.post_f = NULL;
	repo.put_f = NULL;
	repo.get_f = NULL;
	repo.delete_f = NULL;*/
	repo.post_f = dummy_method;
	repo.put_f = dummy_method;
	repo.get_f = dummy_method;
	repo.delete_f = dummy_method;

	int uri_leng = strlen(uri);

	repo.uri = (char *)malloc(sizeof(char) * uri_leng);
	strcpy(repo.uri, uri);

	return repo;
}

void free_repository(struct repository repo) {
	free(repo.uri);
}

void register_method(struct repository *repo, enum method meth, method_func_t func) {
	switch (meth) {
		case POST:
			repo->post_f = func;
			break;
		case GET:
			repo->get_f = func;
			break;
		case PUT:
			repo->put_f = func;
			break;
		case DELETE:
			repo->delete_f = func;
			break;
	}
}

