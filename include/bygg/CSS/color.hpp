/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <bygg/types.hpp>
#include <bygg/CSS/color_type_enum.hpp>
#include <bygg/CSS/color_struct.hpp>

namespace bygg::CSS {
    class ColorFormatter {
        bygg::CSS::ColorFormatting formatting{bygg::CSS::ColorFormatting::Hex};
        bygg::CSS::ColorStruct color{};
    public:
        ColorFormatter() = default;
        explicit ColorFormatter(const bygg::CSS::ColorStruct& color, bygg::CSS::ColorFormatting formatting = bygg::CSS::ColorFormatting::Hex) : formatting(formatting), color(color) {};
        ~ColorFormatter() = default;

        /**
         * @brief Return a formatted string based on the constructor parameters.
         * @return bygg::string_type
         */
        [[nodiscard]] bygg::string_type get(bygg::CSS::ColorFormatting formatting) const;
        /**
         * @brief Return the object's ColorFormatting enum.
         * @return bygg::CSS::ColorFormatting
         */
        [[nodiscard]] bygg::CSS::ColorFormatting get_formatting() const;
        /**
         * @brief Return the object's ColorStruct struct.
         * @return bygg::CSS::ColorStruct
         */
        [[nodiscard]] bygg::CSS::ColorStruct get_color_struct() const;
        /**
         * @brief Set the formatting to the parameter specified.
         * @param formatting The formatting to use.
         */
        void set_formatting(const bygg::CSS::ColorFormatting& formatting);
        /**
         * @brief Set the color struct to the parameter specified.
         * @param color Color struct parameter.
         */
        void set_color_struct(const bygg::CSS::ColorStruct& color);
        /**
         * @brief Return a formatted string based on the constructor parameters.
         * @return bygg::string_type
         */
        template <typename T> [[nodiscard]] T get(const bygg::CSS::ColorFormatting formatting = bygg::CSS::ColorFormatting::Undefined) const {
            return {get(formatting)};
        }
    };

    bygg::CSS::ColorStruct from_hex(const bygg::string_type& str);
    bygg::CSS::ColorStruct from_rgba(int r, int g, int b, int a);
    bygg::CSS::ColorStruct from_float(float r, float g, float b, float a);
    bygg::CSS::ColorStruct from_double(double r, double g, double b, double a);
} // namespace bygg::CSS
