#include "users.h"
#include <netinet/in.h>

int main() {
    sockaddr_in server_address; 
    server_address.sin_family = AF_INET; 
    server_address.sin_port = htons(8080); 
    server_address.sin_addr.s_addr = INADDR_ANY;
    Users client(0.9, 0.05, 10, server_address);
    client.create_connection();
    client.simulate();
    return 0;
}
