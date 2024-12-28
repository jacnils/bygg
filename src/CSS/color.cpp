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
            throw bygg::invalid_argument{"Invalid formatting type"};
        }
    }
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

    if (str.empty()) {
        throw bygg::invalid_argument{"Empty input string is not allowed"};
    }

    string_type hex = str;
    if (str.at(0) == '#') {
        hex = str.substr(1);
    }

    bygg::string_type hr{};
    bygg::string_type hg{};
    bygg::string_type hb{};

    if (hex.length() == 3) {
        hr = hex.substr(0, 1);
        hg = hex.substr(1, 1);
        hb = hex.substr(2, 1);

        hr += hr;
        hg += hg;
        hb += hb;
    } else if (hex.length() == 6 || hex.length() == 8) {
        hr = hex.substr(0, 2);
        hg = hex.substr(2, 2);
        hb = hex.substr(4, 2);
    } else {
        throw bygg::invalid_argument{"Invalid hex color"};
    }

    bygg::CSS::ColorStruct color;

    color.r = convert_to_int(hr) / 255.0;
    color.g = convert_to_int(hg) / 255.0;
    color.b = convert_to_int(hb) / 255.0;
    color.a = 1;

    if (hex.length() == 8) {
        std::string ha{hex.substr(6, 2)};
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

bygg::CSS::ColorStruct bygg::CSS::from_rgba(int r, int g, int b, int a = 255) {
    if (r < 0 || g < 0 || b < 0 || a < 0 || r > 255 || g > 255 || b > 255 || a > 255) {
        throw bygg::invalid_argument{"Invalid hex color"};
    }

    return {r / 255.0, g / 255.0, b / 255.0, a / 255.0};
};

bygg::CSS::ColorStruct bygg::CSS::from_float(float r, float g, float b, float a = 1) {
    if (r < 0.0 || r > 1.0 || g < 0.0 || g > 1.0 || b < 0.0 || b > 1.0) {
        throw bygg::invalid_argument{"Invalid color r/g/b"};
    }

    return {r,g,b,a};
}

bygg::CSS::ColorStruct bygg::CSS::from_double(double r, double g, double b, double a = 1) {
    if (r < 0.0 || r > 1.0 || g < 0.0 || g > 1.0 || b < 0.0 || b > 1.0) {
        throw bygg::invalid_argument{"Invalid color r/g/b"};
    }

    return {r,g,b,a};
}

bygg::CSS::ColorStruct bygg::CSS::from_hsla(int h, int s, int l, int a = 100) {
    if (h < 0 || h > 360 || s < 0 || s > 100 || l < 0 || l > 100 || a < 0 || a > 100) {
        throw bygg::invalid_argument{"Invalid HSL color"};
    }

    float r{}, g{}, b{};
    float H = static_cast<float>(h) / 360.0f;
    float S = static_cast<float>(s) / 100.0f;
    float L = static_cast<float>(l) / 100.0f;
    float A = static_cast<float>(a) / 100.0f;

    if (S == 0) {
        r = g = b = L;
    } else {
        const auto hue2rgb = [](float p, float q, float t) {
            if (t < 0) t += 1;
            if (t > 1) t -= 1;
            if (t < 1.0f / 6.0f) return p + (q - p) * 6 * t;
            if (t < 1.0f / 2.0f) return q;
            if (t < 2.0f / 3.0f) return p + (q - p) * (2.0f / 3.0f - t) * 6;

            return p;
        };

        float q = L < 0.5f ? L * (1 + S) : L + S - L * S;
        float p = 2 * L - q;

        r = hue2rgb(p, q, H + 1.0f / 3.0f);
        g = hue2rgb(p, q, H);
        b = hue2rgb(p, q, H - 1.0f / 3.0f);
    }

    r = std::max(0.0f, std::min(1.0f, r));
    g = std::max(0.0f, std::min(1.0f, g));
    b = std::max(0.0f, std::min(1.0f, b));

    return {r, g, b, A};
}

bygg::CSS::ColorStruct bygg::CSS::from_cmyka(int c, int m, int y, int k, int a = 100) {
    if (c < 0 || c > 100 || m < 0 || m > 100 || y < 0 || y > 100 || k < 0 || k > 100 || a < 0 || a > 100) {
        throw bygg::invalid_argument{"Invalid CMYKA color"};
    }

    float C = static_cast<float>(c) / 100.0f;
    float M = static_cast<float>(m) / 100.0f;
    float Y = static_cast<float>(y) / 100.0f;
    float K = static_cast<float>(k) / 100.0f;
    float A = static_cast<float>(a) / 100.0f;

    float R = 1.0f - std::min(1.0f, C * (1.0f - K) + K);
    float G = 1.0f - std::min(1.0f, M * (1.0f - K) + K);
    float B = 1.0f - std::min(1.0f, Y * (1.0f - K) + K);

    return {R, G, B, A};
}

bygg::CSS::ColorStruct bygg::CSS::from_hsva(int h, int s, int v, int a = 100) {
    if (h < 0 || h > 360 || s < 0 || s > 100 || v < 0 || v > 100 || a < 0 || a > 100) {
        throw bygg::invalid_argument{"Invalid HSVA color"};
    }

    float H = static_cast<float>(h) / 360.0f;
    float S = static_cast<float>(s) / 100.0f;
    float V = static_cast<float>(v) / 100.0f;
    float A = static_cast<float>(a) / 100.0f;

    float r = 0, g = 0, b = 0;

    int i = static_cast<int>(H * 6);
    float f = H * 6 - static_cast<float>(i);
    float p = V * (1 - S);
    float q = V * (1 - f * S);
    float t = V * (1 - (1 - f) * S);

    switch (i % 6) {
        case 0: r = V, g = t, b = p; break;
        case 1: r = q, g = V, b = p; break;
        case 2: r = p, g = V, b = t; break;
        case 3: r = p, g = q, b = V; break;
        case 4: r = t, g = p, b = V; break;
        case 5: r = V, g = p, b = q; break;
        default: break;
    }

    return {r, g, b, A};
}