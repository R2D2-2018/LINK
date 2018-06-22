#ifndef DEVICE_TYPE_HPP
#define DEVICE_TYPE_HPP

#include "wrap-hwlib.hpp"

namespace LinkModule {
template <uint32_t L>
class DeviceType {
    uint8_t address;
    hwlib::string<L> typeStr;

  public:
    /**
     * @brief Construct an uninitialized
     */
    DeviceType() : address(), typeStr() {
    }

    /**
     * @brief Construct a new Device Type object with address and typeStr
     * 
     * @param address Address to use
     * @param typeStr Device type name
     */
    DeviceType(uint8_t address, const char *typeStr) : address(address), typeStr(typeStr) {
    }

    /**
     * @brief Get the Address device
     * 
     * @return uint8_t Address of device
     */
    uint8_t getAddress() {
        return address;
    }

    /**
     * @brief Get the TypeString of device
     * 
     * @return const hwlib::string<L>& TypeString of device
     */
    const hwlib::string<L> &getType() {
        return typeStr;
    }

    /**
     * @brief Writes device in string to OS 
     * 
     * @tparam OS ostream
     * @param os ostream to write to
     * @param device Device to write to os
     * @return OS& ostream
     */
    template <class OS>
    friend OS& operator << (OS& os, DeviceType<L> device) {
        return os << (int) device.getAddress() << " : " << device.getType();
    }
};

/**
 * @brief SlaveDevice declaration. Max size for typeStr is 32 (including terminating null)
 */
using SlaveDevice = DeviceType<32>;
} // namespace LinkModule

#endif // DEVICE_TYPE_HPP