#include "device_type.hpp"
#include "hardware_uart.hpp"
#include "link_module.hpp"
#include "wrap-hwlib.hpp"

int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    hwlib::wait_ms(1000);

    UARTLib::HardwareUART hwUart = {9600, UARTLib::UARTController::ONE};

    LinkModule::Master<1> master = {hwUart, {{{0x01, "Button"}}}};

    master.discoverSlaves();

    return 0;
}
