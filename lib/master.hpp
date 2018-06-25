#ifndef MASTER_HPP
#define MASTER_HPP

#include <array>

#include "address.hpp"
#include "circular_buffer.hpp"
#include "device_type.hpp"
#include "frame.hpp"
#include "object_pool.hpp"
#include "uart_lib.hpp"

namespace LinkModule {
/**
 * @brief Master with slave discovery
 *
 * @details
 * Contains a list of devices that have
 * to be discovered by a subroutine.
 * This class contains all functionality
 * of the master module (link module).
 *
 * @tparam DEVICES_L Amount of devices to discover
 */
template <int DEVICES_L>
class Master {
    UARTLib::UARTConnection &uart;

    std::array<SlaveDevice, DEVICES_L> devices;

  public:
    CircularBuffer<Frame, 6> frameBuffer;

    /**
     * @brief Construct a new Master object
     *
     * @details
     * Calls default constructor for uart and devices`
     */
    Master() : uart(), devices() {
    }

    /**
     * @brief Construct a new Master object
     *
     * @param uart
     * @param devices
     */
    Master(UARTLib::UARTConnection &uart, const std::array<SlaveDevice, DEVICES_L> &devices) : uart(uart), devices(devices) {
    }

    /**
     * @brief Discovers all slaves using R2D2 Simple Config (RSC)
     *
     * @details
     * RSC is a protocol for discovering slaves of
     * the master (linkmodule). This protocol is
     * currently implemented using UART but in
     * theory is also compatible using I2C.
     *
     * More info about RSC:
     * https://github.com/R2D2-2018/R2D2-2018/wiki/Link-Module#rsc---r2d2-simple-config
     */
    void discoverSlaves() {
        for (LinkModule::SlaveDevice device : devices) {
            bool discovered = false;

            while (!discovered) {
                ///< Broadcasting address for `device`

                LinkModule::Address address = {device.getAddress()};

                address.broadcast(uart);

                LinkModule::Address confirmationAddress;

                confirmationAddress.receive(uart, 1000000); ///< Wait for acceptance from slave

                if (device.getAddress() == confirmationAddress) {
                    discovered = true; ///< Accepted address matches broadcasted one
                }
            }
        }
    }

    /**
     * @brief
     */
    void transmitBuffer() {
        while (frameBuffer.size() > 0) {
            Frame frame = frameBuffer.read();

            hwlib::cout << "transmit" << hwlib::endl;

            frame.sendHeader(uart);
            frame.sendPackages(uart);
            frame.sendFooter(uart);
        }
    }

    /**
     * @brief
     */
    void receiveBuffer() {
    }
};
} // namespace LinkModule

#endif // MASTER_HPP