// This is a simple implementation of a UDP server that sends a datagram to a client.
// The aim is to follow RFC 768 (https://tools.ietf.org/html/rfc768)
#include "Datagram.h"
#include "PseudoHeader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <arpa/inet.h>

void terminate(const bool success, const int sendSocket) {
    close(sendSocket);

    if (success) exit(EXIT_SUCCESS);
    exit(EXIT_FAILURE);
}

uint16_t calculateChecksum(const struct PseudoHeader pH, const char* data) {
    // 1. Concatenate the Pseudo Header, UDP Header, and Data
    // + 1 to account for odd lengths, then divide by 2 to get the number of 16-bit integers
    const int bufferLength = (12 + strlen(data) + 1) / 2;
    uint16_t* buffer = malloc(bufferLength);
    if (buffer == NULL) {
        perror("Unable to allocate memory buffer for checksum calculation.");
        terminate(false, -1);
    }

    // Copy the Pseudo Header and data into the buffer
    memcpy(buffer, &pH, 12);
    memcpy((char*)buffer + 12, data, strlen(data));

    // Sum all 16-bit words using one’s complement arithmetic.
    uint32_t sum = 0;
    for (int i = 0; i < sizeof(buffer) / sizeof(buffer[0]); i++) {
        sum += buffer[i];
        // If the sum overflows, add the overflow to the sum
        if (sum & 0xFFFF0000) sum = (sum & 0xFFFF) + (sum >> 16);
    }

    free(buffer);
    return ~sum; // Swap bits
}

char* takeInput() {
    static char input[100];
    printf("Enter a message: ");
    if (NULL == fgets(input, 100, stdin)) {
        perror("Failed to read input.");
        exit(EXIT_FAILURE);
    }
    input[strcspn(input, "\n")] = 0; // Remove newline char
    return input;
}

int main() {
    // AF_INET specifies IPv4, SOCK_DGRAM specifies UDP, 0 leaves the protocol up to the system
    const int sendSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == sendSocket) {
        perror("Failed to create sockets.");
        exit(EXIT_FAILURE);
    }

    // Initialize client addresses, then fill them with zeros
    // sockaddr_in is a struct that represents an IPv4 internet address
    struct sockaddr_in clientAddress = {0};

    // Configure client address to specify the port and address for UDP communication
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = INADDR_ANY;
    clientAddress.sin_port = htons(54321);

    // Initialise PseudoHeader for checksum calculation
    struct PseudoHeader pH;
    const char* payload = takeInput();

    initPseudoHeader(&pH,
        htons(54321),
        htons(54321),
        17, // Protocol number for UDP
        8 + strlen(payload) 
    );

    size_t datagramSize = sizeof(struct Datagram) + strlen(payload) + 1;
    struct Datagram* datagram = malloc(datagramSize);
    if (NULL == datagram) {
        perror("Failed to allocate memory for datagram.");
        terminate(false, sendSocket);
    }

    datagram->sourcePort = htons(54321);
    datagram->destinationPort = htons(54321);
    datagram->length = htons(8 + strlen(payload));
    datagram->checksum = htons(calculateChecksum(pH, payload));
    strcpy(datagram->data, payload);


    /* sendto() sends a datagram to the specified address and port
     * &datagram is a pointer to the datagram to send
     * sizeof(datagram) is the size of the datagram
     * 0 is used to specify that there are no flags
     * (struct sockaddr*)&clientAddress is a pointer to the client's socket
     * sizeof(clientAddress) is the size of the client's socket
     */
    const int sendResponse = sendto(
        sendSocket,
        datagram,
        sizeof(struct Datagram) + strlen(payload),
        0,
        (struct sockaddr*)&clientAddress,
        sizeof(clientAddress)
    );
    if (-1 == sendResponse) {
        perror("Failed to send datagram to client.");
        free(datagram);
        terminate(false, sendSocket);
    }

    printf("Datagram successfully sent %u bytes to client \n", sendResponse);
    printf("sourcePort: %d\n", ntohs(datagram->sourcePort));
    printf("destinationPort: %d\n", ntohs(datagram->destinationPort));
    printf("length: %d\n", ntohs(datagram->length));
    printf("checksum: %d\n", ntohs(datagram->checksum));
    printf("data: %s\n", datagram->data);

    terminate(true, sendSocket);
}