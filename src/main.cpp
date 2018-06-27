/**
 * @file
 * @brief     Master Main Loop
 * @author    Julian van Doorn
 * @license   See LICENSE
 */

#include "device_type.hpp"
#include "hardware_uart.hpp"
#include "link_module.hpp"
#include "wrap-hwlib.hpp"

int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    hwlib::wait_ms(1000);

    UARTLib::HardwareUART hwUart = {115200, UARTLib::UARTController::ONE};

    LinkModule::Master<1> master = {hwUart, {{{0x01, "Button"}}}};
    master.discoverSlaves();

    hwlib::cout << "All slaves discovered!" << hwlib::endl;

    // while (true) {
    //     LinkModule::Package packages[1];

    //     packages[0].data[0] = 0xFF;
    //     packages[0].data[1] = 0xFF;
    //     packages[0].data[2] = 0xFF;
    //     packages[0].data[3] = 0xFF;

    //     master.frameBuffer.write({ packages, 1 });

    //     master.transmitBuffer();
    //     hwlib::wait_ms(10);
    // }

    return 0;
}
