/**
 * @file
 * @brief     Frame Test Cases
 * @author    Julian van Doorn
 * @license   See LICENSE
 */

#ifndef TEST_FRAME_HPP
#define TEST_FRAME_HPP

#include "catch.hpp"
#include "frame.hpp"
#include "test_connection.hpp"

TEST_CASE("Send Header") {
    SECTION("PackageCount = 10") {
        const uint8_t packageCount = 10;

        TestConnection conn;

        LinkModule::Frame frame = {packageCount};
        frame.sendHeader(conn);

        uint8_t requiredParity = LinkModule::calculateParity(0x13) | LinkModule::calculateParity(0x37) << 1 |
                                 LinkModule::calculateParity(packageCount) << 2;
        requiredParity |= LinkModule::calculateParity(0x13) << 7 | LinkModule::calculateParity(0x37) << 6 |
                          LinkModule::calculateParity(packageCount) << 5;

        REQUIRE(conn[0] == 0x13);
        REQUIRE(conn[1] == 0x37);
        REQUIRE(conn[2] == packageCount);
        REQUIRE(conn[3] == requiredParity);
    }

    SECTION("PackageCount = 15") {
        const uint8_t packageCount = 15;

        TestConnection conn;

        LinkModule::Frame frame = {packageCount};
        frame.sendHeader(conn);

        uint8_t requiredParity = LinkModule::calculateParity(0x13) | LinkModule::calculateParity(0x37) << 1 |
                                 LinkModule::calculateParity(packageCount) << 2;
        requiredParity |= LinkModule::calculateParity(0x13) << 7 | LinkModule::calculateParity(0x37) << 6 |
                          LinkModule::calculateParity(packageCount) << 5;

        REQUIRE(conn[0] == 0x13);
        REQUIRE(conn[1] == 0x37);
        REQUIRE(conn[2] == packageCount);
        REQUIRE(conn[3] == requiredParity);
    }

    SECTION("PackageCount = 40") {
        const uint8_t packageCount = 40;

        TestConnection conn;

        LinkModule::Frame frame = {packageCount};
        frame.sendHeader(conn);

        uint8_t requiredParity = LinkModule::calculateParity(0x13) | LinkModule::calculateParity(0x37) << 1 |
                                 LinkModule::calculateParity(packageCount) << 2;
        requiredParity |= LinkModule::calculateParity(0x13) << 7 | LinkModule::calculateParity(0x37) << 6 |
                          LinkModule::calculateParity(packageCount) << 5;

        REQUIRE(conn[0] == 0x13);
        REQUIRE(conn[1] == 0x37);
        REQUIRE(conn[2] == packageCount);
        REQUIRE(conn[3] == requiredParity);
    }
}

TEST_CASE("Send Footer") {
    SECTION("PackageCount = 10") {
        const uint8_t packageCount = 10;

        TestConnection conn;

        LinkModule::Frame frame = {packageCount};
        frame.sendFooter(conn);

        uint8_t requiredParity = LinkModule::calculateParity(0x37) | LinkModule::calculateParity(0x13) << 1 |
                                 LinkModule::calculateParity(packageCount) << 2;
        requiredParity |= LinkModule::calculateParity(0x37) << 7 | LinkModule::calculateParity(0x13) << 6 |
                          LinkModule::calculateParity(packageCount) << 5;

        REQUIRE(conn[0] == 0x37);
        REQUIRE(conn[1] == 0x13);
        REQUIRE(conn[2] == packageCount);
        REQUIRE(conn[3] == requiredParity);
    }

    SECTION("PackageCount = 13") {
        const uint8_t packageCount = 13;

        TestConnection conn;

        LinkModule::Frame frame = {packageCount};
        frame.sendFooter(conn);

        uint8_t requiredParity = LinkModule::calculateParity(0x37) | LinkModule::calculateParity(0x13) << 1 |
                                 LinkModule::calculateParity(packageCount) << 2;
        requiredParity |= LinkModule::calculateParity(0x37) << 7 | LinkModule::calculateParity(0x13) << 6 |
                          LinkModule::calculateParity(packageCount) << 5;

        REQUIRE(conn[0] == 0x37);
        REQUIRE(conn[1] == 0x13);
        REQUIRE(conn[2] == packageCount);
        REQUIRE(conn[3] == requiredParity);
    }

    SECTION("PackageCount = 26") {
        const uint8_t packageCount = 26;

        TestConnection conn;

        LinkModule::Frame frame = {packageCount};
        frame.sendFooter(conn);

        uint8_t requiredParity = LinkModule::calculateParity(0x37) | LinkModule::calculateParity(0x13) << 1 |
                                 LinkModule::calculateParity(packageCount) << 2;
        requiredParity |= LinkModule::calculateParity(0x37) << 7 | LinkModule::calculateParity(0x13) << 6 |
                          LinkModule::calculateParity(packageCount) << 5;

        REQUIRE(conn[0] == 0x37);
        REQUIRE(conn[1] == 0x13);
        REQUIRE(conn[2] == packageCount);
        REQUIRE(conn[3] == requiredParity);
    }
}

TEST_CASE("Receive Header") {
    SECTION("PackageCount = 10") {
        const uint8_t packageCount = 10;
        const uint64_t timeoutStamp = hwlib::now_us() + 10000;

        TestConnection conn = {0x13, 0x37, packageCount, 0x24};

        LinkModule::Frame frame;
        REQUIRE(frame.receiveHeader(conn, timeoutStamp) == true);
        REQUIRE(frame.getPackageCount() == packageCount);
    }

    SECTION("PackageCount = 25") {
        const uint8_t packageCount = 25;
        const uint64_t timeoutStamp = hwlib::now_us() + 10000;

        TestConnection conn = {0x13, 0x37, packageCount, 0x24};

        LinkModule::Frame frame;
        REQUIRE(frame.receiveHeader(conn, timeoutStamp) == true);
        REQUIRE(frame.getPackageCount() == packageCount);
    }

    SECTION("PackageCount = 30") {
        const uint8_t packageCount = 30;
        const uint64_t timeoutStamp = hwlib::now_us() + 10000;

        TestConnection conn = {0x13, 0x37, packageCount, 0x24};

        LinkModule::Frame frame;
        REQUIRE(frame.receiveHeader(conn, timeoutStamp) == true);
        REQUIRE(frame.getPackageCount() == packageCount);
    }
}

TEST_CASE("Receive Footer") {
    SECTION("PackageCount = 10") {
        const uint8_t packageCount = 10;
        const uint64_t timeoutStamp = hwlib::now_us() + 10000;

        TestConnection conn = {0x37, 0x13, packageCount, 0x24};

        LinkModule::Frame frame;
        REQUIRE(frame.receiveFooter(conn, timeoutStamp) == true);
    }

    SECTION("PackageCount = 25") {
        const uint8_t packageCount = 25;
        const uint64_t timeoutStamp = hwlib::now_us() + 10000;

        TestConnection conn = {0x37, 0x13, packageCount, 0x0};

        LinkModule::Frame frame;
        REQUIRE(frame.receiveFooter(conn, timeoutStamp) == true);
    }

    SECTION("PackageCount = 30") {
        const uint8_t packageCount = 30;
        const uint64_t timeoutStamp = hwlib::now_us() + 10000;

        TestConnection conn = {0x37, 0x13, packageCount, 0x24};

        LinkModule::Frame frame;
        REQUIRE(frame.receiveFooter(conn, timeoutStamp) == true);
    }
}

#endif // TEST_FRAME_HPP