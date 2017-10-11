#ifndef CONNECTION_COMMON_H
#define CONNECTION_COMMON_H

#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <netdb.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef int (*OnConnectedFunc)(int index, int sock);
typedef int (*OnSendPayloadFunc)(int index, int sock);

typedef struct _Options {
	const char* localip;
	const char* host;
	int port;
	int clients;
	int duration;
	OnConnectedFunc on_connected;
	OnSendPayloadFunc on_send_payload;
}Options;

int run(Options* options);

#endif/*CONNECTION_COMMON_H*/
