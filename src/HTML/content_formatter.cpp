/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <bygg/types.hpp>
#include <bygg/HTML/content_formatter.hpp>
#include <functional>

static std::vector<std::tuple<bygg::string_type, bygg::string_type, bygg::string_type, bygg::string_type>> bygg::HTML::get_find_replace_table() noexcept {
    return {
        {"@STRONG(", ")", "<strong>", "</strong>"},
        {"@ITALIC(", ")", "<em>", "</em>"},
        {"@UNDERLINE(", ")", "<u>", "</u>"},
        {"@STRIKE(", ")", "<s>", "</s>"},
        {"@CODE(", ")", "<code>", "</code>"},
        {"@MARK(", ")", "<mark>", "</mark>"},
        {"@SMALL(", ")", "<small>", "</small>"},
        {"@BIG(", ")", "<big>", "</big>"},
        {"@SUB(", ")", "<sub>", "</sub>"},
        {"@SUP(", ")", "<sup>", "</sup>"},
        {"@H1(", ")", "<h1>", "</h1>"},
        {"@H2(", ")", "<h2>", "</h2>"},
        {"@H3(", ")", "<h3>", "</h3>"},
        {"@H4(", ")", "<h4>", "</h4>"},
        {"@H5(", ")", "<h5>", "</h5>"},
        {"@H6(", ")", "<h6>", "</h6>"},
        {"@P(", ")", "<p>", "</p>"},
        {"@DIV(", ")", "<div>", "</div>"},
        {"@SPAN(", ")", "<span>", "</span>"},
        {"@A(", ")", "<a>", "</a>"},
        {"@IMG(", ")", "<img>", "</img>"},
        {"@BR(", ")", "<br>", ""},
        {"@HR(", ")", "<hr>", ""},
        {"@UL(", ")", "<ul>", "</ul>"},
        {"@OL(", ")", "<ol>", "</ol>"},
        {"@LI(", ")", "<li>", "</li>"},
        {"@DL(", ")", "<dl>", "</dl>"},
        {"@DT(", ")", "<dt>", "</dt>"},
        {"@DD(", ")", "<dd>", "</dd>"},
        {"@TABLE(", ")", "<table>", "</table>"},
        {"@TR(", ")", "<tr>", "</tr>"},
    };
}

bygg::string_type bygg::HTML::ContentFormatter::get() const {
    string_type return_string{this->input_str};

    if (return_string.find("%s") != string_type::npos) {
        for (const auto& line : this->lines) {
            size_t pos = return_string.find("%s");
            return_string.replace(pos, 2, line);
        }
    }

    // e.g. @STRONG(Hello world) -> <strong>Hello world</strong>
    const auto replace_wrapping = [&](const std::pair<string_type, string_type>& find, const std::pair<string_type, string_type>& replace) {
        size_type pos = 0;
        while ((pos = return_string.find(find.first, pos)) != string_type::npos) {
            return_string.replace(pos, find.first.length(), replace.first);
            pos += replace.first.length();
            pos = return_string.find(find.second, pos);
            return_string.replace(pos, find.second.length(), replace.second);
            pos += replace.second.length();
        }
    };

    const auto find_replace_table = get_find_replace_table();

    for (const auto& [find, find_end, replace, replace_end] : find_replace_table) {
        replace_wrapping({find, find_end}, {replace, replace_end});
    }

    return return_string;
}