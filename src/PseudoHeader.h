#ifndef PSEUDOHEADER_H
#define PSEUDOHEADER_H
#include <stdint.h>

struct PseudoHeader {
    uint32_t sourceAddress;
    uint32_t destinationAddress;
    uint8_t zero;
    uint8_t protocol;
    uint16_t tcpLength;
};

static void initPseudoHeader(
    struct PseudoHeader* header,
    const uint32_t srcAddr,
    const uint32_t destAddr,
    const uint8_t protocol,
    const uint16_t length) {
    header->sourceAddress = srcAddr;
    header->destinationAddress = destAddr;
    header->zero = 0x00;
    header->protocol = protocol;
    header->tcpLength = length;
}
#endif //PSEUDOHEADER_H
