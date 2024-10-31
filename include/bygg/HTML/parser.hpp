/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <bygg/types.hpp>
#include <bygg/HTML/section.hpp>

namespace bygg::HTML::Parser {
    /**
     * @brief Options for the HTML parser
     */
    struct Options {
        bool consider_whitespace{true}; /* Utilize whitespace to determine depth. Results may vary. */
    };

    /**
     * @brief Parse an HTML string into a list of tags
     * @param html The HTML string to parse
     * @return TagList The list of tags
     */
    Section parse_html_string(const string_type& html, const Options& options = {});
} // namespace bygg::HTML::Parser