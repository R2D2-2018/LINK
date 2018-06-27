/**
 * @file
 * @brief     ObjectPool class definition
 * @author    Julian van Doorn
 * @license   See LICENSE
 */

#ifndef OBJECT_POOL_HPP
#define OBJECT_POOL_HPP

#include <math.h>

namespace LinkModule {
/**
 * @brief Pool-like heap implementation for embedded environments.
 *
 * @details
 * This class is designed to function similarly to what a heap would do.
 * But instead it has to be allocated by the programmer and is fixed size during execution.
 * It is designed to support only one datatype. If you want to use multiple datatypes, it is
 * possible but it would require the pool to set up for a small datatype like uint32_t and a
 * lot of intermediate casting. Therefore it is disadvised to do stuff like that.
 *
 * There is some memory overhead. Because empty blocks of data are used as borders between allocations.
 * That way the deallocation algorithm knows where to stop deallocation. That means a ObjectPool<char, 256>
 * holds a little bit less than 256 characters.
 *
 * @code{.cpp}
ObjectPool<char, 64> pool = { };
std::cout << pool << std::endl;                             // ObjectPool with availabilityBitmask:
1111111111111111111111111111111111111111111111111111111111111111
char* p0 = pool.allocateBlocks(13);
std::cout << "Allocated 13 bytes" << std::endl;             // Allocated 13 bytes
std::cout << pool << std::endl;                             // ObjectPool with availabilityBitmask:
0000000000000111111111111111111111111111111111111111111111111111
p0[0] = 'H';
p0[1] = 'e';
p0[2] = 'l';
p0[3] = 'l';
p0[4] = 'o';
p0[5] = ' ';
p0[6] = 'w';
p0[7] = 'o';
p0[8] = 'r';
p0[9] = 'l';
p0[10] = 'd';
p0[11] = '!';
p0[12] = '\0';
char* p1 = pool.allocateBlocks(13);
std::cout << "Allocated another 13 bytes" << std::endl;     // Allocated another 13 bytes
std::cout << pool << std::endl;                             // ObjectPool with availabilityBitmask:
0000000000000100000000000001111111111111111111111111111111111111
strcpy(p1, p0);
p0[6] = 'R';
p0[7] = '2';
p0[8] = 'D';
p0[9] = '2';
p0[10] = '\0';
std::cout << p0 << std::endl;                               // Hello R2D2
std::cout << p1 << std::endl;                               // Hello world!
pool.deallocateBlocks(p0);
std::cout << "Deallocated 13 bytes (p0)" << std::endl;      // Deallocated 13 bytes (p0)
std::cout << pool << std::endl;                             // ObjectPool with availabilityBitmask:
1111111111111100000000000001111111111111111111111111111111111111
uint32_t* myInt = (uint32_t*) pool.allocateBlocks(4);
std::cout << "Allocated 4 bytes" << std::endl;              // Allocated 4 bytes
std::cout << pool << std::endl;                             // ObjectPool with availabilityBitmask:
0000111111111100000000000001111111111111111111111111111111111111
*myInt = 0xDEADBEEF;
std::cout << std::hex << *myInt << std::endl;               // deadbeef
pool.deallocateBlocks(p1);
pool.deallocateBlocks((char*) myInt);
std::cout << "Deallocated p1 and myInt" << std::endl;       // Deallocated p1 and myInt
std::cout << pool << std::endl;                             // ObjectPool with availabilityBitmask:
1111111111111111111111111111111111111111111111111111111111111111
 * @endcode
 *
 *
 * @tparam T Type of the blocks to allocate. Requires a default constructor.
 * @tparam POOL_SIZE Amount of blocks to allocate
 */
template <class T, uint32_t POOL_SIZE>
class ObjectPool {
  private:
    static constexpr uint32_t calculateAvailabilityBitmaskLenLen() {
        if (POOL_SIZE % 8 == 0) {
            return POOL_SIZE / 8;
        } else {
            return POOL_SIZE / 8 + 1;
        }
    }

    static constexpr uint32_t availabilityBitmaskLen = calculateAvailabilityBitmaskLenLen();

    // Required: for every block, 1 bit, minimum of 1 byte
    // 1 is available, 0 is unavailable
    uint8_t availabilityBitmask[availabilityBitmaskLen];
    T blocks[POOL_SIZE];

    void setAvailable(const uint32_t i, const bool b) {
        if (i < POOL_SIZE) {
            if (b == true) {
                availabilityBitmask[i / 8] |= 1 << (i % 8);
            } else {
                availabilityBitmask[i / 8] &= ~(1 << (i % 8));
            }
        }
    }

    bool getAvailable(const uint32_t i) const {
        if (i < POOL_SIZE) {
            return availabilityBitmask[i / 8] & 1 << (i % 8);
        } else {
            return false;
        }
    }

    template <class OS>
    void printBlockAvailability(OS &os) const {
        for (uint32_t i = 0; i < availabilityBitmaskLen; i++) {
            for (uint8_t b = 0; b < 8; b++) {
                os << ((uint32_t)availabilityBitmask[i] >> b & 1);
            }
        }
    }

  public:
    /**
     * @brief Construct a new Object Pool object
     *
     * @details
     * Sets all the bytes in availabilityBitmask to 0xFF as initial state.
     *
     */
    ObjectPool() : blocks() {
        for (uint32_t i = 0; i < availabilityBitmaskLen; i++) {
            availabilityBitmask[i] = 0xFF;
        }
    }

    /**
     * @brief Allocates n blocks with type T
     *
     * @details
     * This method allocates n blocks of type T. Whichs means that the physical length in bytes
     * of the allocation is determined by T's physical size. If allocation was successful,
     * a pointer to the first allocated T is returned. If allocation failed, a nullptr
     * is returned, to indicate failure.
     *
     * @param n Blocks to allocate
     * @return T* Pointer to array of T types
     * @return nullptr_t Failed allocation
     */
    T *allocateBlocks(const uint32_t n) {
        uint32_t availabilityCounter = 0;
        int32_t firstAvailableIndex = -1;
        bool found = false;

        for (uint32_t i = 0; i < POOL_SIZE; i++) {
            if ((i == 0 || getAvailable(i - 1)) && getAvailable(i)) {
                if (firstAvailableIndex == -1) {
                    firstAvailableIndex = i;
                }

                availabilityCounter++;

                if (availabilityCounter == n && getAvailable(firstAvailableIndex + availabilityCounter + 1)) {
                    found = true;
                    break;
                }
            } else {
                firstAvailableIndex = -1;
                availabilityCounter = 0;
            }
        }

        if (found == true) {
            for (uint32_t i = firstAvailableIndex; i < firstAvailableIndex + availabilityCounter; i++) {
                setAvailable(i, false);
            }

            return &blocks[firstAvailableIndex];
        } else {
            return nullptr;
        }
    }

    /**
     * @brief Deallocates all blocks that are in series with ptr
     *
     * @details
     * This method takes the T* and deallocates all allocations that came behind it.
     * Whichs means if char* p = ObjectPool<char, 256>::allocateBlocks(10) was called,
     * ChassPool<char, 256>::deallocateBlocks(p) must be called in order to deallocate
     * all blocks allocated at p.
     *
     * @param ptr Pointer to the first element of the allocated elements.
     */
    void deallocateBlocks(const T *ptr) {
        uint32_t blockIndex = (ptr - blocks);

        while (getAvailable(blockIndex) == false) {
            setAvailable(blockIndex, true);
            blockIndex++;
        }
    }

    /**
     * @brief Print operator for ObjectPool
     *
     * @details
     * Prints "ObjectPool with availabilityBitmask: xxxxxxxx" where xxxxxxxx is a list
     * of ones and zeroes representing the availabilityBitmask. The length of said list
     * varies based on the given POOL_SIZE
     *
     * @tparam OS
     * @param os Output Stream to write to
     * @param cp ObjectPool to print
     * @return OS& Allows for chaining << calls
     */
    template <class OS>
    friend OS &operator<<(OS &os, const ObjectPool &cp) {
        os << "ObjectPool with availabilityBitmask: ";
        cp.printBlockAvailability(os);
        return os;
    }
};
} // namespace LinkModule

#endif // OBJECT_POOL_HPP