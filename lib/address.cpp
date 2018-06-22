#include "address.hpp"

namespace LinkModule {
inline bool Address::waitForChar(UARTLib::UARTConnection &os, uint64_t timeoutStamp) {
    while (!os.char_available()) {
        if (hwlib::now_us() > timeoutStamp) {
            return false;
        }
    }

    return true;
}

inline bool Address::waitForHeader(UARTLib::UARTConnection &os, uint64_t timeoutStamp) {
    unsigned char c;

    while (true) {
        if (!waitForChar(os, timeoutStamp)) {
            return false;
        }

        os >> c;

        if (c == 0x69) {
            if (!waitForChar(os, timeoutStamp)) {
                return false;
            }

            c = 0;
            os >> c;

            if (c == 0x96) {
                break;
            }
        }

        if (hwlib::now_us() > timeoutStamp) {
            return false;
        }
    }

    return true;
}

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
        if (!waitForHeader(is, timeoutStamp)) {
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