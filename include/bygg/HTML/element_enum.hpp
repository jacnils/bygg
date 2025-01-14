/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

namespace bygg::HTML {
    /**
     * @brief Enum for Element() class
     */
    enum class ElementParameters {
        Erase_Newlines = 1 << 0,
        Erase_Tabs = 1 << 1,
        Erase_Spaces = 1 << 2,
        Erase_Multi_Spaces = 1 << 3,
        Erase_All = Erase_Newlines | Erase_Tabs | Erase_Spaces | Erase_Multi_Spaces,
        Erase_Default = Erase_Newlines | Erase_Tabs | Erase_Multi_Spaces,
        Erase_None = ~Erase_All
    };

    inline ElementParameters operator|(ElementParameters lhs, ElementParameters rhs) {
        using T = std::underlying_type_t<ElementParameters>;
        return static_cast<ElementParameters>(static_cast<T>(lhs) | static_cast<T>(rhs));
    }

    inline bool operator&(ElementParameters lhs, ElementParameters rhs) {
        using T = std::underlying_type_t<ElementParameters>;
        return static_cast<T>(lhs) & static_cast<T>(rhs);
    }

    static constexpr ElementParameters _default_element_parameters = ElementParameters::Erase_Default;
} // namespace bygg::HTML