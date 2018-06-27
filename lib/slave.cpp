/**
 * @file
 * @brief     Slave class definition
 * @author    Julian van Doorn
 * @license   See LICENSE
 */

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

Frame Slave::pullData(uint64_t timeoutUs) {
    uint64_t timeoutStamp = hwlib::now_us() + timeoutUs;

    Frame frame;

    if (!frame.receiveHeader(uart, timeoutStamp)) {
        // hwlib::cout << "Timed out" << hwlib::endl;
    }

    Package *packages = packagePool.allocateBlocks(frame.getPackageCount());

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

} // namespace LinkModule