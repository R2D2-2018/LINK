#ifndef UART_UTIL_HPP
#define UART_UTIL_HPP

#include "uart_lib.hpp"

namespace LinkModule {
inline bool waitForChar(UARTLib::UARTConnection &os, uint64_t timeoutStamp) {
    while (!os.char_available()) {
        if (hwlib::now_us() > timeoutStamp) {
            return false;
        }
    }

    return true;
}

template <uint8_t BYTE_0, uint8_t BYTE_1>
inline bool waitForHeader(UARTLib::UARTConnection &os, uint64_t timeoutStamp) {
    uint8_t c;

    while (true) {
        if (!waitForChar(os, timeoutStamp)) {
            return false;
        }

        os >> c;

        if (c == BYTE_0) {
            if (!waitForChar(os, timeoutStamp)) {
                return false;
            }

            c = 0;
            os >> c;

            if (c == BYTE_1) {
                break;
            }
        }

        if (hwlib::now_us() > timeoutStamp) {
            return false;
        }
    }

    return true;
}
} // namespace LinkModule

#endif // UART_UTIL_HPP