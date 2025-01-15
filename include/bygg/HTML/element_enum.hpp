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
        Erase_Newlines = 1 << 0, /* Erase newlines */
        Erase_Tabs = 1 << 1, /* Erase tabs */
        Erase_Spaces = 1 << 2, /* Erase spaces */
        Erase_Multi_Spaces = 1 << 3, /* Erase spaces if there are multiple in a row */
        Erase_Left_Brackets = 1 << 4, /* Erase left brackets */
        Erase_Right_Brackets = 1 << 5, /* Erase right brackets */
        Erase_Single_Quotes = 1 << 6, /* Erase single quotes */
        Erase_Double_Quotes = 1 << 7, /* Erase double quotes */
        Replace_Newlines = 1 << 8, /* Replace newlines with <br/> */
        Replace_Tabs = 1 << 9, /* Replace tabs with &#9; */
        Replace_Spaces = 1 << 10, /* Replace spaces with &nbsp; */
        Replace_Left_Brackets = 1 << 11, /* Replace left brackets with &lt; */
        Replace_Right_Brackets = 1 << 12, /* Replace right brackets with &gt; */
        Replace_Single_Quotes = 1 << 13, /* Replace single quotes with &apos; */
        Replace_Double_Quotes = 1 << 14, /* Replace double quotes with &quot; */
        Erase_All = Erase_Newlines | Erase_Tabs | Erase_Spaces | Erase_Multi_Spaces | Erase_Left_Brackets | Erase_Right_Brackets | Erase_Single_Quotes | Erase_Double_Quotes, /* All erase flags */
        Erase_None = 0, /* No erase flags */
        Replace_All = Replace_Newlines | Replace_Tabs | Replace_Spaces | Replace_Left_Brackets | Replace_Right_Brackets | Replace_Single_Quotes | Replace_Double_Quotes, /* All replace flags */
        Replace_None = 0, /* No replace flags */
        Default = Erase_Multi_Spaces | Erase_Newlines | Erase_Tabs, /* Default flags */
    };

    inline ElementParameters operator|(ElementParameters lhs, ElementParameters rhs) {
        using T = std::underlying_type_t<ElementParameters>;
        return static_cast<ElementParameters>(static_cast<T>(lhs) | static_cast<T>(rhs));
    }

    inline bool operator&(ElementParameters lhs, ElementParameters rhs) {
        using T = std::underlying_type_t<ElementParameters>;
        return static_cast<T>(lhs) & static_cast<T>(rhs);
    }

    static constexpr ElementParameters _default_element_parameters = ElementParameters::Default;
} // namespace bygg::HTML