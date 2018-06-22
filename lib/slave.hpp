#ifndef SLAVE_HPP
#define SLAVE_HPP

#include <array>

#include "wrap-hwlib.hpp"

#include "address.hpp"
#include "frame.hpp"
#include "parity.hpp"
#include "uart_lib.hpp"

namespace LinkModule {
class Slave {
    UARTLib::UARTConnection& uart;

public:
    /**
     * @brief Construct a new Slave object
     * 
     * @param uart UARTConnection to communicate with
     */
    Slave(UARTLib::UARTConnection& uart);

    /**
     * @brief Blocks operations until an address is received
     * 
     * @details
     * Blocks operation until an address is received. 
     * When timed out, a bool is returned signaling 
     * the timeout.
     * 
     * @param timeoutUs 
     * @return true Address address received
     * @return false Timed out
     */
    bool waitForAddress(uint64_t timeoutUs);

    /**
     * @brief Not implemented
     * 
     * @tparam L Amount of frames to push
     * @param frames Frames to push
     */
    template <uint32_t L>
    void pushData(const std::array<Frame, L> &frames);

    /**
     * @brief Not implemented
     * 
     * @tparam L Amount of frames to pull
     * @param timeoutUs Timeout in microseconds
     * @return std::array<Frame, L> Pulled frames
     */
    template <uint32_t L>
    std::array<Frame, L> pullData(uint32_t timeoutUs);
};
} // namespace LinkModule

#endif // SLAVE_HPP