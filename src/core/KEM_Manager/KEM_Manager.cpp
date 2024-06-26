#include "KEM_Manager.h"

/**
 * @brief Construct a new kem manager with null keys and kem. Only defined a method.
 * 
 * @param KEM_METHOD the OQS method to use for key exchange
 */
KEM_Manager::KEM_Manager(const char *KEM_METHOD) :
    KEM_METHOD(KEM_METHOD), public_key(NULL), secret_key(NULL), kem(NULL)
{}

/**
 * @brief Destroy the kem manager
 * free all keys and kem.
 */
KEM_Manager::~KEM_Manager() {
    if (public_key) {
        free(public_key);
    }
    if (secret_key) {
        free(secret_key);
    }
    if (shared_secret) {
        free(shared_secret);
    }
    free(kem);
}

/**
 * @brief Use liboqs method to creat new kem instance.
 */
void KEM_Manager::generate_KEM() {
    OQS_KEM *newKem = OQS_KEM_new(KEM_METHOD);
    if (newKem == NULL) {
        throw "generate_KEM failed to generate KEM";
    }
    kem = newKem;
}

/**
 * @brief Create secret and public keys for the kem instance.
 */
void KEM_Manager::generate_keypair() {
    public_key = (uint8_t *)malloc(kem->length_public_key);
    secret_key = (uint8_t *)malloc(kem->length_secret_key);
    if (OQS_KEM_keypair(kem, public_key, secret_key) != OQS_SUCCESS) {
        throw "OQS_KEM_keypair creation failed";
    }
}

/**
 * @brief extract shared secret from ciphertext.
 * @param ciphertext 
 */
void KEM_Manager::decapsulate_KEM(const uint8_t *ciphertext) {
    shared_secret = (uint8_t *)malloc(kem->length_shared_secret);
    if (OQS_KEM_decaps(kem, shared_secret, ciphertext, secret_key) != OQS_SUCCESS) {
        throw "OQS_KEM_decaps (decapsulation) failed";
    }
}

/**
 * @brief exchange public keys with client and derive shared secret.
 * 
 * @param connection the connection established between client and server.
 */
void KEM_Manager::derive_shared_secret(int connection) {
    if (send(connection, public_key, kem->length_public_key, 0) <= 0) {
        throw "Error sending public key to client";
    }
    uint8_t ciphertext[kem->length_ciphertext];
    if (recv(connection, ciphertext, kem->length_ciphertext, 0) <= 0) {
        throw "Error receiving ciphertext from client";
    }
    try {
        decapsulate_KEM(ciphertext);
        std::cout << "Shared secret established" << std::endl;
    } catch (string &e) {
        throw e;
    }
}


/**
 * @brief gets public key from connection
 * @param connection the connection established between client and server.
 */
void KEM_Manager::retrieve_public_key(int connection) {
    public_key = (uint8_t *)malloc(kem->length_public_key);
    if (recv(connection, public_key, sizeof(public_key), 0) <= 0) {
        throw "Error receiving public key from server";
    }
}

/**
 * @brief allocates memory and encapsulates shared secret.
 * 
 * @param connection the connection established between client and server.
 * @param ciphertext to be sent to server.
 */
void KEM_Manager::encapsulate_KEM(int connection, uint8_t *ciphertext) {
    try {
        retrieve_public_key(connection);
    } catch (string &e) {
        throw e;
    }

    shared_secret = (uint8_t *)malloc(kem->length_shared_secret);
    if (OQS_KEM_encaps(kem, ciphertext, shared_secret, public_key) != OQS_SUCCESS) {
        throw "OQS_KEM_encaps failed" ;
    }

    send_cipher_text(connection, ciphertext);
}

/**
 * @brief sends cipher text
 * 
 * @param connection the connection established between client and server.
 * @param ciphertext the cipher text to send.
 */
void KEM_Manager::send_cipher_text(int connection, uint8_t *ciphertext) {
    if (send(connection, ciphertext, sizeof(ciphertext), 0) <= 0) {
        throw "Error sending ciphertext to server";
    }
}