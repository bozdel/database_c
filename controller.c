#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "controller.h"
#include "repository.h"
#include "http.h"
#include "socket_api.h"

void print_controller(struct controller *ctrl) {
	printf("size: %d\n", ctrl->size);
	printf("loaded: %d\n", ctrl->loaded);
}

struct controller init_controller() {
	int init_size = 20;
	struct controller ctrl;
	ctrl.repos = (struct repository*)malloc(sizeof(struct repository) * init_size);
	ctrl.size = init_size;
	ctrl.loaded = 0;
	if (!raise_connection(&ctrl.lsocket)) {
		printf("error. connection not raised\n");
	}
	else {
		printf("connection raised\n");
	}
	// ctrl.lsocket = lsocket;
	return ctrl;
}

void free_controller(struct controller ctrl) {
	for (int i = 0; i < ctrl.size; i++) {
		free_repository(ctrl.repos[i]);
	}
	free(ctrl.repos);
	// close connection
}

// need writing
/*void register_repository(struct controller *ctrl, struct repository repo) {
	int incr_size = 5;
	if (size <= loaded) {
		realloc(ctrl->repos, incr_size); // fix
		ctrl->size += incr_size;
	}
	ctrl->repos[loaded] = repo;
	ctrl->loaded++;
}*/

void register_repository(struct controller *ctrl, struct repository repo) {
	ctrl->repos[ctrl->loaded] = repo;
	ctrl->loaded++;
	if (ctrl->loaded > 20) {
		printf("warning. overflow of repositorys\n");
	}
}

struct repository unregister_repository(struct controller *ctrl, struct repository repo) {
	// find repo
	// decrement ctrl.loaded
}

struct repository find_repo(struct controller ctrl, char *uri) {
	// print_controller(&ctrl);
	for (int i = 0; i < ctrl.loaded; i++) {
		struct repository curr_repo = ctrl.repos[i];
		if (strcmp(curr_repo.uri, uri) == 0) {
			return curr_repo;
		}
	}
	// fix this behavior
	printf("repository not found\n");
	return ctrl.repos[0]; // error
}

#define REQ_MAX_SIZE 1000

int process_controller(struct controller ctrl) {

	int sock = accept(ctrl.lsocket, NULL, NULL); // we don't interested in client addr

	char *req = (char*)malloc(sizeof(char) * REQ_MAX_SIZE);
	if (!req) {
		printf("error in process_controller\n");
		perror("malloc");
		return -1;
	}
	int req_leng = read(sock, req, REQ_MAX_SIZE);

	/*for (int i = 0; i < req_leng; i++) {
		printf("%c", req[i]);
	}
	printf("\n");*/

	struct http_request http_req = init_http_req();
	parse_http_request(req, req_leng, &http_req);

	// print_http_req(http_req);

	// return 0;


	struct repository repo = find_repo(ctrl, http_req.uri);

	struct dto request_dto;
	request_dto.data = (void *)http_req.body;
	request_dto.size = http_req.body_size;

	struct dto response_dto;

	switch (http_req.mthd) {
		case POST:
			response_dto = repo.post_f(request_dto);
			break;
		case GET:
			response_dto = repo.get_f(request_dto);
			break;
		case DELETE:
			response_dto = repo.delete_f(request_dto);
			break;
		case PUT:
			response_dto = repo.put_f(request_dto);
			break;
	}

	// 100 - additional size for containing http response miscelaneous data e.g. start string, parameters and others
	char *resp_string = (char*)malloc(sizeof(char) * (response_dto.size + 100));

	int resp_leng = mk_http_response((char*)response_dto.data, response_dto.size, resp_string);

	write(sock, resp_string, resp_leng);
}