#include "users.h"
#include "utils.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

int number_of_recused = 0;
Users::Users(double p_msg, double p_u, int n, int port)
    : p_message(p_msg), p_user(p_u), server_port(port), active(false) {
    users.reserve(n);
    for (int i = 0; i < n; i++) {
        users.push_back("user_" + std::to_string(i));
    }
}

int Users::create_connection(int server_port) {
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(server_port);
    address.sin_addr.s_addr = INADDR_ANY;
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Couldn't create the socket");
        exit(EXIT_FAILURE);
    }
    if (connect(client_socket, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("Unable to estabelish a connection. Try again later.");
        number_of_recused++;
    }
    return client_socket;
}

void Users::handle_message(int client_socket) {
    int src_id, dest_id;
    char buffer[1024] = {0};
    int err = recv(client_socket, buffer, 1024, 0);
    if (err == -1) {
        perror("Erro while receiving the message.");
        return;
    }
    std::string message(buffer);
    if (!message.empty()) {
        std::string msg = decode_message(message, src_id, dest_id);
    }
}

void Users::close_connection(int client_socket) {
    int err = close(client_socket);

    if (err == -1) {
        perror("Unable to close the connection.");
        exit(EXIT_FAILURE);
    }
}

void *Users::send_message(void *argc) {
    thread_data *data = (thread_data *)argc;
    data->set_client_socket(create_connection(data->server_port));
    std::string encoded_msg =
        encode_message(data->src_id, data->dest_id, "hello there " + std::to_string(1));
    if (send(data->client_socket, encoded_msg.c_str(), encoded_msg.size(), 0) == -1) {
        perror("Could not sent the message.");
        exit(EXIT_FAILURE);
    }
    std::cout << "sent message" << std::endl;

    handle_message(data->client_socket);
    close_connection(data->client_socket);
    delete data;
    pthread_exit(NULL);
}

void Users::create_user(std::string name) {
    if (name == "") {
        name = "user_" + std::to_string(users.size());
    }
    users.push_back(name);
}

void Users::simulate() {
    srand((unsigned)time(NULL));
    pthread_t thread;
    while (true) {
        if ((double)std::rand() / RAND_MAX < p_message) {
            int src_id = std::rand() % users.size();
            int dest_id = std::rand() % users.size();
            thread_data *data = new thread_data(src_id, dest_id, server_port);
            if (pthread_create(&thread, NULL, send_message, (void *)data)) {
                perror("Error creating thread!");
                exit(EXIT_FAILURE);
            }
        }
        sleep_max_ms(500);
    }
    pthread_join(thread, NULL);
}

Users::~Users() {
    std::cout << "recused: " << number_of_recused << std::endl;
    if (active) {
        close_connection(client_socket);
        active = false;
        std::cout << "ending simulation...\n";
    }
}
