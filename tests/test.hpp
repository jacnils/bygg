/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

// NOLINTBEGIN
// Disable linting. This is a test program and the code is intentionally bad in some places.
// As such, I think it's best to turn linting off rather than to try and resolve any such warnings.

namespace HTML {
    void test_tag();
    void test_property();
    void test_properties();
    void test_element();
    void test_section();
    void test_document();
    void test_pseudocode_generator();
} // namespace HTML

namespace CSS {
    void test_property();
    void test_element();
    void test_stylesheet();
    void test_color_conversions();
    void test_color_formatter();
} // namespace CSS

inline namespace General {
    void test_exceptions();
    void test_npos_values();
    void test_version();
}

#ifdef BYGG_USE_LIBXML2
inline namespace Parser {
    void test_html_parser();
}
#endif