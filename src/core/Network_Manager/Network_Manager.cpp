#include "Network_Manager.h"

Network_Manager::Network_Manager(uint16_t port) : 
    connection_fd(0), client_socket(0), address()
{   
    address.sin_port = htons(port);
    // Indicate address is IPv4
    address.sin_family = AF_INET;
    // Bind to all interfaces
    address.sin_addr.s_addr = INADDR_ANY; 
}

Network_Manager::~Network_Manager() {
    close(connection_fd);
    close(client_socket);
}

void Network_Manager::create_socket() {
    connection_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (connection_fd == 0) {
        throw "Socket creation failed";
    }
}

void Network_Manager::bind_socket() {
    if (bind(connection_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        throw "Bind failed";
    }
}

void Network_Manager::listen_socket() {
    if (listen(connection_fd, 3) < 0) {
        throw "Listening to socket failed";
    }
}

void Network_Manager::accept_socket() {
    int addrlen = sizeof(address);
    client_socket = accept(connection_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (client_socket < 0) {
        throw "Accept failed";
    }
}

void Network_Manager::start_socket() {
    try {
        create_socket();
        bind_socket();
        listen_socket();
    } catch (string &e) {
        throw e;
    }
}

void Network_Manager::create_client_connection(const char *server_ip) {
    inet_pton(AF_INET, server_ip, &address.sin_addr);
    if (connect(connection_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        throw "Connection to server failed";
    }   
}