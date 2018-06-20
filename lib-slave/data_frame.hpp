#ifndef DATA_FRAME_HPP
#define DATA_FRAME_HPP

#include "wrap-hwlib.hpp"

namespace LinkModule {
    const int packetSize = 8;

    constexpr bool calculateParity(uint8_t b) {
        uint8_t highBitCount = 0;

        for (uint8_t = 0; i < 8; i++) {
            highBitCount += (b & (1 << i) > 0);
        }

        return !(highBitCoint & 1); // return 1 if highBitCount is even
    }

    class AddressAssignment {
        uint8_t address;
        
      public:
        AddressAssignment(uint8_t address) : address(address) { }

        template <class OS>
        void broadcastAddress(OS& os) {
            // byte 0 - startbyte
            // byte 1 - startbyte
            // byte 2 - broadcastedAddress
            // byte 3 - parityBits

            os << static_cast<uint8_t>(0x69);
            os << static_cast<uint8_t>(0x96);
            os << recipientAddress;
            
            uint8_t parity = 0;
            parity &= calculateParity(0x69);
            parity &= calculateParity(0x96) << 1;
            parity &= calculateParity(recipientAddress) << 2;

            os << parity;
        }

        template <class OS>
        bool receiveAddress(OS& os, uint32_t timeoutUs) {
            // byte 0 - startbyte
            // byte 1 - startbyte
            // byte 2 - broadcastedAddress
            // byte 3 - parityBits

            uint64_t t = hwlib::now_us();
            uint8_t c0;
            uint8_t c1;
            uint8_t address;
            uint8_t parity;

            while (true) {
                os >> c0;

                if (c0 == 0x69) {
                    os >> c1;

                    if (c1 == 0x96) {
                        break;
                    }
                }

                if (hwlib::now_us() - t > timeoutUs) {
                    return false;
                }
            }

            os >> parity;
            os >> address;

            if (parity == calculateParity(0x69) && parity << 1 == calculateParity(0x96) && parity << 2 == calculateParity(address)) {
                this->address = address;
                return true;
            } else {
                return false;
            }
        }

        template <class OS>
        friend OS& operator<< (OS& os, const DataFrame& frame) {
            broadcastAddress(os);
            return os;
        }

        template <class OS>
        friend OS& operator>> (OS& os, DataFrame& frame) {
            receiveAddress(os);
            return os;
        }
    };

    // class Packet {
    //     uint16_t frameId;
    //     uint8_t data[packetSize];
    // };

    // template <int PACKETS_L>
    // class Frame {
    //     uint16_t frameId;
    //     uint8_t recipientAddress;
    //     Packet packets[PACKETS_L];

    //     constexpr bool calculateParity(uint8_t b) {
    //         uint8_t highBitCount = 0;

    //         for (uint8_t = 0; i < 8; i++) {
    //             highBitCount += (b & (1 << i) > 0);
    //         }

    //         return !(highBitCoint & 1); // return 1 if highBitCount is even
    //     }

    //     template <class OS>
    //     void sendFrameHeader(OS& os) {
    //         // byte 0 - startbyte
    //         // byte 1 - startbyte
    //         // byte 2 - frameId
    //         // byte 3 - frameId
    //         // byte 4 - recipientAddress
    //         // byte 5 - parityBits

    //         os << static_cast<uint8_t>(0x69);
    //         os << static_cast<uint8_t>(0x69);
    //         os << frameId;
    //         os << recipientAddress;
            
    //         uint8_t parity = 0;
    //         parity &= calculateParity(0x69);
    //         parity &= calculateParity(0x69) << 1;
    //         parity &= calculateParity(frameId & 0xFF) << 2;
    //         parity &= calculateParity((frameId >> 8) & 0xFF) << 3;
    //         parity &= calculateParity(recipientAddress) << 4;

    //         os << parity;
    //     }

    //     template <class OS>
    //     void sendFrameFooter(OS& os) {
    //         os << static_cast<uint8_t>(0x96);
    //         os << static_cast<uint8_t>(0x96);
    //         os << frameId;
    //         os << recipientAddress;
            
    //         uint8_t parity = 0;
    //         parity &= calculateParity(0x96);
    //         parity &= calculateParity(0x96) << 1;
    //         parity &= calculateParity(frameId) << 2;
    //         parity &= calculateParity(recipientAddress) << 3;

    //         os << parity;
    //     }
    //   public:
    //     template <class OS>
    //     friend OS& operator<< (OS& os, const DataFrame& frame) {
    //         return os << "DataFrame " << &frame;
    //     }
    // }

    // template <class OS>
    // OS& operator<< (OS& os, const DataFrame& frame) {
    //     return os << "DataFrame " << &frame;
    // }
}

#endif // DATA_FRAME_HPP