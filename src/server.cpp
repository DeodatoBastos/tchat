#include "server.h"
#include "utils.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

Server::Server(int domain, int type, int protocol, int port, int address, int num_requests)
    : n_requests(num_requests), active(false) {
    server_socket = socket(domain, type, protocol);
    if (server_socket == -1) {
        perror("Coundn't create server socket.");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = domain;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = address;
    int err = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    if (err != 0) {
        perror("Cound not bind in server.");
        exit(EXIT_FAILURE);
    }
}

void Server::listen_client() {
    int err = listen(server_socket, n_requests);
    if (err != 0) {
        perror("Cound not listen for client requests");
        exit(EXIT_FAILURE);
    }
    std::cout << "starting...\n\n";
    active = true;
    handle_message();
}

void Server::handle_message() {
    int client_socket = accept(server_socket, nullptr, nullptr);

    while (active) {
        char buffer[1024] = {0};
        int err = recv(client_socket, buffer, sizeof(buffer), 0);
        if (err == -1) {
            perror("Error while receiving the message.");
            exit(EXIT_FAILURE);
        }
        int src_id, dest_id;
        std::string msg = decode_message(std::string(buffer), src_id, dest_id);
        // std::cout << "from: " << src_id << "; to: " << dest_id << "; msg: " << msg << "\n";
        err = send(client_socket, buffer, strlen(buffer), 0);
        if (err == -1) {
            perror("Error while sending the message.");
            exit(EXIT_FAILURE);
        }
    }
}

void Server::close_connection() {
    int err = close(server_socket);
    if (err != 0) {
        perror("Cound not close connection.");
        exit(EXIT_FAILURE);
    }
    active = false;
    std::cout << "shuting down...\n\n";
}

Server::~Server() {
    if (active)
        close_connection();
}
