/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

namespace bygg::HTML {
    /**
     * @brief Enum for element types.
     */
    enum class Type {
        Self_Closing, /* Self-closing element (<tag ... /> - Primarily useful for XHTML or HTML written by perfectionists) */
        Non_Self_Closing, /* Non-self-closing element (<tag></tag>) */
        Non_Closed, /* Non-closed element (<tag> - Invalid in XHTML) */
        Non_Opened, /* Non-opened element (</tag>) */
        Text_No_Formatting, /* Text element with no formatting (my text here). */
        Text, /* Text element with tab characters appropriately prepended (my text here). Note that this does *not* append a newline character. */
    };
} // namespace bygg::HTML