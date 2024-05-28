#include "user.h"
#include <cstdio>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h> 

Users::Users(double p_msg, double p_u, int n) : p_message(p_msg), p_user(p_u) {
    users.reserve(n);
    for (int i = 0; i < n; i++) {
        users.push_back("user_"+std::to_string(i));
    }

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
}

Users::Users(double p_msg, double p_u) : p_message(p_msg), p_user(p_u) {
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
}

void Users::create_connection(int user_id) {
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;
    server_address = address;

    int err = connect(client_socket, (struct sockaddr*)& server_address, sizeof(server_address));

    if (err != 0) {
        perror("Unable to estabelish a connection. Try again later.");
        return;
    }
}

void Users::close_connection() {
    int err = close(client_socket);

    if (err != 0) {
        perror("Unable to close the connection.");
        return;
    }
}

void Users::send_message(int user_id, std::string message) {
    create_connection(user_id);
    send(client_socket, message.c_str(), message.size(), 0);
    close_connection();
}

void Users::create_user(std::string name) {
    if (name == "") {
        name = "user_" + std::to_string(users.size());
    }
    users.push_back(name);
}