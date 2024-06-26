#include <iostream>
#include <oqs/oqs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <chrono>
#include "../core/KEM_Manager/KEM_Manager.h"
#include "../core/Network_Manager/Network_Manager.h"

// Exposed Port
#define PORT 1194
// define which KEM mechanism to use
#define KEM_METHOD "Kyber1024"


int main() {
    KEM_Manager kem_manager(KEM_METHOD);
    
    // Generate key exchange/encapsulation mechanism
    kem_manager.generate_KEM();
    // Retrieve kem instance.
    OQS_KEM *kem = kem_manager.get_KEM();
    // Generate key pair
    kem_manager.generate_keypair();

    // Create network manager
    Network_Manager network_manager(PORT);

    // Set up socket
    int server_fd = network_manager.get_connection_fd();
    struct sockaddr_in address = network_manager.get_address();
    int addrlen = sizeof(address);

    // Try to create & bind socket
    try {
        network_manager.start_socket();
    } catch (string &e) {
        throw e;
    }

    std::cout << "Server is now listening on port " << PORT 
        << std::endl;

    try {
        network_manager.accept_socket();
    } catch (string &e) {
        throw e;
    }

    std::cout << "Server accepted connection from client" 
        << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    // Derive shared secret
    try {
        kem_manager.derive_shared_secret(
            network_manager.get_client_socket());
    } catch (char const *e) {
        cout << e << endl;
        throw e;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Time taken: " << duration.count() 
        << " seconds" << std::endl;

    return 0;
}
