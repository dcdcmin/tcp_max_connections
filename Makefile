all:
	gcc -g connection_common.c http_connection.c -o http_connection
	gcc -g http_request.c -o http_request
clean:
	rm -rf http_request http_connection
