#include "wrap-hwlib.hpp"
#include "hardware_uart.hpp"

template <uint32_t L>
class DeviceType {
    uint8_t typeId;
    hwlib::string<L> typeStr;

  public:
    DeviceType() : typeId(), typeStr() { }

    DeviceType(uint8_t typeId, const char* typeStr) : typeId(typeId), typeStr(typeStr) { }

    uint8_t getId() {
        return typeId;
    }

    const hwlib::string<L>& getType() {
        return typeStr;
    }
};

int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    hwlib::wait_ms(1000);

    using SlaveDevice = DeviceType<32>;

    SlaveDevice devices[] = {
        { 0x01, "Distance" }
    };

    // uint8_t addressMap[127] = { };
    // uint8_t assignedAddresses = 0;

    hwlib::target::pin_out tx = { hwlib::target::pins::d23 };
    hwlib::target::pin_in rx = { hwlib::target::pins::d22 };

    tx.set(1);

    HardwareUART hwUart = { 9600, UARTController::ONE };

    while (true) {
        hwlib::wait_ms(100);
        hwUart << "Hello world!";
    }


    // for (SlaveDevice device : devices) {
    //     while (true) {
    //         hwlib::cout << "Broadcasting address for device of type " << device.getType() << hwlib::endl;
    //         i2c.write(0x00, dat, 12);
    //         hwlib::wait_ms(1000);
    //     }
    // }

    // addressMap[assignedAddresses] = 0x01;

    return 0;
}
