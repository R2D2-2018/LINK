#ifndef SLAVE_HPP
#define SLAVE_HPP

#include <array>

#include "wrap-hwlib.hpp"

#include "address.hpp"
#include "frame.hpp"
#include "parity.hpp"
#include "uart_lib.hpp"
#include "object_pool.hpp"

namespace LinkModule {
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
     */
    template <uint32_t L>
    void pushData(const std::array<Frame, L> &frames)  {
        ///< No implementation
    }

    /**
     * @brief Not implemented
     *
     * @param timeoutUs Timeout in microseconds
     */
    Frame pullData(uint64_t timeoutUs) {
        uint64_t timeoutStamp = hwlib::now_us() + timeoutUs;

        Frame frame;

        if (!frame.receiveHeader(uart, timeoutStamp)) {
            // hwlib::cout << "Timed out" << hwlib::endl;
        }

        Package* packages = packagePool.allocateBlocks(frame.getPackageCount());

        if (packages != nullptr) {
            // hwlib::cout << packagePool << hwlib::endl;

            frame.setPackageBuffer(packages);
            frame.receivePackages(uart, timeoutStamp);
            frame.receiveFooter(uart, timeoutStamp);

            for (int i = 0; i < frame.getPackageCount(); i++) {
                hwlib::cout << hwlib::hex << hwlib::setw(2) << hwlib::setfill('0') << packages[i] << hwlib::endl;
            }

            packagePool.deallocateBlocks(packages);
        } else {
            ///< Pool full
        }

        return frame;
    }

    uint8_t getAddress() {
        return address;
    }
};
} // namespace LinkModule

#endif // SLAVE_HPP