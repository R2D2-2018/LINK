#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

#include "wrap-hwlib.hpp"

template <class T, uint32_t L>
class CircularBuffer {
    T data[L];
    T* readPtr;
    T* writePtr;
    uint32_t count;

public:
    CircularBuffer() : readPtr(data), writePtr(data), count(0) {}

    uint32_t size() {
        return count;
    }

    void write(const T& element) {
        if (writePtr - data >= L) {
            hwlib::cout << "write overflow" << hwlib::endl;
            static int i = 4;
            i--;

            if (i == 0) {
                while (true) {}
            }
            writePtr = data;
        }

        *writePtr = element;

        count++;
        writePtr++;
    }

    T& read() {
        if (readPtr - data > L) {
            hwlib::cout << "read overflow" << hwlib::endl;
            readPtr = data;
        }
        
        T* temp = readPtr;

        hwlib::cout << "Reading from index: " << (readPtr - data) << hwlib::endl;
        hwlib::cout << "Count: " << count << hwlib::endl;
        hwlib::cout << readPtr << " : " << &data << " + " << L << hwlib::endl;

        readPtr++;
        count--;

        return *temp;
    }
};

#endif // CIRCULAR_BUFFER_HPP