/**
 * @file
 * @brief     Parity Test Cases
 * @author    Julian van Doorn
 * @license   See LICENSE
 */

#ifndef TEST_PARITY_HPP
#define TEST_PARITY_HPP

#include "catch.hpp"
#include "parity.hpp"

TEST_CASE("Parity Bit Calculation Test _calculateParity<bool EVEN_PARITY>", "[linkmodule][parity]") {
    SECTION("Even parity 10101010") {
        REQUIRE(LinkModule::_calculateParity<true>(0b10101010) == 1);
    }

    SECTION("Even parity 10001110") {
        REQUIRE(LinkModule::_calculateParity<true>(0b10001110) == 1);
    }

    SECTION("Even parity 10100011") {
        REQUIRE(LinkModule::_calculateParity<true>(0b10100011) == 1);
    }

    SECTION("Even parity 10111000") {
        REQUIRE(LinkModule::_calculateParity<true>(0b10111000) == 1);
    }

    SECTION("Odd parity 10101010") {
        REQUIRE(LinkModule::_calculateParity<false>(0b10101110) == 1);
    }

    SECTION("Odd parity 10001010") {
        REQUIRE(LinkModule::_calculateParity<false>(0b10001010) == 1);
    }

    SECTION("Odd parity 10100010") {
        REQUIRE(LinkModule::_calculateParity<false>(0b10100010) == 1);
    }

    SECTION("Odd parity 10101000") {
        REQUIRE(LinkModule::_calculateParity<false>(0b10101000) == 1);
    }
}

TEST_CASE("Parity Bit Calculation Test calculateParity", "[linkmodule][parity]") {
    SECTION("Even parity 10101010") {
        REQUIRE(LinkModule::calculateParity(0b10101010) == 1);
    }

    SECTION("Even parity 10001110") {
        REQUIRE(LinkModule::calculateParity(0b10001110) == 1);
    }

    SECTION("Even parity 10100011") {
        REQUIRE(LinkModule::calculateParity(0b10100011) == 1);
    }

    SECTION("Even parity 10111000") {
        REQUIRE(LinkModule::calculateParity(0b10111000) == 1);
    }

    SECTION("Even parity 10101110") {
        REQUIRE(LinkModule::calculateParity(0b10101110) == 0);
    }

    SECTION("Even parity 10001010") {
        REQUIRE(LinkModule::calculateParity(0b10001010) == 0);
    }

    SECTION("Even parity 10100010") {
        REQUIRE(LinkModule::calculateParity(0b10100010) == 0);
    }

    SECTION("Even parity 10101000") {
        REQUIRE(LinkModule::calculateParity(0b10101000) == 0);
    }
}

#endif // TEST_PARITY_HPP