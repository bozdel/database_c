#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "http.h"
#include "socket_api.h"
#include "controller.h"
#include "repository.h"



struct dto get_theatre(struct dto dto) {
	struct dto ret_dto;
	ret_dto.data = "hello, world\n";
	ret_dto.size = strlen(ret_dto.data);
	return ret_dto;
}

char *string = NULL;

struct dto post_home(struct dto dto) {
	struct dto ret_dto;

	string = (char*)malloc(sizeof(char) * 50);
	strcpy(string, (char*)dto.data);

	ret_dto.data = "got\n";
	ret_dto.size = strlen(ret_dto.data);
	return ret_dto;
}

int main() {

	struct controller controller = init_controller();
	printf("controller inited\n");


	// ------theatre------
	struct repository theatre_repo = init_repository("/theatre/");
	printf("repository inited\n");

	register_method(&theatre_repo, GET, get_theatre);
	printf("method registered\n");


	// ------home------
	struct repository home_repo = init_repository("/home/");
	printf("repository inited\n");

	register_method(&home_repo, POST, post_home);
	printf("method registered\n");



	register_repository(&controller, theatre_repo);
	printf("repository registered\n");
	register_repository(&controller, home_repo);
	printf("repository registered\n");

	for (int i = 0; i < 3; i++) {
		process_controller(controller);
	}

	if (string != NULL) {
		printf("%s\n", string);
	}


	return 0;
}