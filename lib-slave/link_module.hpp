#ifndef LINK_MODULE_HPP
#define LINK_MODULE_HPP

#include <array>

#include "wrap-hwlib.hpp"
#include "data_frame.hpp"
#include "hardware_uart.hpp"
#include "uart_bit_banged.hpp"

namespace LinkModule {
    const hwlib::target::pins gndPin = hwlib::target::pins::d49;
    const hwlib::target::pins btnPin = hwlib::target::pins::d53;
    const hwlib::target::pins txPin = hwlib::target::pins::d18;
    const hwlib::target::pins rxPin = hwlib::target::pins::d19;

    uint8_t slaveAddress;

    HardwareUART hwUart = { 9600, UARTController::ONE };

    void waitForAddress() {
        

        while (true) {
            char c[21];
            for (int i = 0; i < 20; ) {
                if (hwUart.available() > 0) {
                    hwUart >> c[i];
                    i++;
                }
            }
            c[20] = 0;
            hwlib::cout << c;
        }

    }

    template <uint32_t L>
    void pushData(const std::array<DataFrame, L>& frames) {
        
    }

    template <uint32_t L>
    std::array<DataFrame, L> pullData(uint32_t timeout) {
        return { };
    }
}

#endif // LINK_MODULE_HPP