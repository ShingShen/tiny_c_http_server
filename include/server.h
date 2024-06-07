#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>

extern int create_sock(struct sockaddr_in, unsigned int);
extern void run_server(int, void * (*)(void *));

#endif