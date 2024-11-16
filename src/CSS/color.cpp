/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <sstream>
#include <tuple>
#include <bygg/types.hpp>
#include <bygg/except.hpp>
#include <bygg/CSS/color_struct.hpp>
#include <bygg/CSS/color.hpp>
#include <bygg/CSS/impl/color_conversions.hpp>

bygg::string_type bygg::CSS::ColorFormatter::get(const bygg::CSS::ColorFormatting formatting = bygg::CSS::ColorFormatting::Undefined) const {
    switch (formatting == bygg::CSS::ColorFormatting::Undefined ? this->formatting : formatting) {
        case bygg::CSS::ColorFormatting::Hex: {
            return bygg::impl_color_to_string(color);
        } case bygg::CSS::ColorFormatting::Hex_A: {
            return bygg::impl_color_to_string_a(color);
        } case bygg::CSS::ColorFormatting::Rgb: {
            std::tuple<int, int, int> tuple = bygg::impl_color_to_int(color);
            std::stringstream ss{};
            ss << "rgb(" << std::get<0>(tuple) << ", " << std::get<1>(tuple) << ", " << std::get<2>(tuple) << ")";
            return ss.str();
        } case bygg::CSS::ColorFormatting::Rgb_A: {
            std::tuple<int, int, int, int> tuple_ = bygg::impl_color_to_int_a(color);
            std::stringstream ss_{};
            ss_ << "rgba(" << std::get<0>(tuple_) << ", " << std::get<1>(tuple_) << ", " << std::get<2>(tuple_) << ", " << std::get<3>(tuple_) << ")";
            return ss_.str();
        } case bygg::CSS::ColorFormatting::Hsl: {
            std::tuple<int, int, int> tuple_ = bygg::impl_color_to_hsl(color);
            std::stringstream ss_{};
            ss_ << "hsl(" << std::get<0>(tuple_) << ", " << std::get<1>(tuple_) << "%, " << std::get<2>(tuple_) << "%)";
            return ss_.str();
        } case bygg::CSS::ColorFormatting::Hsl_A: {
            std::tuple<int, int, int, int> tuple_ = bygg::impl_color_to_hsl_a(color);
            std::stringstream ss_{};
            ss_ << "hsla(" << std::get<0>(tuple_) << ", " << std::get<1>(tuple_) << "%, " << std::get<2>(tuple_) << "%, " << std::get<3>(tuple_) / 100.0 << ")";
            return ss_.str();
        } default: {
            break;
        }
    }

    throw bygg::invalid_argument{"Invalid formatting"};
}

bygg::CSS::ColorStruct bygg::CSS::ColorFormatter::get_color_struct() const {
    return this->color;
}

bygg::CSS::ColorFormatting bygg::CSS::ColorFormatter::get_formatting() const {
    return this->formatting;
}

void bygg::CSS::ColorFormatter::set_color_struct(const bygg::CSS::ColorStruct& color) {
    this->color = color;
}

void bygg::CSS::ColorFormatter::set_formatting(const bygg::CSS::ColorFormatting& formatting) {
    this->formatting = formatting;
}

bygg::CSS::ColorStruct bygg::CSS::from_hex(const bygg::string_type& str) {
    const auto convert_to_int = [](const bygg::string_type& str) -> int {
        int value;
        std::stringstream ss;
        ss << std::hex << str;
        ss >> value;
        return value;
    };

    if (str.length() < 7) {
        throw bygg::invalid_argument{"Invalid hex color"};
    }

    bygg::string_type hr{str.substr(1, 2)};
    bygg::string_type hg{str.substr(3, 2)};
    bygg::string_type hb{str.substr(5, 2)};

    bygg::CSS::ColorStruct color;

    color.r = convert_to_int(hr) / 255.0;
    color.g = convert_to_int(hg) / 255.0;
    color.b = convert_to_int(hb) / 255.0;
    color.a = 1;

    if (str.length() == 9) {
        std::string ha{str.substr(7, 2)};
        color.a = convert_to_int(ha) / 255.0;
    }

    /* Whether invalid argument is the right exception to throw is debatable, but if this fails,
       then it's most probable that an invalid argument was passed to this function.
     */
    if (color.r < 0.0 || color.r > 1.0 || color.g < 0.0 ||
        color.g > 1.0 || color.b < 0.0 || color.b > 1.0 ||
        color.a < 0.0 || color.a > 1.0) {
        throw bygg::invalid_argument{"Invalid hex color"};
    }

    return color;
}

bygg::CSS::ColorStruct bygg::CSS::from_rgba(int r, int g, int b, int a) {
    if (r < 0 || g < 0 || b < 0 || a < 0 || r > 255 || g > 255 || b > 255 || a > 255) {
        throw bygg::invalid_argument{"Invalid hex color"};
    }

    return {r / 255.0, g / 255.0, b / 255.0, a / 255.0};
};

bygg::CSS::ColorStruct bygg::CSS::from_float(float r, float g, float b, float a) {
    if (r < 0.0 || r > 1.0 || g < 0.0 || g > 1.0 || b < 0.0 || b > 1.0) {
        throw bygg::invalid_argument{"Invalid color r/g/b"};
    }

    return {r,g,b,a};
}

bygg::CSS::ColorStruct bygg::CSS::from_double(double r, double g, double b, double a) {
    if (r < 0.0 || r > 1.0 || g < 0.0 || g > 1.0 || b < 0.0 || b > 1.0) {
        throw bygg::invalid_argument{"Invalid color r/g/b"};
    }

    return {r,g,b,a};
}