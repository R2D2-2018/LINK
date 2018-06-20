#ifndef LINK_MODULE_HPP
#define LINK_MODULE_HPP

#include <array>

#include "wrap-hwlib.hpp"
#include "data_frame.hpp"
#include "uart_bit_banged.hpp"

namespace LinkModule {
    const hwlib::target::pins gndPin = hwlib::target::pins::d49;
    const hwlib::target::pins btnPin = hwlib::target::pins::d53;
    const hwlib::target::pins txPin = hwlib::target::pins::d14;
    const hwlib::target::pins rxPin = hwlib::target::pins::d15;

    uint8_t slaveAddress;

    void waitForAddress() {

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