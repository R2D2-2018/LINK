/**
 * @file
 * @brief     Slave class declaration
 * @author    Julian van Doorn
 * @license   See LICENSE
 */

#ifndef SLAVE_HPP
#define SLAVE_HPP

#include <array>

#include "wrap-hwlib.hpp"

#include "address.hpp"
#include "frame.hpp"
#include "object_pool.hpp"
#include "parity.hpp"
#include "uart_lib.hpp"

namespace LinkModule {
/**
 * @brief Slave class
 *
 * @details
 * Used for hosting all required resources for communicating
 * with the master. pushData should be ran periodically in
 * order to provide other modules with data from this module.
 */
class Slave {
    UARTLib::UARTConnection &uart;
    hwlib::pin_in &addressSelect;
    uint8_t address;
    ObjectPool<Package, 256> packagePool;

  public:
    /**
     * @brief Construct a new Slave object
     *
     * @param uart UARTConnection to communicate with
     */
    Slave(UARTLib::UARTConnection &uart, hwlib::pin_in &addressSelect);

    /**
     * @brief Blocks operations until an address is received
     *
     * @details
     * Blocks operation until an address is received.
     * When timed out, a bool is returned signaling
     * the timeout.
     *
     * @param timeoutUs Microseconds to wait until timeout
     * @return true Address address received
     * @return false Timed out
     */
    bool waitForAddress(uint64_t timeoutUs);

    /**
     * @brief Not implemented
     *
     * @tparam L Amount of frames to push
     * @param frames Frames to push
     *
     * @ingroup Unimplemented
     */
    template <uint32_t L>
    void pushData(const std::array<Frame, L> &frames) {
        ///< No implementation
    }

    /**
     * @brief Waits until timeoutUs has passed for all slaves to send their data
     *
     * @details
     * Undetermined what a good way is for this method to work.
     *
     * How this method eventually should work is as following:
     * The method waits for the master to receive data targeted
     * for the slave module. (Using address in header)
     * Then the method formats the data in a usable manner,
     * so the module code can react accordingly.
     *
     * @param timeoutUs Timeout in microseconds
     *
     * @ingroup Untested
     * @ingroup Unimplemented
     */
    Frame pullData(uint64_t timeoutUs);

    uint8_t getAddress() {
        return address;
    }
};
} // namespace LinkModule

#endif // SLAVE_HPP