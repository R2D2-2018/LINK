/**
 * @file
 * @brief     Address class defintion
 * @author    Julian van Doorn
 * @license   See LICENSE
 */

#include "address.hpp"

namespace LinkModule {
Address::Address() {
}

Address::Address(uint8_t address) : address(address) {
}

void Address::broadcast(UARTLib::UARTConnection &os) const {
    ///< byte 0 - startbyte
    ///< byte 1 - startbyte
    ///< byte 2 - broadcastedAddress
    ///< byte 3 - parityBits

    uint8_t parity = 0;
    parity |= calculateParity(0x69);
    parity |= calculateParity(0x96) << 1;
    parity |= calculateParity(address) << 2;

    os << static_cast<uint8_t>(0x69);
    os << static_cast<uint8_t>(0x96);
    os << address;
    os << parity;
}

bool Address::receive(UARTLib::UARTConnection &is, uint32_t timeoutUs) {
    ///< byte 0 - startbyte
    ///< byte 1 - startbyte
    ///< byte 2 - broadcastedAddress
    ///< byte 3 - parityBits

    uint64_t timeoutStamp = hwlib::now_us() + timeoutUs;
    uint8_t receivedAddress;
    uint8_t parity;

    while (true) {
        if (!waitForHeader<0x69, 0x96>(is, timeoutStamp)) {
            return false;
        }

        if (!waitForChar(is, timeoutStamp)) {
            return false;
        }

        is >> receivedAddress;

        if (!waitForChar(is, timeoutStamp)) {
            return false;
        }

        is >> parity;

        uint8_t requiredParity = calculateParity(0x69) | calculateParity(0x96) << 1 | calculateParity(receivedAddress) << 2;

        if (parity == requiredParity) {
            address = receivedAddress;
            return true;
        } else {
            ///< Parity bits incorrect, try again
        }
    }
}

void Address::confirm(UARTLib::UARTConnection &os) {
    broadcast(os); ///< Place address on Tx for the master to be able to receive confirmation
}
} // namespace LinkModule