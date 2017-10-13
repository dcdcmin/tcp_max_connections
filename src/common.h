#ifndef CONNECTION_COMMON_H
#define CONNECTION_COMMON_H

#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <signal.h>
#ifdef WIN32
#include "windows.h"
#include <direct.h>
#include <signal.h>
#define socket_t SOCKET
#define snprintf _snprintf
#define close(sock) closesocket(sock)
#else
#include <unistd.h>
#include <netdb.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

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
Options parse_options(int argc, char *argv[]);

void msleep(int ms);
void socketInit();
void socketDeinit();

#endif/*CONNECTION_COMMON_H*/
