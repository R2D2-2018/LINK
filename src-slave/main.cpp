/**
 * @file
 * @brief     Slave Main Loop
 * @author    Julian van Doorn
 * @license   See LICENSE
 */

#include "link_module.hpp"

int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    hwlib::wait_ms(1000);

    hwlib::target::pin_in asp = {hwlib::target::pins::d48};
    asp.pullup_enable();

    UARTLib::HardwareUART hwUart = {115200, UARTLib::UARTController::ONE};

    hwlib::target::pin_out gnd = {hwlib::target::pins::d52};
    gnd.set(0);

    LinkModule::Slave slave = {hwUart, asp};

    while (true) {
        if (slave.waitForAddress(10000000)) {
            hwlib::cout << "Address received" << hwlib::endl;
            break;
        } else {
            hwlib::cout << "Timeout, trying again" << hwlib::endl;
        }
    }

    hwlib::cout << "Assigned address: " << static_cast<int>(slave.getAddress()) << hwlib::endl;

    // while (true) {
    //     LinkModule::Frame f = slave.pullData(1000000);

    //     hwlib::wait_ms(10);
    // }
}