#include "common.h"

static int requestsNr = 0;
static int send_data(int index, int sock) {
	char response[10240];
	const char* message = "GET / HTTP/1.0\r\n\r\n";
	int wret = send(sock, message, strlen(message), 0);
	int rret = recv(sock, response, sizeof(response), 0);

	requestsNr++;
	printf("(%d)%d send:%d recv:%d\n", requestsNr, index, wret, rret);

	return 0;
}

int main(int argc,char *argv[])
{
	Options opts = parse_options(argc, argv);
	
	opts.on_connected = send_data;
	opts.on_send_payload = send_data;

	run(&opts);

    return 0;

}
