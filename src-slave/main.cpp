#include "link_module.hpp"

int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    hwlib::wait_ms(1000);

    hwlib::target::pin_in asp = {hwlib::target::pins::d48};
    asp.pullup_enable();

    UARTLib::HardwareUART hwUart = {9600, UARTLib::UARTController::ONE};

    hwlib::target::pin_out gnd = {hwlib::target::pins::d52};
    gnd.set(0);

    LinkModule::Slave slave = {hwUart, asp};

    while (true) {
        if (slave.waitForAddress(10000000)) {
            break;
        } else {
            hwlib::cout << "Timeout, trying again" << hwlib::endl;
        }
    }

    hwlib::cout << "Src-slave hello world! Assigned address: " << (int)slave.getAddress() << hwlib::endl;
}