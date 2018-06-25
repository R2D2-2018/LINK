#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include "wrap-hwlib.hpp"
#include "uart_lib.hpp"
#include "uart_util.hpp"

#include <array>

namespace LinkModule {
template <uint32_t SIZE>
class SizedPackage {
  public:
    std::array<uint8_t, SIZE> data;

    SizedPackage() : data() { }
    SizedPackage(const std::array<uint8_t, SIZE>& data) : data(data) { }

    void sendPackage(UARTLib::UARTConnection& os) {
        for (uint32_t i = 0; i < SIZE; i++) {
            os << data[i];
        }
    }

    bool receivePackage(UARTLib::UARTConnection& is, uint64_t timeoutStamp) {
        for (uint32_t i = 0; i < SIZE; i++) {
            if (!waitForChar(is, timeoutStamp)) {
                return false;
            }

            is >> data[i];
        }

        return true;
    }

    template <class OS>
    friend OS& operator << (OS& os, const SizedPackage& sizedPackage) {
        for (uint8_t d : sizedPackage.data) {
            os << (int) d;
        }

        return os;
    }
};

using Package = SizedPackage<16>;
} // namespace LinkModule

#endif // PACKAGE_HPP