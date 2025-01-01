/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <bygg/HTML/type_enum.hpp>
#include <bygg/HTML/properties.hpp>
#include <bygg/types.hpp>

namespace bygg {
    struct TagData {
        string_type tag;
        HTML::Type type;
        std::string data;
        HTML::Properties properties;
        int depth{};
    };

    using TagList = std::vector<TagData>;

    TagList parse_html_string(const string_type& html);
} // namespace bygg
