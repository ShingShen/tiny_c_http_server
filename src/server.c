#include "server.h"

int create_sock(struct sockaddr_in addr, unsigned int port) {
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        fprintf(stderr, "create socket error\n");
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(port);

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        fprintf(stderr, "bind() error\n");
        exit(1);
    }

    if (listen(sock, 3) < 0) {
        fprintf(stderr, "listen() error\n");
        exit(1);
    }

    return sock;
}

void run_server(int server_sock, void *(*start_routine) (void *))
{
    int client_sock;
    struct sockaddr_in client_addr;
    unsigned int client_addr_size;
    while (1) {
        client_addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
        if (client_sock < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Connection Request : %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        
        pthread_t t_id;
        if (pthread_create(&t_id, NULL, start_routine, &client_sock)) {
            perror("Could not create thread");
            continue;
        }

        pthread_detach(t_id);
    }
}

// void run_server(int server_sock, void *(*start_routine) (void *))
// {
//     struct epoll_event event, events[10];
//     int epoll_fd = epoll_create1(0);
//     if (epoll_fd == -1) {
//         perror("epoll_create1");
//         exit(EXIT_FAILURE);
//     }

//     event.events = EPOLLIN;
//     event.data.fd = server_sock;
//     if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_sock, &event) == -1) {
//         perror("epoll_ctl: server_sock");
//         exit(EXIT_FAILURE);
//     }

//     while (1) {
//         int num_events = epoll_wait(epoll_fd, events, 10, -1);
//         if (num_events == -1) {
//             perror("epoll_wait failed");
//             exit(EXIT_FAILURE);
//         }

//         for (int i = 0; i < num_events; ++i) {
//             if (events[i].data.fd == server_sock) {
//                 struct sockaddr_in client_addr;
//                 socklen_t client_addr_size = sizeof(client_addr);
//                 int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
//                 if (client_sock == -1) {
//                     perror("Accept failed");
//                     continue;
//                 }

//                 printf("Connection Request : %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

//                 event.events = EPOLLIN | EPOLLET;
//                 event.data.fd = client_sock;
//                 if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_sock, &event) == -1) {
//                     perror("epoll_ctl: server_sock");
//                     exit(EXIT_FAILURE);
//                 }
//             } else {
//                 pthread_t t_id;
//                 if (pthread_create(&t_id, NULL, start_routine, &events[i].data.fd)) {
//                     perror("Could not create thread");
//                     continue;
//                 }

//                 pthread_detach(t_id);
//             }
//         }
//     }
// }