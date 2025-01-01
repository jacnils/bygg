/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <tuple>
#include <bygg/types.hpp>
#include <bygg/CSS/color_struct.hpp>

namespace bygg {
    /**
     * @brief Convert a bygg::CSS::ColorStruct to an #RRGGBB formatted bygg::string_type
     * @param color The color to parse from.
     * @return bygg::string_type
     */
    bygg::string_type impl_color_to_string(const bygg::CSS::ColorStruct& color);
    /**
     * @brief Convert a bygg::CSS::ColorStruct to an #RRGGBBAA formatted bygg::string_type
     * @param color The color to parse from.
     * @return bygg::string_type
     */
    bygg::string_type impl_color_to_string_a(const bygg::CSS::ColorStruct& color);
    /**
     * @brief Convert a bygg::CSS::ColorStruct to an std::tuple<int, int, int>
     * @param color The color to parse from.
     * @return std::tuple<int, int, int>
     */
    std::tuple<int, int, int> impl_color_to_int(const bygg::CSS::ColorStruct& color);
    /**
     * @brief Convert a bygg::CSS::ColorStruct to an std::tuple<int, int, int, int>
     * @param color The color to parse from.
     * @return std::tuple<int, int, int, int>
     */
    std::tuple<int, int, int, int> impl_color_to_int_a(const bygg::CSS::ColorStruct& color);
    /**
     * @brief Convert a bygg::CSS::ColorStruct to an std::tuple<int, int, int>
     * @param color The color to parse from.
     * @return std::tuple<int, int, int>
     */
    std::tuple<int, int, int> impl_color_to_hsl(const bygg::CSS::ColorStruct& color);
    /**
     * @brief Convert a bygg::CSS::ColorStruct to an std::tuple<int, int, int, int>
     * @param color The color to parse from.
     * @return std::tuple<int, int, int, int>
     */
    std::tuple<int, int, int, int> impl_color_to_hsl_a(const bygg::CSS::ColorStruct& color);
} // namespace bygg
