client: KEM_Manager Network_Manager
	g++ src/client/client.cpp KEM_Manager.o Network_Manager.o -o client -loqs -lcrypto -lpthread

server: KEM_Manager Network_Manager
	g++ src/server/server.cpp KEM_Manager.o Network_Manager.o -o server -loqs -lcrypto -lpthread

KEM_Manager:
	g++ src/core/KEM_Manager/KEM_Manager.cpp -c -o KEM_Manager.o -loqs -lcrypto -lpthread

Network_Manager:
	g++ src/core/Network_Manager/Network_Manager.cpp -c -o Network_Manager.o -loqs -lcrypto -lpthread

clean:
	rm -f server.o
	rm -f client.o
	rm -f KEM_Manager.o
	rm -f Network_Manager.o