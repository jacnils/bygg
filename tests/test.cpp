/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>
#include <bygg/bygg.hpp>
#include <catch2/catch_test_macros.hpp>
#include "test.hpp"

// NOLINTBEGIN
// Disable linting. This is a test program and the code is intentionally bad in some places.
// As such, I think it's best to turn linting off rather than to try and resolve any such warnings.

/**
 * @brief Test cases for the bygg namespace.
 */
SCENARIO("Test general", "[GENERAL]") {
    General::test_exceptions();
    General::test_npos_values();
    General::test_version();
}

/**
 * @brief Test cases for the bygg namespace.
 */
SCENARIO("Test HTML", "[HTML]") {
    HTML::test_tag();
    HTML::test_property();
    HTML::test_properties();
    HTML::test_element();
    HTML::test_section();
    HTML::test_document();
    HTML::test_pseudocode_generator();
}

/**
 * @brief Test cases for the bygg namespace.
 */
SCENARIO("Test CSS", "[CSS]") {
    CSS::test_property();
    CSS::test_element();
    CSS::test_stylesheet();
    CSS::test_color_conversions();
    CSS::test_color_formatter();
    CSS::test_function();
}

#ifdef BYGG_USE_LIBXML2
/**
 * @brief Test cases for the bygg namespace.
 */
SCENARIO("Test HTML parser", "[libxml2]") {
    Parser::test_html_parser();
}
#endif

// NOLINTEND