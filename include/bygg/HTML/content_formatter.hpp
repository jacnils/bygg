/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <bygg/types.hpp>

namespace bygg::HTML {
    /**
     * @brief Return a vector of tuples containing the find and replace strings for the content formatter. Mostly useless for the end user but not useless enough to be considered an implementation detail.
     * @return std::vector<std::tuple<bygg::string_type, bygg::string_type, bygg::string_type, bygg::string_type>>
     */
    [[nodiscard]] static std::vector<std::tuple<bygg::string_type, bygg::string_type, bygg::string_type, bygg::string_type>> get_find_replace_table() noexcept;

    /**
     * @brief Class used to format content with placeholders, similar to std::format() and the like.
     */
    class ContentFormatter {
        string_type input_str{};
        std::vector<string_type> lines{};
    public:
        /**
         * @brief Construct a new ContentFormatter object
         * @param input_str The string to format. 
         */
        explicit ContentFormatter(string_type& input_str) : input_str(std::move(input_str)) {}
        /**
         * @brief Construct a new ContentFormatter object
         * @param str The string to format.
         */
        explicit ContentFormatter(const char* str) : input_str(str) {};

        /**
         * @brief Construct a new ContentFormatter object
         * @param input_str The string to format.
         * @param args The arguments to format the string with.
         */
        template <typename... Args> explicit ContentFormatter(string_type& input_str, Args... args) : input_str(input_str) {
            this->lines = {args...};
        }
        /**
         * @brief Construct a new ContentFormatter object
         * @param str The string to format.
         * @param args The arguments to format the string with.
         */
        template <typename... Args> explicit ContentFormatter(const char* str, Args... args) : input_str(str) {
            this->lines = {args...};
        }

        /**
         * @brief Get the formatted string.
         * @return string_type The formatted string.
         */
        [[nodiscard]] string_type get() const;

        /**
         * @brief Get the formatted string.
         * @return T The formatted string.
         */
        template <typename T> T get() const {
            return T(this->get());
        }
        operator string_type() const { // NOLINT(google-explicit-constructor)
            return this->get();
        }
    };
} // namespace bygg::HTML