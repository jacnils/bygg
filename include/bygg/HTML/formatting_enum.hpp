/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

namespace bygg::HTML {
    /**
     * @brief Enum for formatting options.
     */
    enum class Formatting {
        None, /* No formatting. Output is in the form of one long string of text, and a single newline character. */
        Pretty, /* Pretty formatting. Output is formatted with newlines and tabs as deemed appropriate. */
        Newline, /* Newline formatting. Each element has a newline appended. */
    };
} // namespace bygg
