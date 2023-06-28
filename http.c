#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "http.h"

const char *method_str[] = {
	"POST",
	"GET",
	"PUT",
	"DELETE"
};

struct http_request init_http_req() {
	struct http_request req;
	req.mthd = -1;
	req.uri = NULL;
	return req;
}

void free_http_req(struct http_request req) {
	if (req.uri != NULL) {
		free(req.uri);
	}
}

void print_http_req(struct http_request req) {
	printf("(method): %s\n", method_str[req.mthd]);
	printf("(uri): %s\n", req.uri);
	printf("(body_size): %d\n", req.body_size);
	printf("(body): %s\n", req.body);
}

void parse_start_line(char *start_line, struct http_request *http_req) {
	char *saveptr_sl = NULL;

	char *method = NULL;
	char *uri = NULL;
	char *http_ver = NULL;
	method = strtok_r(start_line, " ", &saveptr_sl);
	uri = strtok_r(NULL, " ", &saveptr_sl);
	http_ver = strtok_r(NULL, " ", &saveptr_sl);
	printf("[method]: %s\n", method);
	printf("[uri]: %s\n", uri);
	printf("[http_ver]: %s\n", http_ver);

	if (strcmp(method, method_str[POST]) == 0) {
		http_req->mthd = POST;
	}
	else if (strcmp(method, method_str[GET]) == 0) {
		http_req->mthd = GET;
	}
	else if (strcmp(method, method_str[PUT]) == 0) {
		http_req->mthd = PUT;
	}
	else if (strcmp(method, method_str[DELETE]) == 0) {
		http_req->mthd = DELETE;
	}

	http_req->uri = strdup(uri);
}

void parse_param_line(char *param_line) {
	char *saveptr_pl = NULL;

	char *param = NULL;
	char *arg = NULL;
	param = strtok_r(param_line, " ", &saveptr_pl);
	arg = strtok_r(NULL, " ", &saveptr_pl);
	printf("[param]: %s\n", param);
	printf("[arg]: %s\n", arg);
}

// req maybe should be null-terminated?
// maybe leng argument doesn't required?
void parse_http_request(char *req, int leng, struct http_request *http_req) {
	printf("\n");
	int i = 0;
	char *start_line = NULL;
	char *param_line = NULL;
	start_line = strtok(req, "\n");
	printf("[%d][leng: %ld]: %s\n", i++, strlen(start_line), start_line);

	parse_start_line(start_line, http_req);

	while ( (param_line = strtok(NULL, "\n")) != NULL ) {
		printf("[%d][leng: %ld]: %s\n", i++, strlen(param_line), param_line);

		if (strlen(param_line) == 1) {
			printf("[data here]\n");
			break;
		}

		parse_param_line(param_line);
	}

	char *body = strtok(NULL, "\0");
	http_req->body = body;
	http_req->body_size = (body == NULL ? 0 : strlen(body));
}

int mk_http_response(char *data, int data_len, char *response) {
	sprintf(response, "HTTP/1.1 200 OK\nContent-length: %d\n\n%s", data_len, data);
	return strlen(response);
}

/*int tmp_mk_http_response(char *data, int leng, char *response_string) {
	if (response_string != NULL) {
		printf("warning: response_string isn't NULL. may be memory overwrite\n");
	}
	const char response[100] = "HTTP/1.1 200 OK\n\n";
	int resp_leng = strlen(response);
	response_string = (char*)malloc(sizeof(char) * (leng + resp_leng + 1));
	strcpy(response_string, response);
	char *response_string_data_ptr = response_string + strlen(response_string);
	strncpy(response_string_data_ptr, data, leng);
	response_string_data_ptr[leng] = '\0';
	return leng + resp_leng + 1;
}*/