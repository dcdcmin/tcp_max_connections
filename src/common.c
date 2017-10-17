#include "common.h"

static int running = 1;
static void on_ctrl_c(int signal){
    running = 0;
    printf("(%d):quiting...\n", signal);
}

static void error(const char *msg) {
    perror(msg); exit(0); 
}

static void warnning(const char *msg) { 
    perror(msg);
}

#define START_PORT 1204
int run(Options* options) {
    int sockfd = 0;
    struct hostent *server = NULL;
    struct sockaddr_in serv_addr;
	
	int port = options->port;
	const char* host = options->host;
	const char* localip = options->localip;
	int duration = options->duration;
	int clients = options->clients;

    socketInit();
    signal(SIGINT, on_ctrl_c);

    printf("remote=%s port=%d clients=%d localip=%s\n", host, port, clients, localip);
    server = gethostbyname(host);
    if (server == NULL) error("ERROR, no such host");

    int i = 0;
    int* socks = (int*)calloc(clients, sizeof(int));

    for(i = 0; running && (i < clients); i++) {
        struct sockaddr_in localaddr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0) error("ERROR opening socket");
        if(localip && *localip) {
            localaddr.sin_family = AF_INET;
            localaddr.sin_addr.s_addr = inet_addr(localip);
            localaddr.sin_port = htons(START_PORT+i);

            if(bind(sockfd, (struct sockaddr *)&localaddr, sizeof(localaddr)) < 0) {
                printf("port conflict %d\n", (int)(START_PORT+i));
                continue;
            }
        }

        memset(&serv_addr,0,sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) >= 0) {
            socks[i] = sockfd;
       		if(options->on_connected) {
       			options->on_connected(i, sockfd);
       		}
        }else{
            warnning("ERROR connecting");
        }
    }

	while(duration && running) {
		duration--;
		if(options->on_send_payload == NULL) {
			printf(".");
			fflush(stdout);
			msleep(1000);
		}
		else{
			for(i = 0;  running && (i < clients); i++) {
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
            printf("%d: close(%d)\n", i, sockfd);
        }
    }

    free(socks);

    socketDeinit();

    return 0;
}

Options parse_options(int argc, char *argv[]) {
	Options opts;

    if(argc < 4) {
        printf("Usage: %s host port clients [localip]\n", argv[0]);
        exit(0);
    }

	memset(&opts, 0x00, sizeof(opts));

    opts.host = argv[1];
    opts.port = atoi(argv[2]);
    opts.clients = atoi(argv[3]);
    opts.localip = argv[4];
    opts.duration = 600;

	return opts;
}

#ifdef WIN32
void socketInit() {
    int iResult;
    WSADATA wsaData;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
    }
}

void socketDeinit() {
    WSACleanup();
}

void msleep(int ms) {
    Sleep(ms);
}

#else
void socketInit() {
}

void socketDeinit() {
}

void msleep(int ms) {
    usleep(ms*1000);
}
#endif
