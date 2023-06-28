#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "repository.h"

struct controller {
	struct repository *repos;
	int size;
	int loaded;
	int lsocket;
};

struct controller init_controller();

void free_controller(struct controller ctrl);

void register_repository(struct controller *ctrl, struct repository repo);

struct repository unregister_repository(struct controller *ctrl, struct repository repo);

int process_controller(struct controller ctrl);

#endif