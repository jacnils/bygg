/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
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

std::tuple<int, int, int> bygg::impl_color_to_hsl(const bygg::CSS::ColorStruct& color) {
    double r = color.r;
    double g = color.g;
    double b = color.b;

    double max = std::max(r, std::max(g, b));
    double min = std::min(r, std::min(g, b));

    double h, s, l = (max + min) / 2;

    if (max == min) {
        h = s = 0;
    } else {
        double d = max - min;
        s = l > 0.5 ? d / (2 - max - min) : d / (max + min);

        if (max == r) {
            h = (g - b) / d + (g < b ? 6 : 0);
        } else if (max == g) {
            h = (b - r) / d + 2;
        } else {
            h = (r - g) / d + 4;
        }

        h /= 6;
    }

    return std::make_tuple(static_cast<int>(h * 360), static_cast<int>(s * 100), static_cast<int>(l * 100));
}

std::tuple<int, int, int, int> bygg::impl_color_to_hsl_a(const bygg::CSS::ColorStruct& color) {
    double r = color.r;
    double g = color.g;
    double b = color.b;

    double max = std::max(r, std::max(g, b));
    double min = std::min(r, std::min(g, b));

    double h, s, l = (max + min) / 2;

    if (max == min) {
        h = s = 0;
    } else {
        double d = max - min;
        s = l > 0.5 ? d / (2 - max - min) : d / (max + min);

        if (max == r) {
            h = (g - b) / d + (g < b ? 6 : 0);
        } else if (max == g) {
            h = (b - r) / d + 2;
        } else {
            h = (r - g) / d + 4;
        }

        h /= 6;
    }

    return std::make_tuple(static_cast<int>(h * 360), static_cast<int>(s * 100), static_cast<int>(l * 100), static_cast<int>(color.a * 100));
}
