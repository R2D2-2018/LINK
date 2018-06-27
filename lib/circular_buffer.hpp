/**
 * @file
 * @brief     CircularBuffer class definition
 * @author    Julian van Doorn
 * @license   See LICENSE
 */

#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include "wrap-hwlib.hpp"

namespace LinkModule {
/**
 * @defgroup Untested
 * Items in this group are untested.
 * Which means it is likely for them to not work
 */

/**
 * @brief CircularBuffer to store FIFO data
 *
 * @details Behaves like a CircularQueue. Data
 * is enqueued using "write" and dequeued using
 * "read".
 *
 * @tparam T Type to place in data array
 * @tparam L Length of data array
 *
 * @ingroup Untested
 */
template <class T, uint32_t L>
class CircularBuffer {
    T data[L];
    T *readPtr;
    T *writePtr;
    uint32_t count;

  public:
    CircularBuffer() : readPtr(data), writePtr(data), count(0) {
    }

    /**
     * @brief Gives the amount of occupied slots in the buffer
     *
     * @return uint32_t Amount of occupied slots
     */
    uint32_t size() {
        return count;
    }

    /**
     * @brief Places an element in the buffer
     *
     * @details
     * Element is copied into the slot writePtr is pointing to
     * and then writePtr is incremented by one.
     *
     * @param element Element to place in the back
     */
    void write(const T &element) {
        if (writePtr - data >= L) {
            hwlib::cout << "write overflow" << hwlib::endl;
            static int i = 4;
            i--;

            if (i == 0) {
                while (true) {
                }
            }
            writePtr = data;
        }

        *writePtr = element;

        count++;
        writePtr++;
    }

    /**
     * @brief Reads an element from the buffer
     *
     * @details
     * Reference to the slot where readPtr is pointing at
     * is returned and readPtr is incremented by one.
     *
     * @return T& Reference to element read
     */
    T &read() {
        if (readPtr - data > L) {
            hwlib::cout << "read overflow" << hwlib::endl;
            readPtr = data;
        }

        T *temp = readPtr;

        hwlib::cout << "Reading from index: " << (readPtr - data) << hwlib::endl;
        hwlib::cout << "Count: " << count << hwlib::endl;
        hwlib::cout << readPtr << " : " << &data << " + " << L << hwlib::endl;

        readPtr++;
        count--;

        return *temp;
    }
};
} // namespace LinkModule

#endif // CIRCULAR_BUFFER_HPP