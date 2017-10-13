#include "common.h"

static int requestsNr = 0;
const unsigned char message[] = {  
  0x10, 0x0c, 0x00, 0x04, 
  0x4d, 0x51, 0x54, 0x54, 
  0x04, 0x02, 0xef, 0xef, 
  0x00, 0x00
}; 

static int send_data(int index, int sock) {
	char response[10240];
	int len = sizeof(message);
	int wret = send(sock, (const void*)message, len, 0);
	int rret = recv(sock, response, sizeof(response), 0);

	requestsNr++;
	printf("(%d)%d send(%d):%d recv:%d\n", requestsNr, index, len, wret, rret);

	return 0;
}

int main(int argc,char *argv[])
{
	Options opts = parse_options(argc, argv);
	opts.on_connected = send_data;

	run(&opts);

    return 0;
}
