all:
	gcc srvr.c repository.c controller.c http.c socket_api.c -o srvr_test

serv: repository.o controller.o srvr.o
	gcc repository.o controller.o srvr.o -o server

repository.o: repository.c
	gcc -c repository.c

controller.o: controller.c
	gcc -c controller.c

srvr.o: srvr.c
	gcc -c srvr.c

clean:
	rm *.o