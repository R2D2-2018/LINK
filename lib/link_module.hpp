#ifndef LINK_MODULE_HPP
#define LINK_MODULE_HPP

#include <array>

#include "wrap-hwlib.hpp"

#include "address_assignment.hpp"
#include "data_frame.hpp"

#include "hardware_uart.hpp"

namespace LinkModule {
class Slave {
    const hwlib::target::pins gndPin = hwlib::target::pins::d49;
    const hwlib::target::pins btnPin = hwlib::target::pins::d53;
    const hwlib::target::pins txPin = hwlib::target::pins::d18;
    const hwlib::target::pins rxPin = hwlib::target::pins::d19;

    uint8_t slaveAddress;

    UARTLib::HardwareUART hwUart = {9600, UARTLib::UARTController::ONE};

  public:
    void waitForAddress() {
        AddressAssignment assignment = {};
        bool success = assignment.receiveAddress(hwUart, 10000000);

        hwlib::wait_ms(5);

        if (success) {
            hwlib::cout << "RECEIVED ADDRESS: " << (int)assignment.getAddress() << hwlib::endl;
            assignment.confirm(hwUart);
        } else {
            hwlib::cout << "TIMEOUT" << hwlib::endl;
        }
    }

    template <uint32_t L>
    void pushData(const std::array<Frame, L> &frames) {
    }

    template <uint32_t L>
    std::array<Frame, L> pullData(uint32_t timeout) {
        return {};
    }
};
}

#endif // LINK_MODULE_HPP