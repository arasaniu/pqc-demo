# Dockerfile
FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

# Install necessary dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    curl \
    libssl-dev \
    ninja-build \
    vim \
    iproute2 \
    libpthread-stubs0-dev

# Clone and install liboqs
RUN git clone --branch main --depth 1 https://github.com/open-quantum-safe/liboqs.git /opt/liboqs && \
    cd /opt/liboqs && \
    mkdir build && cd build && \
    cmake -GNinja .. && \
    ninja && \
    ninja install

# Set the working directory
WORKDIR /app

# Copy the source code
COPY . /app

# Compile the source code
RUN make server && \
    make client

# Command to run the server by default
CMD ["bash"]
