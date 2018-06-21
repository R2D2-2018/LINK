#ifndef ADDRESS_ASSIGNMENT_HPP
#define ADDRESS_ASSIGNMENT_HPP

#include "uart_connection.hpp"
#include "wrap-hwlib.hpp"

namespace LinkModule {
class AddressAssignment {
    uint8_t address;

    static bool calculateParity(uint8_t b) {
        uint8_t highBitCount = 0;

        for (uint8_t i = 0; i < 8; i++) {
            highBitCount += (b >> i & 1);
        }

        return !(highBitCount & 1); // return 1 if highBitCount is even
    };

    inline bool waitForChar(UARTLib::UARTConnection &os, uint64_t timeoutStamp) {
        while (!os.char_available()) {
            if (hwlib::now_us() > timeoutStamp) {
                return false;
            }
        }

        return true;
    }

    inline bool waitForHeader(UARTLib::UARTConnection &os, uint64_t timeoutStamp) {
        char c0;
        char c1;

        while (true) {
            if (!waitForChar(os, timeoutStamp)) {
                return false;
            }

            os >> c0;

            if (c0 == 0x69) {
                if (!waitForChar(os, timeoutStamp)) {
                    return false;
                }

                os >> c1;

                if (c1 == 0x96) {
                    break;
                }
            }

            if (hwlib::now_us() > timeoutStamp) {
                return false;
            }
        }

        return true;
    }

  public:
    AddressAssignment() {
    }

    AddressAssignment(uint8_t address) : address(address) {
    }

    uint8_t getAddress() const {
        return address;
    }

    void broadcastAddress(UARTLib::UARTConnection &os) const {
        // byte 0 - startbyte
        // byte 1 - startbyte
        // byte 2 - broadcastedAddress
        // byte 3 - parityBits

        uint8_t parity = 0;
        parity |= calculateParity(0x69);
        parity |= calculateParity(0x96) << 1;
        parity |= calculateParity(address) << 2;

        os << static_cast<uint8_t>(0x69);
        os << static_cast<uint8_t>(0x96);
        os << address;
        os << parity;
    }

    bool receiveAddress(UARTLib::UARTConnection &os, uint32_t timeoutUs) {
        // byte 0 - startbyte
        // byte 1 - startbyte
        // byte 2 - broadcastedAddress
        // byte 3 - parityBits

        uint64_t timeoutStamp = hwlib::now_us() + timeoutUs;
        char address;
        char parity;

        while (true) {
            if (!waitForHeader(os, timeoutStamp)) {
                return false;
            }

            if (!waitForChar(os, timeoutStamp)) {
                return false;
            }

            os >> address;

            if (!waitForChar(os, timeoutStamp)) {
                return false;
            }

            os >> parity;

            if ((parity & 1) == calculateParity(0x69) && ((parity >> 1) & 1) == calculateParity(0x96) &&
                ((parity >> 2) & 1) == calculateParity(address)) {
                this->address = address;
                return true;
            }
        }
    }

    void confirm(UARTLib::UARTConnection &os) {
        broadcastAddress(os); ///< Place address on Tx for the master to be able to receive confirmation
    }
};
}

#endif // ADDRESS_ASSIGNMENT_HPP