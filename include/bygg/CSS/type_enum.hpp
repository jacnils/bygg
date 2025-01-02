/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

namespace bygg::CSS {
    /**
     * @brief Enum for formatting options.
     */
    enum class Type {
        Selector, /* element { } */
        Class, /* .element { } */
        Id, /* #element { } */
        Rule, /* @rule { }; */
    };
} // namespace bygg::CSS
