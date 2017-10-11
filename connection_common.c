#include "connection_common.h"

void error(const char *msg) {
    perror(msg); exit(0); 
}

void warnning(const char *msg) { 
    perror(msg);
}

int run(Options* options) {
    int sockfd = 0;
    struct hostent *server = NULL;
    struct sockaddr_in serv_addr;
	
	int port = options->port;
	const char* host = options->host;
	const char* localip = options->localip;
	int duration = options->duration;
	int clients = options->clients;

    printf("remote=%s port=%d clients=%d localip=%s\n", host, port, clients, localip);
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");

    int i = 0;
    int* socks = (int*)calloc(clients, sizeof(int));

    for(i = 0; i < clients; i++) {
        struct sockaddr_in localaddr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0) error("ERROR opening socket");
        localaddr.sin_family = AF_INET;
        localaddr.sin_addr.s_addr = inet_addr(localip);
        localaddr.sin_port = 0;  // Any local port will do
        bind(sockfd, (struct sockaddr *)&localaddr, sizeof(localaddr));

        memset(&serv_addr,0,sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) >= 0) {
            socks[i] = sockfd;
            printf("%d: %d\n", i, sockfd);
        }else{
            warnning("ERROR connecting");
        }
    }

    for(i = 0; i < clients; i++) {
        sockfd = socks[i];
        if(sockfd) {
       		if(options->on_connected) {
       			options->on_connected(i, sockfd);
       		}
        }
    }

	while(duration) {
		duration--;
		if(options->on_send_payload == NULL) {
			printf(".");
			fflush(stdout);
			sleep(1);
		}
		else{
			for(i = 0; i < clients; i++) {
				sockfd = socks[i];
				if(sockfd) {
					if(options->on_send_payload) {
						options->on_send_payload(i, sockfd);
					}
				}
			}
		}
    }

    for(i = 0; i < clients; i++) {
        sockfd = socks[i];
        if(sockfd) {
            close(sockfd);
        }
    }

    free(socks);

    return 0;

}
