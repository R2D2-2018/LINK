#include "slave.hpp"

namespace LinkModule {
Slave::Slave(UARTLib::UARTConnection &uart, hwlib::pin_in &addressSelect) : uart(uart), addressSelect(addressSelect), address(0) {
}

bool Slave::waitForAddress(uint64_t timeoutUs) {
    uint64_t timeoutStamp = hwlib::now_us() + timeoutUs;

    while (true) {
        Address receivedAddress = {};
        bool success = receivedAddress.receive(uart, timeoutUs);

        hwlib::wait_ms(5);

        if (success && addressSelect.get() == 0) {
            Address confirmationAddress = {receivedAddress};
            confirmationAddress.confirm(uart);
            address = receivedAddress;
            return true; ///< Received address
        } else if (hwlib::now_us() > timeoutStamp) {
            return false; ///< Timed out
        }
    }
}

template <uint32_t L>
void Slave::pushData(const std::array<Frame, L> &frames) {
    ///< No implementation
}

template <uint32_t L>
std::array<Frame, L> Slave::pullData(uint32_t timeout) {
    return {}; ///< No implementation
}
} // namespace LinkModule