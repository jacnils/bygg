/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <sstream>
#include <iomanip>
#include <tuple>
#include <bygg/types.hpp>
#include <bygg/CSS/color_struct.hpp>
#include <bygg/CSS/impl/color_conversions.hpp>

bygg::string_type bygg::impl_color_to_string(const bygg::CSS::ColorStruct& color) {
   std::stringstream ss{};
   ss << "#" << std::hex << std::setfill('0') << std::setw(2)
   << static_cast<int>(color.r * 255) << std::setfill('0') << std::setw(2)
   << static_cast<int>(color.g * 255) << std::setfill('0') << std::setw(2)
   << static_cast<int>(color.b * 255);
   return ss.str();
}

bygg::string_type bygg::impl_color_to_string_a(const bygg::CSS::ColorStruct& color) {
 std::stringstream ss{};
 ss << "#" << std::hex << std::setfill('0') << std::setw(2)
 << static_cast<int>(color.r * 255) << std::setfill('0') << std::setw(2)
 << static_cast<int>(color.g * 255) << std::setfill('0') << std::setw(2)
 << static_cast<int>(color.b * 255) << std::setfill('0') << std::setw(2)
 << static_cast<int>(color.a * 255);
 return ss.str();
}

std::tuple<int, int, int> bygg::impl_color_to_int(const bygg::CSS::ColorStruct& color) {
    return std::make_tuple(static_cast<int>(color.r * 255), static_cast<int>(color.g * 255), static_cast<int>(color.b * 255));
}

std::tuple<int, int, int, int> bygg::impl_color_to_int_a(const bygg::CSS::ColorStruct& color) {
    return std::make_tuple(static_cast<int>(color.r * 255), static_cast<int>(color.g * 255), static_cast<int>(color.b * 255), static_cast<int>(color.a * 255));
}
