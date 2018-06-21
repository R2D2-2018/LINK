#include "device_type.hpp"
#include "hardware_uart.hpp"
#include "link_module.hpp"
#include "wrap-hwlib.hpp"

int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    hwlib::wait_ms(1000);

    SlaveDevice devices[] = {{0x01, "Button"}};

    uint8_t addressMap[127];
    uint8_t assignedAddresses = 0;

    UARTLib::HardwareUART hwUart = {9600, UARTLib::UARTController::ONE};

    while (true) {
        hwlib::wait_ms(100);

        for (SlaveDevice device : devices) {
            while (true) {
                hwlib::cout << "Broadcasting address for device of type " << device.getType() << hwlib::endl;

                LinkModule::AddressAssignment assignment = {device.getAddress()};
                assignment.broadcastAddress(hwUart);

                if (assignment.receiveAddress(hwUart, 100000)) {
                    hwlib::cout << "received confirmation" << hwlib::endl;
                }

                hwlib::wait_ms(1000);
            }
        }

        addressMap[assignedAddresses] = 0x01;
    }

    return 0;
}
