#include "slave.hpp"

namespace LinkModule {
Slave::Slave(UARTLib::UARTConnection &uart) : uart(uart) {
}

bool Slave::waitForAddress(uint64_t timeoutUs) {
    Address address = {};
    bool success = address.receive(uart, timeoutUs);

    hwlib::wait_ms(5);

    if (success) {
        Address confirmationAddress = {address};
        confirmationAddress.confirm(uart);
        return true; ///< Received address
    } else {
        return false; ///< Timed out
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