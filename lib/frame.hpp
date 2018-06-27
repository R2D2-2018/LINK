/**
 * @file
 * @brief     Frame class declaration
 * @author    Julian van Doorn
 * @license   See LICENSE
 */

#ifndef FRAME_HPP
#define FRAME_HPP

#include "package.hpp"
#include "parity.hpp"
#include "uart_util.hpp"
#include "wrap-hwlib.hpp"

namespace LinkModule {

/**
 * @brief Frame used to send variable sized data over UART
 *
 * @details
 * Frame layout:
 *
 * startbyte 0 {sendHeader -> receiveHeader}
 * startbyte 1
 * package count
 * parity byte
 *
 * package 0 [0..7] {sendPackages -> receivePackages}
 * package 1 [0..7]
 * package 2 [0..7]
 * package 3 [0..7]
 * package 4 [0..7]
 * package 5 [0..7]
 * package 6 [0..7]
 * package 7 [0..7]
 * parity byte for package 0 .. n, max 7
 * [repeat for every 8 packages]
 *
 * endbyte 0 {sendFooter -> receiveFooter}
 * endbyte 1
 * package count
 * parity byte
 */
class Frame {
    Package *packageBuffer;
    uint8_t packageCount;

  public:
    /**
     * @brief Construct a new Frame object
     *
     * @details
     * packageBuffer and packageCount are uninitialized and thus undefined
     * packageBuffer and packageCount must be set before sending and receiving frames
     */
    Frame();

    /**
     * @brief Construct a new Frame object
     *
     * @details
     * packageBuffer is uninitialized and thus undefined.
     * packageBuffer must be set before sending and receiving frames
     *
     * @param packageCount Amount of packages to send/receive
     */
    Frame(uint8_t packageCount);

    /**
     * @brief Construct a new Frame object
     *
     * @param packageBuffer Buffer to write received packages to or read packages to send from
     * @param packageCount Amount of packages to send/receive
     */
    Frame(Package *packageBuffer, uint8_t packageCount);

    /**
     * @brief Sends the header over UART
     *
     * @param os UART stream to write to
     */
    void sendHeader(UARTLib::UARTConnection &os);

    /**
     * @brief Send the footer over UART
     *
     * @param os UART stream to write to
     */
    void sendFooter(UARTLib::UARTConnection &os);

    /**
     * @brief Receives the header over UART
     *
     * @param is UART stream to read from
     * @param timeoutStamp Timestamp to wait until timing out in us
     * @return true Header received
     * @return false Timed out
     */
    bool receiveHeader(UARTLib::UARTConnection &is, uint64_t timeoutStamp);

    /**
     * @brief Receives the footer over UART
     *
     * @param is UART stream to read from
     * @param timeoutStamp Timestamp to wait until timing out in us
     * @return true Header received
     * @return false Timed out
     */
    bool receiveFooter(UARTLib::UARTConnection &is, uint64_t timeoutStamp);

    /**
     * @brief Sends packageCount packages from packageBuffer over UART
     *
     * @param os UART stream to write to
     *
     * @ingroup Untested
     */
    void sendPackages(UARTLib::UARTConnection &os);

    /**
     * @brief Received packageCount from UART and places them in packageBuffer
     *
     * @param is UART stream to read from
     * @param timeoutStamp Timestamp to wait until timing out in us
     *
     * @ingroup Untested
     */
    void receivePackages(UARTLib::UARTConnection &is, uint64_t timeoutStamp);

    /**
     * @brief Set the packageBuffer pointer to the given pointer
     *
     * @param packageBuffer Pointer to the allocated package buffer
     */
    void setPackageBuffer(Package *packageBuffer);

    /**
     * @brief Gets the packageCount
     *
     * @return uint8_t packageCount
     */
    uint8_t getPackageCount();
};
} // namespace LinkModule

#endif // FRAME_HPP