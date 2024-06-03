#ifndef USERS_H
#define USERS_H

#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <vector>

class Users {
    std::vector<std::string> users;
    double p_message, p_user;
    int client_socket;
    sockaddr_in server_address;
    bool active;
    void send_message(int src_id, int dest_id, std::string message);
    void handle_message();
    void create_connection();
    void close_connection();
    void create_user(std::string name = "");

  public:
    Users(double p_message, double p_user, int num_users=0, int port=8080);
    ~Users();
    void simulate();
};

#endif // USERS_H
