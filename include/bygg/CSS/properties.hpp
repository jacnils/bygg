/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <vector>
#include <bygg/types.hpp>
#include <bygg/CSS/property.hpp>
#include <bygg/CSS/formatting_enum.hpp>

namespace bygg::CSS {
   /**
    * @brief Container for Property, akin to std::vector<Property>
    */
    using PropertyList = std::vector<Property>;

    /**
     * @brief A class to represent the properties of an CSS element
     */
    class Properties {
            PropertyList properties{};
        public:
            using PropertyList = PropertyList;
            using iterator = PropertyList::iterator;
            using const_iterator = PropertyList::const_iterator;
            using reverse_iterator = PropertyList::reverse_iterator;
            using const_reverse_iterator = PropertyList::const_reverse_iterator;

            /**
             * @brief Return an iterator to the beginning.
             * @return iterator The iterator to the beginning.
             */
            iterator begin() { return properties.begin(); }
            /**
             * @brief Return an iterator to the end.
             * @return iterator The iterator to the end.
             */
            iterator end() { return properties.end(); }
            /**
             * @brief Return an iterator to the beginning.
             * @return iterator The iterator to the beginning.
             */
            [[nodiscard]] const_iterator begin() const { return properties.begin(); }
            /**
             * @brief Return an iterator to the end.
             * @return iterator The iterator to the end.
             */
            [[nodiscard]] const_iterator end() const { return properties.end(); }
            /**
             * @brief Return a const iterator to the beginning.
             * @return const_iterator The const iterator to the beginning.
             */
            [[nodiscard]] const_iterator cbegin() const { return properties.cbegin(); }
            /**
             * @brief Return a const iterator to the end.
             * @return const_iterator The const iterator to the end.
             */
            [[nodiscard]] const_iterator cend() const { return properties.cend(); }
            /**
             * @brief Return a reverse iterator to the beginning.
             * @return reverse_iterator The reverse iterator to the beginning.
             */
            reverse_iterator rbegin() { return properties.rbegin(); }
            /**
             * @brief Return a reverse iterator to the end.
             * @return reverse_iterator The reverse iterator to the end.
             */
            reverse_iterator rend() { return properties.rend(); }
            /**
             * @brief Return a const reverse iterator to the beginning.
             * @return const_reverse_iterator The const reverse iterator to the beginning.
             */
            [[nodiscard]] const_reverse_iterator crbegin() const { return properties.crbegin(); }
            /**
             * @brief Return a const reverse iterator to the end.
             * @return const_reverse_iterator The const reverse iterator to the end.
             */
            [[nodiscard]] const_reverse_iterator crend() const { return properties.crend(); }

            /**
             * @brief The npos value
             */
            static constexpr size_type npos = -1;

            /**
             * @brief Get the properties of the element
             * @return PropertyList The properties of the element
             */
            [[nodiscard]] PropertyList get_properties() const;

            /**
             * @brief Get the properties of the element in string form
             * @param formatting The formatting of the properties
             * @param tabc The number of tabs to use for formatting
             */
            [[nodiscard]] string_type get(Formatting formatting = Formatting::None, integer_type tabc = 0) const;

            /**
             * @brief Get the properties of the element in a specific type
             * @param formatting The formatting of the properties
             * @param tabc The number of tabs to use for formatting
             */
            template <typename T> T get(Formatting formatting = Formatting::None, integer_type tabc = 0) const {
                if (std::is_same_v<T, string_type>) {
                    return this->get(formatting, tabc);
                }
                return T(this->get(formatting, tabc));
            }
            /**
             * @brief Set the properties of the element
             * @param properties The properties to set
             */
            void set(const PropertyList& properties);
            /**
             * @brief Get the property at an index
             * @param index The index of the property
             * @return Property The property at the index
             */
            [[nodiscard]] Property& at(size_type index);
            /**
             * @brief Get the property at an index
             * @param index The index of the property
             * @return Property The property at the index
             */
            [[nodiscard]] Property at(size_type index) const;
            /**
             * @brief Insert a property into the element
             * @param index The index to insert the property
             * @param property The property to insert
             */
            void insert(size_type index, const Property& property);
            /**
             * @brief Erase a property from the element
             * @param index The index of the property to erase
             */
            void erase(size_type index);
            /**
             * @brief Find a property in the element
             * @param property The property to find
             * @return size_type The index of the property
             */
            [[nodiscard]] size_type find(const bygg::CSS::Property& property) const;
            /**
             * @brief Find a property in the element
             * @param str The property to find
             * @return size_type The index of the property
             */
            [[nodiscard]] size_type find(const string_type& str) const;
            /**
             * @brief Swap two properties in the element
             * @param index1 The index of the first property
             * @param index2 The index of the second property
             */
            void swap(size_type index1, size_type index2);
            /**
             * @brief Swap two properties in the element
             * @param property1 The first property
             * @param property2 The second property
             */
            void swap(const bygg::CSS::Property& property1, const bygg::CSS::Property& property2);
            /**
             * @brief Get the first property of the element
             * @return Property The first property of the element
             */
            [[nodiscard]] Property& front();
            /**
             * @brief Get the last property of the element
             * @return Property The last property of the element
             */
            [[nodiscard]] Property& back();
            /**
             * @brief Get the first property of the element
             * @return Property The first property of the element
             */
            [[nodiscard]] Property front() const;
            /**
             * @brief Get the last property of the element
             * @return Property The last property of the element
             */
            [[nodiscard]] Property back() const;
            /**
             * @brief Get the size of the element
             * @return size_type The size of the element
             */
            [[nodiscard]] size_type size() const;
            /**
             * @brief Clear the properties
             */
            void clear();
            /**
             * @brief Check if the properties are empty
             * @return bool True if the properties are empty, false otherwise
             */
            [[nodiscard]] bool empty() const;
            /**
             * @brief Prepend a property to the element
             * @param property The property to add
             */
            void push_front(const Property& property);
            /**
             * @brief Append a property to the element
             * @param property The property to add
             */
            void push_back(const Property& property);
            template <typename... Args> explicit Properties(Args... args) { (this->push_back(args), ...); }
            /**
             * @brief Construct a new Properties object
             * @param properties The properties to set
             */
            explicit Properties(PropertyList& properties) : properties(std::move(properties)) {};
            /**
             * @brief Construct a new Properties object
             * @param property The property to add
             */
            explicit Properties(const Property& property) : properties({property}) {};
            /**
             * @brief Construct a new Properties object
             * @param properties The properties to set
             */
            Properties(const Properties& properties) = default;
            /**
             * @brief Construct a new Properties object
             */
            Properties() = default;
            /**
             * @brief Destroy the Properties object
             */
            ~Properties() = default;
            Properties& operator=(const Properties& properties);
            Properties& operator=(const PropertyList& properties);
            Properties& operator=(const Property& property);
            bool operator==(const Properties& properties) const;
            bool operator==(const Property& property) const;
            bool operator!=(const Properties& properties) const;
            bool operator!=(const Property& property) const;
            Property& operator[](const size_type& index);
            Property operator[](const size_type& index) const;
            Properties& operator+=(const Property& property);
            Properties& operator+=(const Properties& properties);
    };

    /**
     * @brief Construct a new Properties object
     * @param args The properties to set
     */
    template <typename... Args> Properties make_properties(Args&&... args) { return Properties(std::forward<Args>(args)...); }
} // namespace bygg
