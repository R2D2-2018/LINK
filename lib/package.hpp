/**
 * @file
 * @brief     Package class declaration
 * @author    Julian van Doorn
 * @license   See LICENSE
 */

#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include "uart_lib.hpp"
#include "uart_util.hpp"
#include "wrap-hwlib.hpp"

#include <array>

namespace LinkModule {
/**
 * @brief SizedPackage for sending data over UART
 *
 * @details
 * Package type for sending and receiving data over
 * UART.
 *
 * @tparam SIZE Size of the data array to send or receive
 * @ingroup Untested
 */
template <uint32_t SIZE>
class SizedPackage {
  public:
    std::array<uint8_t, SIZE> data;

    /**
     * @brief Construct a new SizedPackage with uninitialized data
     *
     * @details
     * Before sending data, the data array must be filled with data.
     * Otherwise undefined data is sent.
     */
    SizedPackage() : data() {
    }

    /**
     * @brief Construct a new SizedPackage
     *
     * @param data Data to send
     */
    SizedPackage(const std::array<uint8_t, SIZE> &data) : data(data) {
    }

    /**
     * @brief Iterates over data array and sends it to UART
     *
     * @param os UART stream to send data to
     */
    void sendPackage(UARTLib::UARTConnection &os) {
        for (uint32_t i = 0; i < SIZE; i++) {
            os << data[i];
        }
    }

    /**
     * @brief Reads from UART until data is filled or timed out
     *
     * @param is UART stream to read data from
     * @param timeoutStamp Timestamp to wait until before timing out in us
     * @return true Package received
     * @return false Timed out
     */
    bool receivePackage(UARTLib::UARTConnection &is, uint64_t timeoutStamp) {
        for (uint32_t i = 0; i < SIZE; i++) {
            if (!waitForChar(is, timeoutStamp)) {
                return false;
            }

            is >> data[i];
        }

        return true;
    }

    /**
     * @brief
     * Writes the data array to the ostream in integer format
     *
     * @details
     * This method can be used to send data over UART, it is
     * advised to use the sendPackage method in order to provide
     * consistency and more predictable behavior.
     *
     * @tparam OS stream type to write to
     * @param os OStream to write to
     * @param sizedPackage Package to send
     * @return OS& Reference to os for chaining << calls
     */
    template <class OS>
    friend OS &operator<<(OS &os, const SizedPackage &sizedPackage) {
        for (uint8_t d : sizedPackage.data) {
            os << (int)d;
        }

        return os;
    }
};

/**
 * @brief Package typedef for packages with 16 bytes
 */
using Package = SizedPackage<16>;
} // namespace LinkModule

#endif // PACKAGE_HPP