/**
 * @file
 * @brief     UART Utility function definitions
 * @author    Julian van Doorn
 * @license   See LICENSE
 */

#ifndef UART_UTIL_HPP
#define UART_UTIL_HPP

#include "uart_lib.hpp"

namespace LinkModule {
/**
 * @brief Waits until timeoutStamp for a character to be placed on os
 *
 * @param os UART stream to wait for
 * @param timeoutStamp Timestamp to wait until before timing out
 * @return true Char available for reading
 * @return false Timed out
 */
inline bool waitForChar(UARTLib::UARTConnection &os, uint64_t timeoutStamp) {
    while (!os.char_available()) {
        if (hwlib::now_us() > timeoutStamp) {
            return false;
        }
    }

    return true;
}

/**
 * @brief Waits until BYTE_0 and BYTE_1 have been read in order from UART
 *
 * @tparam BYTE_0 Start byte 0
 * @tparam BYTE_1 Start byte 1
 * @param os UART stream to read from
 * @param timeoutStamp Timestamp to wait until before timing out
 * @return true Correct header received
 * @return false Timed out
 */
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