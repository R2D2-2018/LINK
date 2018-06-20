#ifndef DATA_FRAME_HPP
#define DATA_FRAME_HPP

#include "wrap-hwlib.hpp"

namespace LinkModule {
    class DataFrame {
        uint8_t d;
    };

    template <class OS>
    OS& operator<< (OS& os, const DataFrame& frame) {
        return os << "DataFrame " << &frame;
    }
}

#endif // DATA_FRAME_HPP