#include "frame.hpp"

namespace LinkModule {
Frame::Frame() : packageCount() {
}

Frame::Frame(uint8_t packageCount) : packageCount(packageCount) {
}

void Frame::sendHeader(UARTLib::UARTConnection &os) {
    ///< byte 0 - startbyte
    ///< byte 1 - startbyte
    ///< byte 2 - package count
    ///< byte 3 - parityBits

    uint8_t parity = calculateParity(0x13) | calculateParity(0x37) << 1 | calculateParity(packageCount) << 2;
    parity |= calculateParity(0x13) << 7 | calculateParity(0x37) << 6 | calculateParity(packageCount) << 5;

    os << static_cast<uint8_t>(0x13);
    os << static_cast<uint8_t>(0x37);
    os << packageCount;
    os << parity;
}

void Frame::sendFooter(UARTLib::UARTConnection &os) {
    ///< byte 0 - startbyte
    ///< byte 1 - startbyte
    ///< byte 2 - package count
    ///< byte 3 - parityBits

    uint8_t parity = calculateParity(0x37) | calculateParity(0x13) << 1 | calculateParity(packageCount) << 2;
    parity |= calculateParity(0x37) << 7 | calculateParity(0x13) << 6 | calculateParity(packageCount) << 5;

    os << static_cast<uint8_t>(0x37);
    os << static_cast<uint8_t>(0x13);
    os << packageCount;
    os << parity;
}

bool Frame::receiveHeader(UARTLib::UARTConnection &is, uint64_t timeoutStamp) {
    uint8_t receivedPackageCount;
    uint8_t parity;

    while (true) {
        if (!waitForHeader<0x13, 0x37>(is, timeoutStamp)) {
            return false;
        }

        if (!waitForChar(is, timeoutStamp)) {
            return false;
        }

        is >> receivedPackageCount;

        if (!waitForChar(is, timeoutStamp)) {
            return false;
        }

        is >> parity;

        uint8_t requiredParity = calculateParity(0x13) | calculateParity(0x37) << 1 | calculateParity(packageCount) << 2;
        requiredParity |= calculateParity(0x13) << 7 | calculateParity(0x37) << 6 | calculateParity(packageCount) << 5;

        if (parity == requiredParity) {
            packageCount = receivedPackageCount;
            return true;
        } else {
            ///< Parity bits incorrect, try again
        }
    }
}

bool Frame::receiveFooter(UARTLib::UARTConnection &is, uint64_t timeoutStamp) {
    uint8_t receivedPackageCount;
    uint8_t parity;

    while (true) {
        if (!waitForHeader<0x37, 0x13>(is, timeoutStamp)) {
            return false;
        }

        if (!waitForChar(is, timeoutStamp)) {
            return false;
        }

        is >> receivedPackageCount;

        if (!waitForChar(is, timeoutStamp)) {
            return false;
        }

        is >> parity;

        uint8_t requiredParity = calculateParity(0x37) | calculateParity(0x13) << 1 | calculateParity(receivedPackageCount) << 2;
        requiredParity |= calculateParity(0x37) << 7 | calculateParity(0x13) << 6 | calculateParity(receivedPackageCount) << 5;

        if (parity == requiredParity) {
            return true;
        } else {
            ///< Parity bits incorrect, try again
        }
    }
}
} // namespace LinkModule