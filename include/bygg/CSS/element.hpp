/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <bygg/types.hpp>
#include <bygg/HTML/tag.hpp>
#include <bygg/CSS/property.hpp>
#include <bygg/CSS/properties.hpp>
#include <bygg/CSS/formatting_enum.hpp>
#include <bygg/CSS/type_enum.hpp>

namespace bygg::CSS {
    using PseudoClass = string_type;

    /**
     * @brief A class to represent the properties of a CSS element
     */
    class Element {
            string_type selector{};
            mutable std::vector<std::variant<Properties, Element>> children{};
            mutable Properties properties{};
            mutable std::vector<Element> elements{};
            Type type{Type::Selector};
            PseudoClass pseudo{};

            void ensure_has_properties() const {
                for (const auto& it : children) {
                    if (std::holds_alternative<Properties>(it)) {
                        return;
                    }
                }

                children.emplace_back(Properties());
            }
        public:
            using variant_t = std::variant<Properties, Element>;
            using variant_list = std::vector<variant_t>;

            using iterator = Properties::iterator;
            using const_iterator = Properties::const_iterator;
            using reverse_iterator = Properties::reverse_iterator;
            using const_reverse_iterator = Properties::const_reverse_iterator;
            using variant_iterator = variant_list::iterator;
            using variant_const_iterator = variant_list::const_iterator;
            using variant_reverse_iterator = variant_list::reverse_iterator;
            using variant_const_reverse_iterator = variant_list::const_reverse_iterator;

            /**
             * @brief Return a variant_list of all elements and properties.
             * @note To use the result, you must use std::visit.
             */
            [[nodiscard]] variant_list& get_all() const {
                return children;
            }

            /**
             * @brief Get a variant_t at a specific index.
             * @param index The index to get the variant_t from.
             * @return variant_t The variant_t at the index.
             */
            [[nodiscard]] variant_t& get_any(size_type index) {
                for (size_type i{}; i < children.size(); ++i) {
                    if (i == index) {
                        return children.at(i);
                    }
                }

                throw out_of_range("Index out of range");
            }
            /**
             * @brief Return an iterator to the beginning.
             * @return iterator The iterator to the beginning.
             */
            iterator begin() {
                properties.clear();
                for (const auto& it : children) {
                    if (std::holds_alternative<Properties>(it)) {
                        properties.push_back(std::get<Properties>(it));
                    }
                }
                return properties.begin();
            }
            /**
             * @brief Return an iterator to the end.
             * @return iterator The iterator to the end.
             */
            iterator end() {
                properties.clear();
                for (const auto& it : children) {
                    if (std::holds_alternative<Properties>(it)) {
                        properties.push_back(std::get<Properties>(it));
                    }
                }
                return properties.end();
            }
            /**
             * @brief Return a const_iterator to the beginning.
             * @return const_iterator The const_iterator to the beginning.
             */
            [[nodiscard]] const_iterator begin() const {
                properties.clear();
                for (const auto& it : children) {
                    if (std::holds_alternative<Properties>(it)) {
                        properties.push_back(std::get<Properties>(it));
                    }
                }
                return properties.begin();
            }
            /**
             * @brief Return a const_iterator to the end.
             * @return const_iterator The const_iterator to the end.
             */
            [[nodiscard]] const_iterator end() const {
                properties.clear();
                for (const auto& it : children) {
                    if (std::holds_alternative<Properties>(it)) {
                        properties.push_back(std::get<Properties>(it));
                    }
                }
                return properties.end();
            }
            /**
             * @brief Return a const iterator to the beginning.
             * @return const_iterator The const iterator to the beginning.
             */
            [[nodiscard]] const_iterator cbegin() const {
                properties.clear();
                for (const auto& it : children) {
                    if (std::holds_alternative<Properties>(it)) {
                        properties.push_back(std::get<Properties>(it));
                    }
                }
                return properties.cbegin();
            }
            /**
             * @brief Return a const iterator to the end.
             * @return const_iterator The const iterator to the end.
             */
            [[nodiscard]] const_iterator cend() const {
                properties.clear();
                for (const auto& it : children) {
                    if (std::holds_alternative<Properties>(it)) {
                        properties.push_back(std::get<Properties>(it));
                    }
                }
                return properties.cend();
            }
            /**
             * @brief Return a reverse iterator to the beginning.
             * @return reverse_iterator The reverse iterator to the beginning.
             */
            reverse_iterator rbegin() {
                properties.clear();
                for (const auto& it : children) {
                    if (std::holds_alternative<Properties>(it)) {
                        properties.push_back(std::get<Properties>(it));
                    }
                }
                return properties.rbegin();
            }
            /**
             * @brief Return a reverse iterator to the end.
             * @return reverse_iterator The reverse iterator to the end.
             */
            reverse_iterator rend() {
                properties.clear();
                for (const auto& it : children) {
                    if (std::holds_alternative<Properties>(it)) {
                        properties.push_back(std::get<Properties>(it));
                    }
                }
                return properties.rend();
            }
            /**
             * @brief Return a const reverse iterator to the beginning.
             * @return const_reverse_iterator The const reverse iterator to the beginning.
             */
            [[nodiscard]] const_reverse_iterator crbegin() const {
                properties.clear();
                for (const auto& it : children) {
                    if (std::holds_alternative<Properties>(it)) {
                        properties.push_back(std::get<Properties>(it));
                    }
                }
                return properties.crbegin();
            }
            /**
             * @brief Return a const reverse iterator to the end.
             * @return const_reverse_iterator The const reverse iterator to the end.
             */
            [[nodiscard]] const_reverse_iterator crend() const {
                properties.clear();
                for (const auto& it : children) {
                    if (std::holds_alternative<Properties>(it)) {
                        properties.push_back(std::get<Properties>(it));
                    }
                }
                return properties.crend();
            }
            /**
             * @brief Return an iterator to the beginning.
             * @return variant_iterator The iterator to the beginning.
             */
            [[nodiscard]] variant_iterator variant_begin() {
                return children.begin();
            }
            /**
             * @brief Return an iterator to the end.
             * @return variant_iterator The iterator to the end.
             */
            [[nodiscard]] variant_iterator variant_end() {
                return children.end();
            }
            /**
             * @brief Return a const iterator to the beginning.
             * @return variant_const_iterator The const iterator to the beginning.
             */
            [[nodiscard]] variant_const_iterator variant_begin() const {
                return children.begin();
            }
            /**
             * @brief Return a const iterator to the end.
             * @return variant_const_iterator The const iterator to the end.
             */
            [[nodiscard]] variant_const_iterator variant_end() const {
                return children.end();
            }
            /**
             * @brief Return a const iterator to the beginning.
             * @return variant_const_iterator The const iterator to the beginning.
             */
            [[nodiscard]] variant_const_iterator variant_cbegin() const {
                return children.cbegin();
            }
            /**
             * @brief Return a const iterator to the end.
             * @return variant_const_iterator The const iterator to the end.
             */
            [[nodiscard]] variant_const_iterator variant_cend() const {
                return children.cend();
            }
            /**
             * @brief Return a reverse iterator to the beginning.
             * @return variant_reverse_iterator The reverse iterator to the beginning.
             */
            [[nodiscard]] variant_reverse_iterator variant_rbegin() {
                return children.rbegin();
            }
            /**
             * @brief Return a reverse iterator to the end.
             * @return variant_reverse_iterator The reverse iterator to the end.
             */
            [[nodiscard]] variant_reverse_iterator variant_rend() {
                return children.rend();
            }
            /**
             * @brief Return a const reverse iterator to the beginning.
             * @return variant_const_reverse_iterator The const reverse iterator to the beginning.
             */
            [[nodiscard]] variant_const_reverse_iterator variant_crbegin() const {
                return children.crbegin();
            }
            /**
             * @brief Return a const reverse iterator to the end.
             * @return variant_const_reverse_iterator The const reverse iterator to the end.
             */
            [[nodiscard]] variant_const_reverse_iterator variant_crend() const {
                return children.crend();
            }

            /**
             * @brief The npos value
             */
            static constexpr bygg::size_type npos = -1;

            /**
             * @brief Construct a new Element object
             * @param tag The tag of the element
             * @param properties The properties of the element
             * @param type The type of the element
             * @param p The pseudo class of the element
             */
            Element(string_type tag, const bygg::CSS::Properties& properties, Type type = Type::Selector, PseudoClass p = {}) : selector(std::move(tag)), children({properties}), type(type), pseudo(std::move(p)) {};
            /**
             * @brief Construct a new Element object
             * @param tag The tag of the element
             * @param properties The properties of the element
             * @param type The type of the element
             * @param p The pseudo class of the element
             */
            Element(HTML::Tag tag, const Properties& properties, Type type = Type::Selector, PseudoClass p = {}) : selector(resolve_tag(tag).first), children({properties}), type(type), pseudo(std::move(p)) {};
            /**
             * @brief Construct a new Element object
             * @param tag The tag of the element
             * @param type The type of the element
             * @param p The pseudo class of the element
             * @param properties The properties of the element
             */
            Element(string_type tag, Type type, PseudoClass p, const Properties& properties) : selector(std::move(tag)), children({properties}), type(type), pseudo(std::move(p)) {};
            /**
             * @brief Construct a new Element object
             * @param tag The tag of the element
             * @param type The type of the element
             * @param p The pseudo class of the element
             * @param properties The properties of the element
             */
            Element(HTML::Tag tag, Type type, PseudoClass p, const Properties& properties) : selector(resolve_tag(tag).first), children({properties}), type(type), pseudo(std::move(p)) {};
            /**
             * @brief Construct a new Element object
             * @param tag The tag of the element
             * @param type The type of the element
             * @param properties The properties of the element
             */
            Element(string_type tag, Type type, const Properties& properties) : selector(std::move(tag)), children({properties}), type(type) {};
            /**
             * @brief Construct a new Element object
             * @param tag The tag of the element
             * @param type The type of the element
             * @param properties The properties of the element
             */
            Element(HTML::Tag tag, Type type, const Properties& properties) : selector(resolve_tag(tag).first), children({properties}), type(type) {};
            /**
             * @brief Construct a new Element object
             * @param element The element to set
             */
            Element(const Element& element) = default;
            /**
             * @brief Construct a new Element object
             * @param tag The tag of the element
             * @param args The properties of the element
             */
            template <typename... Args, typename = std::enable_if_t<std::conjunction_v<std::disjunction<std::is_same<Args, Property>, std::is_same<Args, Element>, std::is_same<Args, Properties>>...>>>
            explicit Element(string_type tag, Args&&... args) :
                selector(std::move(tag)),
                children({bygg::CSS::Properties(std::forward<Args>(args)...)}) {};
            /**
             * @brief Construct a new Element object
             * @param tag The tag of the element
             * @param args The properties of the element
             */
            template <typename... Args, typename = std::enable_if_t<std::conjunction_v<std::disjunction<std::is_same<Args, Property>, std::is_same<Args, Element>, std::is_same<Args, Properties>>...>>>
            explicit Element(HTML::Tag tag, Args&&... args) :
                selector(resolve_tag(tag).first),
                children({bygg::CSS::Properties(std::forward<Args>(args)...)}) {};
            /**
             * @brief Construct a new Element object
             * @param tag The tag of the element
             * @param type The type of the element
             * @param p The pseudo class of the element
             * @param args The properties of the element
             */
            template <typename... Args> explicit Element(string_type tag, Type type, PseudoClass p, Args&&... args) :
                selector(std::move(tag)),
                type(type),
                pseudo(std::move(p)) {
                (push_back(std::forward<Args>(args)), ...);
            }
            /**
             * @brief Construct a new Element object
             * @param tag The tag of the element
             * @param type The type of the element
             * @param p The pseudo class of the element
             * @param args The properties of the element
             */
            template <typename... Args> explicit Element(HTML::Tag tag, Type type, PseudoClass p, Args&&... args) :
                selector(resolve_tag(tag).first),
                type(type),
                pseudo(std::move(p)) {
                (push_back(std::forward<Args>(args)), ...);
            }
            /**
             * @brief Construct a new Element object
             * @param tag The tag of the element
             * @param type The type of the element
             * @param args The properties of the element
             */
            template <typename... Args, typename = std::enable_if_t<std::conjunction_v<std::disjunction<std::is_same<Args, Property>, std::is_same<Args, Element>, std::is_same<Args, Properties>>...>>>
            explicit Element(string_type tag, Type type, Args&&... args) :
                selector(std::move(tag)),
                type(type) {
                (push_back(std::forward<Args>(args)), ...);
            }
            /**
             * @brief Construct a new Element object
             * @param tag The tag of the element
             * @param type The type of the element
             * @param args The properties of the element
             */
            template <typename... Args, typename = std::enable_if_t<std::conjunction_v<std::disjunction<std::is_same<Args, Property>, std::is_same<Args, Element>, std::is_same<Args, Properties>>...>>>
            explicit Element(HTML::Tag tag, Type type, Args&&... args) :
                selector(resolve_tag(tag).first),
                type(type) {
                (push_back(std::forward<Args>(args)), ...);
            }
            /**
             * @brief Construct a new Element object
             */
            Element() = default;
            /**
             * @brief Destroy the Element object
             */
            ~Element() = default;

            /**
             * @brief Prepend a property to the element
             * @param property The property to push
             */
            void push_front(const Property& property);
            /**
             * @brief Append a property to the element
             * @param property The property to push
             */
            void push_back(const Property& property);
            /**
             * @brief Prepend properties to the element
             * @param properties The properties to push
             */
            void push_front(const Properties& properties);
            /**
             * @brief Append properties to the element
             * @param properties The properties to push
             */
            void push_back(const Properties& properties);
            /**
             * @brief Prepend an element to the element
             * @param element The property to push
             */
            void push_front(const Element& element);
            /**
             * @brief Append an element to the element
             * @param element The element to push
             */
            void push_back(const Element& element);
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
            [[nodiscard]] bygg::size_type find(const Property &property) const;
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
            void swap(const Property& property1, const Property& property2);
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
             * @brief Clear the element
             */
            void clear();
            /**
             * @brief Check if the element is empty
             * @return bool True if the element is empty, false otherwise
             */
            [[nodiscard]] bool empty() const;
            /**
             * @brief Set the properties of the element
             * @param tag The tag of the element
             * @param properties The properties to set
             * @param type The type of the element
             * @param pseudo The pseudo class of the element
             */
            void set(const string_type& tag, const bygg::CSS::Properties& properties, Type type = Type::Selector, const PseudoClass& pseudo = {});
            /**
             * @brief Set the properties of the element
             * @param tag The tag of the element
             * @param properties The properties to set
             * @param type The type of the element
             * @param pseudo The pseudo class of the element
             */
            void set(HTML::Tag tag, const bygg::CSS::Properties& properties, Type type = Type::Selector, const PseudoClass& pseudo = {});
            /**
             * @brief Set the tag of the element
             * @param tag The tag to set
             */
            void set_tag(const string_type& tag);
            /**
             * @brief Set the tag of the element
             * @param tag The tag to set
             */
            void set_tag(HTML::Tag tag);
            /**
             * @brief Set the properties of the element
             * @param properties The properties to set
             */
            void set_properties(const bygg::CSS::Properties& properties);
            /**
             * @brief Set the type of the element
             * @param type The type to set
             */
            void set_type(Type type);
            /**
             * @brief Set the pseudo class of the element
             * @param pseudo The pseudo class to set
             */
            void set_pseudo(const PseudoClass& pseudo);
            /**
             * @brief Get the element
             * @return std::pair<string_type, bygg::CSS::Properties> The element
             */
            [[nodiscard]] string_type get(Formatting formatting = Formatting::None, integer_type tabc = 0) const;
            /**
             * @brief Get the element in the form of a specific type.
             * @return T The element in the form of a specific type
             */
            template <typename T> T get(const Formatting formatting = Formatting::None, const integer_type tabc = 0) const {
                if (std::is_same_v<T, string_type>) {
                    return this->get(formatting, tabc);
                }
                return T(this->get(formatting, tabc));
            }
            /**
             * @brief Get the tag of the element
             * @return string_type The tag of the element
             */
            [[nodiscard]] string_type get_tag() const;
            /**
             * @brief Get the tag of the element in a specific type
             * @return T The tag of the element
             */
            template <typename T> T get_tag() const {
                return T(this->selector);
            }
            /**
             * @brief Get the properties of the element
             * @return bygg::CSS::Properties The properties of the element
             */
            [[nodiscard]] bygg::CSS::Properties get_properties() const;
            /**
             * @brief Get the type of the element
             * @return Type The type of the element
             */
            [[nodiscard]] Type get_type() const;
            /**
             * @brief Get the pseudo class of the element
             * @return PseudoClass The pseudo class of the element
             */
            [[nodiscard]] PseudoClass get_pseudo() const;

            Element& operator=(const Element& element);
            Element& operator=(const std::pair<string_type, bygg::CSS::Properties>& element);
            Element& operator+=(const Property& property);
            Property& operator[](const size_type& index);
            bool operator==(const Element& element) const;
            bool operator!=(const Element& element) const;
    };

    template <typename... Args> Element make_element(Args&&... args) { return Element(std::forward<Args>(args)...); }
} // namespace bygg
