#include "connection_common.h"

static int requestsNr = 0;
static int send_data(int index, int sock) {
	char response[10240];
	const char* message = "GET / HTTP/1.0\r\n";
	int wret = send(sock, message, strlen(message), 0);
	int rret = recv(sock, response, sizeof(response), 0);

	requestsNr++;
	printf("(%d)%d send:%d recv:%d\n", requestsNr, index, wret, rret);

	return 0;
}

Options parse_options(int argc,char *argv[]) {
	Options opts;

    if(argc < 4) {
        printf("Usage: %s host port clients localip\n", argv[0]);
        exit(0);
    }

	memset(&opts, 0x00, sizeof(opts));

    opts.host = argv[1];
    opts.port = atoi(argv[2]);
    opts.clients = atoi(argv[3]);
    opts.localip = argv[4];
    opts.duration = 60;
	opts.on_connected = send_data;
	opts.on_send_payload = send_data;
	return opts;
}

int main(int argc,char *argv[])
{
	Options opts = parse_options(argc, argv);

	run(&opts);

    return 0;

}
