// The header file is a simple struct that represents a UDP datagram
// The datagram class follows RFC 768

#ifndef DATAGRAM_H
#define DATAGRAM_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Datagram {
    uint16_t sourcePort;
    uint16_t destinationPort;
    uint16_t length;
    uint16_t checksum;
    char data[];
};

static struct Datagram* initDatagram(
    const uint16_t sourcePort,
    const uint16_t destinationPort,
    const uint16_t length,
    const uint16_t checksum,
    const char data[] ) {

    struct Datagram* datagram = malloc(sizeof(struct Datagram) + length - 8);
    if (datagram == NULL) {
        perror("Failed to allocate memory for Datagram.");
        exit(EXIT_FAILURE);
    }

    datagram->sourcePort = sourcePort;
    datagram->destinationPort = destinationPort;
    datagram->length = length;
    datagram->checksum = checksum;

    memcpy(datagram->data, data, length - 8); // Copy the payload into the data field

    return datagram;
}

#endif //DATAGRAM_H