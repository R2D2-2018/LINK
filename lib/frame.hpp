#ifndef FRAME_HPP
#define FRAME_HPP

#include "parity.hpp"
#include "uart_util.hpp"
#include "wrap-hwlib.hpp"
#include "package.hpp"

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

class Frame {
    Package* packageBuffer;
    uint8_t packageCount;

  public:
    Frame();
    Frame(uint8_t packageCount);
    Frame(Package* packageBuffer, uint8_t packageCount);

    void sendHeader(UARTLib::UARTConnection &os);
    void sendFooter(UARTLib::UARTConnection &os);
    bool receiveHeader(UARTLib::UARTConnection &is, uint64_t timeoutStamp);
    bool receiveFooter(UARTLib::UARTConnection &is, uint64_t timeoutStamp);

    uint8_t getPackageCount() {
        return packageCount;
    }
    void sendPackages(UARTLib::UARTConnection &os);
    void receivePackages(UARTLib::UARTConnection& is, uint64_t timeoutStamp);
    void setPackageBuffer(Package* packageBuffer);
};
} // namespace LinkModule

#endif // FRAME_HPP