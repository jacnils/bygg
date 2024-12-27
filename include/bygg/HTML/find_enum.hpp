/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

namespace bygg::HTML {
    /**
     * @brief Enum for find() method.
     */
    enum class FindParameters {
        Only_Elements = 1 << 0,
        Only_Sections = 1 << 1,
        Search_Tag = 1 << 2,
        Search_Data = 1 << 3,
        Search_Properties = 1 << 4,
        Search_Deserialized = 1 << 5,
        Exact = 1 << 6,
    };

    inline FindParameters operator|(FindParameters lhs, FindParameters rhs) {
        using T = std::underlying_type_t<FindParameters>;
        return static_cast<FindParameters>(static_cast<T>(lhs) | static_cast<T>(rhs));
    }

    inline bool operator&(FindParameters lhs, FindParameters rhs) {
        using T = std::underlying_type_t<FindParameters>;
        return static_cast<T>(lhs) & static_cast<T>(rhs);
    }
} // namespace bygg::HTML