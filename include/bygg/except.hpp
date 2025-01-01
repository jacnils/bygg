/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <bygg/types.hpp>

namespace bygg {
    /**
     * @brief A class to represent an exception when an index is out of range
     */
    class out_of_range : public exception_type {
        private:
            const char* message{"Out of range"};
        public:
            [[nodiscard]] const char* what() const noexcept override {
                return message;
            }
            out_of_range() = default;
            explicit out_of_range(const char* message) : message(message) {};
    };

    /**
     * @brief A class to represent an exception when an argument is invalid
     */
    class invalid_argument : public exception_type {
        private:
            const char* message{"Invalid argument"};
        public:
            [[nodiscard]] const char* what() const noexcept override {
                return message;
            }
            invalid_argument() = default;
            explicit invalid_argument(const char* message) : message(message) {};
    };
} // namespace bygg
