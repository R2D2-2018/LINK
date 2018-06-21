#ifndef DEVICE_TYPE_HPP
#define DEVICE_TYPE_HPP

template <uint32_t L>
class DeviceType {
    uint8_t address;
    hwlib::string<L> typeStr;

  public:
    DeviceType() : address(), typeStr() {
    }

    DeviceType(uint8_t address, const char *typeStr) : address(address), typeStr(typeStr) {
    }

    uint8_t getAddress() {
        return address;
    }

    const hwlib::string<L> &getType() {
        return typeStr;
    }
};

using SlaveDevice = DeviceType<32>;

#endif // DEVICE_TYPE_HPP