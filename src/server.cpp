#include "server.h"
#include "utils.h"
#include <cstdio>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

Server::Server(int domain, int type, int protocol, int port, int address,
               int num_requests)
    : n_requests(num_requests), active(false) {
  server_socket = socket(domain, type, protocol);
  server_address.sin_family = domain;
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = address;
  int err = bind(server_socket, (struct sockaddr *)&server_address,
                 sizeof(server_address));

  if (err != 0) {
    perror("Cound not bind in server.");
  }
}

void Server::listen_client() {
  int err = listen(server_socket, n_requests);
  if (err != 0) {
    perror("Cound not listen for client requests");
    return;
  }
  active = true;
}

void Server::handle_message() {
  while (active) {
    int client_socket = accept(server_socket, nullptr, nullptr);
    char buffer[1024] = {0};
    recv(client_socket, buffer, sizeof(buffer), 0);
    int src_id, dest_id;
    std::string msg = decode_message(std::string(buffer), src_id, dest_id);
    std::cout << "from: " << src_id << "; to: " << dest_id << "; msg: " << msg << "\n";
  }
}

void Server::close_connection() {
  int err = close(server_socket);
  if (err != 0) {
    perror("Cound not close connection.");
  }
  active = false;
}

Server::~Server() {
  if (active)
    close_connection();
}
