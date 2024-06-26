#include <iostream>
#include <oqs/oqs.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "../core/KEM_Manager/KEM_Manager.h"
#include "../core/Network_Manager/Network_Manager.h"

#define PORT 1194
// define which KEM mechanism to use
#define KEM_METHOD "Kyber1024"
// default server ip. On some machines this may differ.
#define SERVER_IP "127.0.0.1"

int main() {
    KEM_Manager kem_manager(KEM_METHOD);
    kem_manager.generate_KEM();

    // Retrieve kem instance.
    OQS_KEM *kem = kem_manager.get_KEM();
    Network_Manager network_manager(PORT);

    // Attempt server connection
    try {
        network_manager.create_socket();
        network_manager.create_client_connection(SERVER_IP);
    } catch (char const *e) {
        cerr << e << endl;
    }

    // Receive public key from server
    int connection = network_manager.get_connection_fd();


    // Generate the ciphertext and shared secret
    uint8_t ciphertext[kem->length_ciphertext];
    kem_manager.encapsulate_KEM(connection, ciphertext);

    // Send ciphertext to server    
    std::cout << "Shared secret established" << std::endl;

    return 0;
}
