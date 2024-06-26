#include <netinet/in.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

class Network_Manager{
    // Server file descriptor
    int connection_fd;
    // Client socket
    int client_socket;

    // Networking details
    struct sockaddr_in address;

    public:
        Network_Manager(uint16_t port);
        ~Network_Manager();
        
        void start_socket();
        void accept_socket();
        int get_connection_fd();
        int get_client_socket();
        void create_socket();
        void create_client_connection(const char *server_ip);
        struct sockaddr_in get_address();
        
    private:
        void bind_socket();
        void listen_socket();
};

inline int Network_Manager::get_connection_fd() {
    return connection_fd;
};

inline struct sockaddr_in Network_Manager::get_address() {
    return address;
};

inline int Network_Manager::get_client_socket() {
    return client_socket;
};