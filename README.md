# pqc-demo
Several examples for basic implementations of pqc algorithms

Build Docker

`docker build -t serv .`

Start Server Instance

`docker run -it --rm --name oqs-server -p 1194:1194 serv ./server`

Start Client Instance

`docker run --rm --name oqs-client --network="host" serv ./client`