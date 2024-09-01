#include "Datagram.h"
#include "PseudoHeader.h"

#include <stdio.h>
#include <stdlib.h>
#include <uchar.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

uint16_t calculateChecksum(const struct PseudoHeader pH, const char* data) {
    const int bufferLength = (12 + strlen(data) + 1) / 2;
    uint16_t* buffer = malloc(bufferLength);
    if (buffer == NULL) {
        perror("Unable to allocate memory buffer for checksum calculation.");
        return -1;
    }

    memcpy(buffer, &pH, 12);
    memcpy((char*)buffer + 12, data, strlen(data));

    uint32_t sum = 0;
    for (int i = 0; i < sizeof(buffer) / sizeof(buffer[0]); i++) {
        sum += buffer[i];
        if (sum & 0xFFFF0000) sum = (sum & 0xFFFF) + (sum >> 16);
    }

    free(buffer);
    return ~sum;
}

int main() {
    const int receiveSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (receiveSocket == -1) {
        perror("Failed to create socket.");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddress = {0};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(54321);

    const int rc = bind(receiveSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (-1 == rc) {
        perror("Failed to bind socket.");
        close(receiveSocket);
        exit(EXIT_FAILURE);
    }

    size_t bufferSize = sizeof(struct Datagram) + 100;
    struct Datagram* response = malloc(bufferSize);
    if (response == NULL) {
        perror("Failed to allocate memory.");
        close(receiveSocket);
        exit(EXIT_FAILURE);
    }

    socklen_t addrLen = sizeof(serverAddress);
    printf("Waiting for datagram...\n");

    int recvResponse = recvfrom(
        receiveSocket,
        response,
        bufferSize,
        MSG_WAITALL,
        (struct sockaddr*)&serverAddress,
        &addrLen
    );
    close(receiveSocket);

    if (-1 == recvResponse) {
        perror("Failed to receive datagram from server.");
        close(receiveSocket);
        exit(EXIT_FAILURE);
    }
    printf("Received payload of %d bytes: %s\n", recvResponse, response->data);
    printf("sourcePort: %d\n", ntohs(response->sourcePort));
    printf("destinationPort: %d\n", ntohs(response->destinationPort));
    printf("length: %d\n", ntohs(response->length));
    printf("checksum: %d\n", ntohs(response->checksum));
    printf("data: %s\n", response->data);

    // Verify checksum
    struct PseudoHeader pH;
    initPseudoHeader(&pH,
        htons(54321),
        htons(54321),
        17,
        8 + strlen(response->data)
    );
    const uint16_t responseChecksum = calculateChecksum(pH, response->data);
    if (responseChecksum == ntohs(response->checksum)) printf("Checksums match\n");
    else {
        printf("Checksums do not match!\n");
        printf("Received: %d\n", response->checksum);
        printf("Calculated: %d\n", responseChecksum);
    }

    return 0;
}