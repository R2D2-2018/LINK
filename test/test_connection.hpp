/**
 * @file
 * @brief     TestConnection class definition
 * @author    Julian van Doorn
 * @license   See LICENSE
 */

#ifndef TEST_CONNECTION_HPP
#define TEST_CONNECTION_HPP

#include "uart_lib.hpp"
#include "wrap-hwlib.hpp"

#include <queue>
#include <vector>

class TestConnection : public UARTLib::UARTConnection {
    std::vector<char> sendBuffer;
    std::queue<char> receiveQueue;

  public:
    TestConnection() {
    }

    TestConnection(std::initializer_list<char> rxData) : receiveQueue(rxData) {
    }

    char &operator[](uint32_t i) {
        return sendBuffer.at(i);
    }

    uint32_t size() {
        return sendBuffer.size();
    }

    void begin() {
    }

    inline void enable() {
    }

    inline void disable() {
    }

    unsigned int available() {
        return receiveQueue.size();
    }

    bool send(const uint8_t b) {
        sendByte(b);

        return true;
    }

    bool send(const uint8_t *str) {
        for (const uint8_t *p = str; *p != '\0'; p++) {
            sendByte(*p);
        }

        return true;
    }

    bool send(const char *str) {
        for (const char *p = str; *p != '\0'; p++) {
            sendByte(*p);
        }

        return true;
    }

    bool send(const uint8_t *data, size_t length) {
        for (unsigned int i = 0; i < length; i++) {
            sendByte(data[i]);
        }

        return true;
    }

    uint8_t receive() {
        return receiveByte();
    }

    bool isInitialized() {
        return true;
    }

    void putc(char c) {
        sendByte(c);
    }

    char getc() {
        if (available() > 0) {
            return receive();
        }

        return 0;
    }

    bool char_available() {
        return true;
    }

  private:
    void sendByte(const uint8_t &b) {
        sendBuffer.emplace_back(b);
    }

    inline uint8_t receiveByte() {
        uint8_t b = receiveQueue.front();
        receiveQueue.pop();
        return b;
    }

    inline bool txReady() {
        return true;
    }
};

#endif // TEST_CONNECTION_HPP