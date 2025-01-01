/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <bygg/types.hpp>
#include <bygg/CSS/color_type_enum.hpp>
#include <bygg/CSS/color_struct.hpp>

namespace bygg::CSS {
    /**
     * @brief A class to format colors.
     */
    class ColorFormatter {
        bygg::CSS::ColorFormatting formatting{bygg::CSS::ColorFormatting::Hex};
        bygg::CSS::ColorStruct color{};
    public:
        ColorFormatter() = default;
        ~ColorFormatter() = default;

        /**
         * @brief Construct a new ColorFormatter object
         * @param color The color to format
         * @param formatting The formatting to use
         */
        explicit ColorFormatter(const bygg::CSS::ColorStruct& color, bygg::CSS::ColorFormatting formatting = bygg::CSS::ColorFormatting::Hex) : formatting(formatting), color(color) {};

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
        #ifndef BYGG_ALLOW_IMPLICIT_CONVERSIONS
        explicit
        #endif
        operator string_type() const { // NOLINT(google-explicit-constructor)
            return this->get(ColorFormatting::Undefined);
        }
    };

    /**     *
     * @brief Convert a hex string to a color struct.
     * @param str The hex string to convert.
     * @return bygg::CSS::ColorStruct
     */
    bygg::CSS::ColorStruct from_hex(const bygg::string_type& str);
    /**
     * @brief Convert an RGBA color to a color struct.
     * @param r Red value.
     * @param g Green value.
     * @param b Blue value.
     * @param a Alpha value.
     * @return bygg::CSS::ColorStruct
     */
    bygg::CSS::ColorStruct from_rgba(int r, int g, int b, int a);
    /**
     * @brief Convert an RGBA color to a color struct.
     * @param r Red value.
     * @param g Green value.
     * @param b Blue value.
     * @param a Alpha value.
     * @return bygg::CSS::ColorStruct
     */
    bygg::CSS::ColorStruct from_float(float r, float g, float b, float a);
    /**
     * @brief Convert an RGBA color to a color struct.
     * @param r Red value.
     * @param g Green value.
     * @param b Blue value.
     * @param a Alpha value.
     * @return bygg::CSS::ColorStruct
     */
    bygg::CSS::ColorStruct from_double(double r, double g, double b, double a);
    /**
     * @brief Convert an HSL color to a color struct.
     * @param h Hue value.
     * @param s Saturation value.
     * @param l Lightness value.
     * @param a Alpha value.
     * @return bygg::CSS::ColorStruct
     */
    bygg::CSS::ColorStruct from_hsla(int h, int s, int l, int a);
    /**
     * @brief Convert a CMYK color to a color struct.
     * @param c Cyan value.
     * @param m Magenta value.
     * @param y Yellow value.
     * @param k Black/key value.
     * @param a Alpha value.
     * @return bygg::CSS::ColorStruct
     */
    bygg::CSS::ColorStruct from_cmyka(int c, int m, int y, int k, int a);
    /**
     * @param h Hue value.
     * @param s Saturation value.
     * @param v Value value.
     * @param a Alpha value.
     * @return bygg::CSS::ColorStruct
     */
    bygg::CSS::ColorStruct from_hsva(int h, int s, int v, int a);
} // namespace bygg::CSS
