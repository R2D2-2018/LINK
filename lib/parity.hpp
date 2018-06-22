#ifndef PARITY_HPP
#define PARITY_HPP

#include "wrap-hwlib.hpp"

namespace LinkModule {
/**
 * @brief Calculates a parity bit for the provided byte, uses EVEN_PARITY for even or odd parity
 *
 * @tparam EVEN_PARITY sets even or odd parity
 * @param b Byte to calculate parity for
 * @return high Parity bit is 1
 * @return low Parity bit is 0
 */
template <bool EVEN_PARITY>
constexpr bool _calculateParity(uint8_t b) {
    uint8_t highBitCount = 0;

    for (uint8_t i = 0; i < 8; i++) {
        highBitCount += (b >> i & 1);
    }

    return (highBitCount & 1) != EVEN_PARITY; // return 1 if highBitCount is even
}

/**
 * @brief Calculates a parity bit for the provided byte
 *
 * @details
 * This function is a wrapper for _calculateParity<true>(b)
 * for possible easy re-configruation of the CRC parity.
 * Currently uses even parity.
 *
 * @param b Byte to calculate parity for
 * @return high Parity bit is 1
 * @return low Parity bit is 0
 */
constexpr bool calculateParity(uint8_t b) {
    return _calculateParity<true>(b);
}
} // namespace LinkModule

#endif // PARITY_HPP