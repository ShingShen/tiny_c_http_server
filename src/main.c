#include <stdlib.h>
#include <unistd.h>

#include "server.h"
#include "request.h"

int main(int argc, char *argv[]) 
{
    struct sockaddr_in server_addr;
    unsigned int server_port = 8080;
    if (argc > 1) {
        if (sscanf(argv[1], "%u", &server_port) == 0 || server_port > 65535) {
            fprintf(stderr, "error: invalid command line argument, using default port 8080.\n");
            server_port = 8080;
        }
    }

    int server_sock = create_sock(server_addr, server_port);

    run_server(server_sock, request_handler);

    return 0;
}