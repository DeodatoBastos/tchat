#ifndef USERS_H
#define USERS_H

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <vector>

struct thread_data {
    int client_socket, dest_id, src_id, server_port;

    thread_data(int dest_id, int src_id, int server_port)
        : dest_id(dest_id), src_id(src_id), server_port(server_port) {}

    void set_client_socket(int client_socket) { this->client_socket = client_socket; }
};

class Users {
    std::vector<std::string> users;
    double p_message, p_user;
    int client_socket;
    int server_port;
    bool active;
    static void *send_message(void *argc);
    static void handle_message(int client_socket);
    static int create_connection(int server_port);
    static void close_connection(int client_socket);
    void create_user(std::string name = "");

  public:
    Users(double p_message, double p_user, int num_users = 0, int port = 8080);
    ~Users();
    void simulate();
};

#endif // USERS_H
