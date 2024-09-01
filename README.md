# UDP Communication Simulator

## Overview
This project is a UDP communication simulator developed in C, adhering to RFC 768 standards. It includes a UDP server and client to send and receive datagrams, with checksum validation to ensure data integrity.

## Features
- **UDP Server and Client**: Implements basic UDP communication between a server and a client.
- **Checksum Validation**: Ensures data integrity by calculating and verifying checksums.
- **Logging**: Provides detailed logging for tracking packet flow and debugging.

## Getting Started

### Prerequisites
- CMake 3.29 or higher
- GCC or any C compiler
- Linux operating system

### Building the Project
1. Clone the repository:
    ```sh
    git clone https://github.com/Rahat464/udp-c.git
    cd udp-c
    ```

2. Create a build directory and navigate into it:
    ```sh
    mkdir build
    cd build
    ```

3. Run CMake to configure the project:
    ```sh
    cmake ..
    ```

4. Build the project:
    ```sh
    make
    ```

### Running the Server
1. Navigate to the build directory:
    ```sh
    cd build
    ```

2. Run the server executable:
    ```sh
    ./server
    ```

### Running the Client
1. Navigate to the build directory:
    ```sh
    cd build
    ```

2. Run the client executable:
    ```sh
    ./client
    ```

## Project Structure
- `src/`: Contains the source code for the server and client.
  - `server.c`: Implementation of the UDP server.
  - `client.c`: Implementation of the UDP client.
  - `Datagram.h`: Definition of the UDP datagram structure.
  - `PseudoHeader.h`: Definition of the pseudo-header used for checksum calculation.
- `CMakeLists.txt`: CMake configuration file for building the project.

## License
This project is licensed under the MIT License. See the `LICENSE` file for more details.