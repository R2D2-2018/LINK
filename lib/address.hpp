#ifndef ADDRESS_HPP
#define ADDRESS_HPP

#include "uart_connection.hpp"
#include "wrap-hwlib.hpp"

#include "parity.hpp"

namespace LinkModule {
/**
 * @brief Address wrapper for uint8_t
 * 
 * @details
 * Wrapper for sending and retrieving uint8_t addresses.
 * This wrapper includes CRC checksums for receiving and
 * transmitting addresses. That way the integrity of
 * the communication is sustained.
 */
class Address {
    uint8_t address;

    inline bool waitForChar(UARTLib::UARTConnection &os, uint64_t timeoutStamp);

    inline bool waitForHeader(UARTLib::UARTConnection &os, uint64_t timeoutStamp);

  public:
    /**
     * @brief Constructs an uninitialized address
     */
    Address();

    /**
     * @brief Constructs an initialized address
     * 
     * @param address Address to wrap
     */
    Address(uint8_t address);

    /**
     * @brief Places this address on the communication line
     * 
     * @details
     * Sends two start bytes, the address
     * and a parity byte. The communication line curently
     * is simply an UART tx line. In the future this method
     * can be improved to use a protocol like I2C. 
     * 
     * @param os UARTConnection to transmit to
     */
    void broadcast(UARTLib::UARTConnection &os) const;

    /**
     * @brief Receives an address from the communication line
     * 
     * @details
     * Waits for two valid start bytes, an address
     * and a correct parity byte. If the parity byte
     * is incorrect, it simply tries again until
     * the timeoutUs has been exceeded.
     * 
     * @param is UARTConnection to receive from
     * @param timeoutUs Timeout in microseconds
     * @return true Successfully received address
     * @return false Timed out, no address received
     */
    bool receive(UARTLib::UARTConnection &is, uint32_t timeoutUs);

    /**
     * @brief Places a confirmation message on the communication line
     * 
     * @brief
     * Currently just calls broadcast and
     * the master is expected to receive
     * this message.
     * 
     * @param os 
     */
    void confirm(UARTLib::UARTConnection &os);

    operator uint8_t() {
        return address;
    }
};
} // namespace LinkModule

#endif // ADDRESS_HPP