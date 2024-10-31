/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <bygg/types.hpp>

namespace bygg::CSS {
    /**
     * @brief A class to represent a CSS property
     */
    class Property {
        private:
            std::pair<string_type, string_type> property{};
        protected:
        public:
            /**
             * @brief The npos value
             */
            static constexpr size_type npos = -1;

            /**
             * @brief Construct a new Property object
             * @param key The key of the property
             * @param value The value of the property
             */
            Property(const string_type& key, const string_type& value) : property(std::make_pair(key, value)) {};
            /**
             * @brief Construct a new Property object
             */
            Property(const Property& property) = default;
            /**
             * @brief Construct a new Property object
             */
            Property() = default;
            /**
             * @brief Destroy the Property object
             */
            ~Property() = default;

            /**
             * @brief Get the key of the property
             * @return string_type The key of the property
             */
            [[nodiscard]] string_type get_key() const;
            /**
             * @brief Get the key of the property in a specific type
             * @return T The key of the property
             */
            template <typename T> T get_key() const {
                if (std::is_same_v<T, string_type>) {
                    return this->property.first;
                }
                return T(this->property.first);
            }
            /**
             * @brief Get the value of the property
             * @return string_type The value of the property
             */
            [[nodiscard]] string_type get_value() const;
            /**
             * @brief Get the value of the property in a specific type
             * @return T The value of the property
             */
            template <typename T> T get_value() const {
                if (std::is_same_v<T, string_type>) {
                    return this->property.second;
                }
                return T(this->property.second);
            }
            /**
             * @brief Get the property.
             * @return std::pair<string_type, string_type> The value of the property
             */
            [[nodiscard]] std::pair<string_type, string_type> get_pair() const;
            /**
             * @brief Get the property in a specific type.
             * @return std::pair<T, T> The value of the property
             */
            template <typename T> std::pair<T, T> get_pair() const {
                if (std::is_same_v<T, string_type>) {
                    return std::make_pair(this->property.first, this->property.second);
                }
                return std::pair<T, T>(this->property.first, this->property.second);
            }
            /**
             * @brief Get the property in a formatted form.
             * @return string_type The formatted CSS property.
             */
            [[nodiscard]] string_type get() const;
            /**
             * @brief Get the property in a specific type.
             * @return T The formatted CSS property.
             */
            template <typename T> T get() const {
                return T(get());
            }
            /**
             * @brief Set the key of the property.
             * @param key The key.
             */
            void set_key(const string_type& key);
            /**
             * @brief Set the value of the property.
             * @param value The value.
             */
            void set_value(const string_type& value);
            /**
             * @brief Set the property
             * @param key The key of the property
             * @param value The value of the property
             */
            void set(const string_type& key, const string_type& value);

            Property& operator=(const Property& property);
            bool operator==(const Property& property) const;
            bool operator!=(const Property& property) const;
    };
} // namespace bygg
