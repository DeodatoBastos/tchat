#include "server.h"
#include "utils.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

int message_id = 0;
pthread_mutex_t message_mutex;

Server::Server(int domain, int type, int protocol, int port, int address, int num_requests,
               bool is_single_thread)
    : n_requests(num_requests), active(false), is_single_thread(is_single_thread) {
    // start socket
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
    accept_connection();
}

void Server::accept_connection() {
    int client_socket;
    pthread_t thread;
    std::cout << "listening and accepting connections" << std::endl;
    pthread_mutex_init(&message_mutex, NULL);
    int i = 0;
    while (i < 20000) {
        i++;
        if ((client_socket = accept(server_socket, nullptr, nullptr)) < 0) {
            perror("Error accepting connection");
            exit(EXIT_FAILURE);
        }
        if (pthread_create(&thread, NULL, handle_message, (void *)&client_socket)) {
            perror("Error creating thread!");
            exit(EXIT_FAILURE);
        }
        if (is_single_thread) {
            std::cout << "waiting.." << std::endl;
            pthread_join(thread, NULL); // one thread only
        }
    }
    pthread_join(thread, NULL);
    std::cout << "Got " << i << " messages and final message_id is " << message_id << std::endl;
}

void *Server::handle_message(void *socket_arg) {
    int client_socket = *(int *)socket_arg;
    char buffer[1024] = {0};
    if (recv(client_socket, buffer, sizeof(buffer), 0) == -1) {
        perror("Error while receiving the message.");
        exit(EXIT_FAILURE);
    }
    int src_id, dest_id;
    std::string msg = decode_message(std::string(buffer), src_id, dest_id);
    sleep_max_ms(1000);
    // sync message_id
    pthread_mutex_lock(&message_mutex);
    message_id++;
    pthread_mutex_unlock(&message_mutex);
    if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
        perror("Error while sending the message.");
        exit(EXIT_FAILURE);
    }
    pthread_exit(NULL);
}

void Server::close_connection() {
    if (close(server_socket) != 0) {
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
