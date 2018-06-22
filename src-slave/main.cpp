#include "link_module.hpp"

int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    hwlib::wait_ms(1000);

    UARTLib::HardwareUART hwUart = {9600, UARTLib::UARTController::ONE};

    LinkModule::Slave slave = {hwUart};
    slave.waitForAddress();

    hwlib::cout << "Src-slave hello world!" << hwlib::endl;
}