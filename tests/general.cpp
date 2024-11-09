/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <bygg/bygg.hpp>
#include <catch2/catch_test_macros.hpp>
#include "test.hpp"

// NOLINTBEGIN
// Disable linting. This is a test program and the code is intentionally bad in some places.
// As such, I think it's best to turn linting off rather than to try and resolve any such warnings.

void General::test_exceptions() {
    try {
        throw bygg::out_of_range{};
    } catch (const bygg::out_of_range& e) {
        REQUIRE(std::string(e.what()) == "Out of range");
    }

    try {
        throw bygg::out_of_range{"Out of range, very very out of range"};
    } catch (const bygg::out_of_range& e) {
        REQUIRE(std::string(e.what()) == "Out of range, very very out of range");
    }

    try {
        throw bygg::invalid_argument{};
    } catch (const bygg::invalid_argument& e) {
        REQUIRE(std::string(e.what()) == "Invalid argument");
    }

    try {
        throw bygg::invalid_argument{"Invalid, very very invalid argument"};
    } catch (const bygg::invalid_argument& e) {
        REQUIRE(std::string(e.what()) == "Invalid, very very invalid argument");
    }
}

void General::test_npos_values() {
    REQUIRE(bygg::HTML::Property::npos == -1);
    REQUIRE(bygg::HTML::Properties::npos == -1);
    REQUIRE(bygg::HTML::Element::npos == -1);
    REQUIRE(bygg::HTML::Section::npos == -1);
    REQUIRE(bygg::HTML::Document::npos == -1);
    REQUIRE(bygg::CSS::Property::npos == -1);
    REQUIRE(bygg::CSS::Element::npos == -1);
    REQUIRE(bygg::CSS::Stylesheet::npos == -1);
}

void General::test_version() {
    std::tuple<int, int, int> version = bygg::version();

    REQUIRE(std::get<0>(version) >= 0);
    REQUIRE(std::get<1>(version) >= 0);
    REQUIRE(std::get<2>(version) >= 0);
}