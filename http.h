#ifndef HTTP_H
#define HTTP_H

enum method {
	POST = 0,
	GET = 1,
	PUT = 2,
	DELETE = 3
};

extern const char *method_str[];

struct http_request {
	int mthd;
	char *uri; // this is string (null-terminated)
	char *body;
	int body_size;
};

struct http_request init_http_req();

void free_http_req(struct http_request req);

void print_http_req(struct http_request req);

void parse_http_request(char *req, int leng, struct http_request *http_req);

int mk_http_response(char *data, int data_len, char *response);

// int tmp_mk_http_response(char *data, int leng, char *response_string);

#endif