#ifndef FRAME_HPP
#define FRAME_HPP

#include "wrap-hwlib.hpp"
#include "uart_util.hpp"

namespace LinkModule {

// frame layout:

// startbyte 0
// startbyte 1
// package count
// parity byte

// package 0 [0..7]
// package 1 [0..7]
// package 2 [0..7]
// package 3 [0..7]
// package 4 [0..7]
// package 5 [0..7]
// package 6 [0..7]
// package 7 [0..7]
// parity byte for package 0 .. n, max 7
// [repeat for every 8 packages]

// endbyte 0
// endbyte 1
// package count
// parity byte

class Package {};

class Frame {
    uint8_t packageCount;

    void sendHeader(UARTLib::UARTConnection& os);
    void sendFooter(UARTLib::UARTConnection& os);
    bool receiveHeader(UARTLib::UARTConnection& is, uint64_t timeoutStamp);
    bool receiveFooter(UARTLib::UARTConnection& is, uint64_t timeoutStamp);

public:

};
} // namespace LinkModule

#endif // FRAME_HPP