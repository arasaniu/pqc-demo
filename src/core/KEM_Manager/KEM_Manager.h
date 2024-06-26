#include <oqs/oqs.h>
#include <string>
#include <iostream>
#include <sys/socket.h>

using namespace std;

class KEM_Manager {
    OQS_KEM *kem;
    const char *KEM_METHOD;
    uint8_t *public_key;
    uint8_t *secret_key;
    uint8_t *shared_secret;
    uint8_t *cipher_text;

public:
    KEM_Manager(const char *KEM_METHOD);
    ~KEM_Manager();
    void generate_KEM();
    void generate_keypair();  
    void derive_shared_secret(int connection);
    void retrieve_public_key(int connection);
    void encapsulate_KEM(int connection, uint8_t *ciphertext);
    void send_cipher_text(int connection, uint8_t *ciphertext);

    OQS_KEM *get_KEM();
    uint8_t * get_public_key();
    uint8_t * get_secret_key();
    uint8_t * get_cipher_text();
private:
    void decapsulate_KEM(const uint8_t *ciphertext);

};

inline OQS_KEM *KEM_Manager::get_KEM() {
    return kem;
}

inline uint8_t *KEM_Manager::get_public_key() {
    return public_key;
}

inline uint8_t *KEM_Manager::get_secret_key() {
    return secret_key;
}

inline uint8_t *KEM_Manager::get_cipher_text() {
    return cipher_text;
}