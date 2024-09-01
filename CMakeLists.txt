cmake_minimum_required(VERSION 3.29)
project(udp_c C)

set(CMAKE_C_STANDARD 23)

add_executable(server src/server.c src/Datagram.h
        src/PseudoHeader.h
)
add_executable(client src/client.c src/Datagram.h
)
