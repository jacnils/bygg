/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

namespace bygg::CSS {
    /**
    * @brief A class to represent a CSS function.
    * @note To get the function as a standard C++ string, no conversion is needed, as the class has an implicit conversion operator
    * @note Does not append a semicolon, under the assumption that the function is part of a CSS Property() element
    */
    class Function {
        string_type name{};
        std::vector<string_type> parameters{};
        mutable string_type cache{};
    public:
        using internal_type = std::vector<string_type>;
        using iterator = internal_type::iterator;
        using const_iterator = internal_type::const_iterator;
        using reverse_iterator = internal_type::reverse_iterator;
        using const_reverse_iterator = internal_type::const_reverse_iterator;
        static constexpr auto npos = -1;

        Function() = default;
        /**
         * @brief Construct a new Function object
         * @param name The name of the function
         * @param args The parameters of the function
         */
        template <typename... Args> explicit Function(string_type name, Args&&... args) : name(std::move(name)) {
            (this->push_back(std::forward<Args>(args)), ...);
        }
        /**
        * @brief Push a parameter to the function
        * @param parameter The parameter to push
        */
        void push_back(const string_type& parameter);
        /**
        * @brief Emplace a parameter to the function
        * @param parameter The parameter to emplace
        */
        void emplace_back(string_type&& parameter);
        /**
        * @brief Push a parameter to the function
        * @param parameter The parameter to push
        */
        void push_back(double parameter);
        /**
        * @brief Emplace a parameter to the function
        * @param parameter The parameter to emplace
        */
        void emplace_back(double parameter);
        /**
        * @brief Push a parameter to the function
        * @param parameter The parameter to push
        */
        void push_back(int parameter);
        /**
        * @brief Emplace a parameter to the function
        * @param parameter The parameter to emplace
        */
        void emplace_back(int parameter);
        /**
        * @brief Pop the last parameter from the function
        */
        void pop_back();
        /**
        * @brief Get a parameter at a specific index
        * @param index The index of the parameter
        * @return string_type The parameter at the index
        */
        [[nodiscard]] string_type at(size_type index) const;
        /**
        * @brief Get a parameter at a specific index
        * @param index The index of the parameter
        * @return string_type The parameter at the index
        */
        [[nodiscard]] string_type& at(size_type index);
        /**
        * @brief Find a parameter in the function
        * @param parameter The parameter to find
        * @param start The index to start searching from
        * @return size_type The index of the parameter
        */
        [[nodiscard]] size_type find(const string_type& parameter, size_type start = 0) const;
        /**
        * @brief Find a parameter at a specific index
        * @param parameter The index of the parameter
        * @param start The index to start searching from
        * @return size_type The index of the parameter
        */
        [[nodiscard]] size_type find(const string_type& parameter, size_type start = 0);
        /**
        * @brief Get the last parameter
        * @return string_type The last parameter
        */
        [[nodiscard]] string_type& back();
        /**
        * @brief Get the last parameter
        * @return string_type The last parameter
        */
        [[nodiscard]] string_type back() const;
        /**
        * @brief Get the first parameter
        * @return string_type The first parameter
        */
        [[nodiscard]] string_type front() const;
        /**
        * @brief Get the first parameter
        * @return string_type The first parameter
        */
        [[nodiscard]] string_type& front();
        /**
        * @brief Get the size of the function
        * @note May be used to get the number of parameters.
        * @return size_type The size of the function
        */
        [[nodiscard]] size_type size() const;
        /**
        * @brief Check if the function is empty
        * @note May be used to check if the function has any parameters.
        * @return bool True if the function is empty, false otherwise
        */
        [[nodiscard]] bool empty() const;
        /**
        * @brief Get the name of the function
        * @return string_type The name of the function
        */
        [[nodiscard]] string_type get_name() const;
        /**
        * @brief Get the parameters of the function
        * @return T The parameters of the function
        */
        template <typename T> [[nodiscard]] T get_name() const {
            return T(name);
        }
        /**
        * @brief Get the parameters of the function
        * @return std::vector<string_type> The parameters of the function
        */
        [[nodiscard]] std::vector<string_type> get_parameters() const;
        /**
        * @brief Get the parameters of the function
        * @return std::vector<T> The parameters of the function
        */
        template <typename T> [[nodiscard]] std::vector<T> get_parameters() const {
            return std::vector<T>(parameters.begin(), parameters.end());
        }
        /**
        * @brief Get the function as a formatted string
        * @note Does not append a semicolon, under the assumption that the function is part of a CSS Property() element
        * @return string_type The function as a formatted string
        */
        [[nodiscard]] string_type get() const;
        /**
        * @brief Get the function as a formatted string
        * @note Does not append a semicolon, under the assumption that the function is part of a CSS Property() element
        * @return T The function as a formatted string
        */
        template <typename T> [[nodiscard]] T get() const {
            return T(get());
        }
        /**
        * @brief Get the function as a C-style string
        * @note Valid until the Function object goes out of scope
        * @return const char* The function as a C-style string
        */
        const char* c_str() const; // NOLINT
        /**
        * @brief Clear the function
        */
        void clear();
        /**
        * @brief Set the name of the function
        * @param name The name of the function
        */
        void set_name(const string_type& name);
        /**
        * @brief Set the parameters of the function
        * @note It is possible to convert the parameters to string_type using e.g. std::to_string
        * @param parameters The parameters of the function
        */
        void set_parameters(const std::vector<string_type>& parameters);
        /**
        * @brief Set the function name and parameters
        * @param name The name of the function
        * @param parameters The parameters of the function
        */
        void set(const string_type& name, const std::vector<string_type>& parameters);

        [[nodiscard]] string_type& operator[](size_type index);
        [[nodiscard]] string_type operator[](size_type index) const;
        bool operator==(const Function& other) const;
        bool operator==(const string_type& other) const;
        Function& operator=(const Function& other);
        operator string_type() const; // NOLINT
        void operator+=(const string_type& parameter);
        void operator+=(const Function& other);
        Function operator+(const string_type& parameter) const;

        [[nodiscard]] iterator begin();
        [[nodiscard]] iterator end();
        [[nodiscard]] const_iterator begin() const;
        [[nodiscard]] const_iterator end() const;
        [[nodiscard]] const_iterator cbegin() const;
        [[nodiscard]] const_iterator cend() const;
        [[nodiscard]] reverse_iterator rbegin();
        [[nodiscard]] reverse_iterator rend();
        [[nodiscard]] const_reverse_iterator crbegin() const;
        [[nodiscard]] const_reverse_iterator crend() const;
    };
} // namespace bygg::CSS
