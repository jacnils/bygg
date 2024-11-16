/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

namespace bygg::CSS {
    /**
     * @brief Enum for color formatting options.
     */
    enum class ColorFormatting {
      Undefined,
      Hex,
      Hex_A,
      Rgb,
      Rgb_A,
      Hsl,
      Hsl_A,
    };
} // namespace bygg
