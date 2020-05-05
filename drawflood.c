#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>
#include <assert.h>

#include "drawflood.h"

#define LISTEN_ADDRESS_DEFAULT "::"
#define LISTEN_PORT_DEFAULT "1234"
#define CONNECTION_QUEUE_SIZE 16

static int one = 1;

int main(int argc, char *argv[]) {
    printf("Starting drawflood\n");

    int opt, err;
    struct sockaddr_storage* inaddr;
    struct addrinfo* addr_list;
    size_t addr_len;

    char* addr = LISTEN_ADDRESS_DEFAULT;
    char* port = LISTEN_PORT_DEFAULT;

    while((opt = getopt(argc, argv, "l:p:")) != -1) {
        switch(opt) {
            case('l'):
                addr = optarg;
                break;
            case('p'):
                port = optarg;
                break;
            default:
                return EINVAL;
        }
    }

    if((err = -getaddrinfo(addr, port, NULL, &addr_list))) {
        fprintf(stderr, "Failed to resolve listen address '%s', %d => %s\n", addr, err, gai_strerror(-err));
        return EINVAL;
    }


    inaddr = (struct sockaddr_storage*)addr_list->ai_addr;
    addr_len = addr_list->ai_addrlen;

    net_listen(inaddr, addr_len);
}

int net_listen(struct sockaddr_storage* addr, size_t addr_len) {
    char host_tmp[NI_MAXHOST], port_tmp[NI_MAXSERV];

    // Create socket
    int sock;
    sock = socket(addr->ss_family, SOCK_STREAM, 0);
    if(sock < 0) {
        fprintf(stderr, "Failed to create socket\n");
        return errno;
    }
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));

    assert(!getnameinfo((struct sockaddr*)addr, addr_len, host_tmp, NI_MAXHOST, port_tmp, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV));


    // Start listening
    if(bind(sock, (struct sockaddr*)addr, addr_len) < 0) {
        fprintf(stderr, "Failed to bind to %s:%s\n", host_tmp, port_tmp);
        return errno;
    }

    if(listen(sock, CONNECTION_QUEUE_SIZE)) {
        fprintf(stderr, "Failed to start listening: %d => %s\n", errno, strerror(errno));
        return errno;
    }

    if (addr->ss_family == AF_INET6) {
        printf("Listening on [%s]:%s\n", host_tmp, port_tmp);
    } else {
        printf("Listening on %s:%s\n", host_tmp, port_tmp);
    }


    // // Start listening
    // if(bind(socket, listen_address, listen_address_length) < 0) {
    //     fprintf(stderr, "Failed to bind to %s:%s\n", host_tmp, port_tmp);
    //     err = -errno;
    //     goto fail_socket;
    // }

     return 0;
}
