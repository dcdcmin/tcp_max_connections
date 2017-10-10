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

void error(const char *msg) {
    perror(msg); exit(0); 
}

void warnning(const char *msg) { 
    perror(msg);
}

int main(int argc,char *argv[])
{
    if(argc < 4) {
        printf("Usage: %s host port clients localip\n", argv[0]);
        exit(0);
    }

    int sockfd = 0;
    char* host = argv[1];
    int port = atoi(argv[2]);
    int clients = atoi(argv[3]);
    char* localip = argv[4];
    struct hostent *server = NULL;
    struct sockaddr_in serv_addr;
    char response[4096];
    const char* message = "hello";

    printf("%s remote=%s port=%d clients=%d localip=%s\n", argv[0], host, port, clients, localip);
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
        }else{
            warnning("ERROR connecting");
        }
    }

    for(i = 0; i < clients; i++) {
        sockfd = socks[i];
        if(sockfd) {
            int ret = send(sockfd, message, strlen(message), 0);
            ret = recv(sockfd, response, sizeof(response), 0);
        }
    }

    for(i = 0; i < 10; i++) {
        printf(".");
        fflush(stdout);
        sleep(1);
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
