/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <bygg/except.hpp>
#include <bygg/CSS/element.hpp>

bygg::CSS::Element& bygg::CSS::Element::operator=(const bygg::CSS::Element& element) {
    this->set(element.get_tag(), element.get_properties(), element.get_type(), element.get_pseudo());
    return *this;
}

bygg::CSS::Element& bygg::CSS::Element::operator=(const std::pair<string_type, bygg::CSS::Properties>& element) {
    this->children = {element.second};
    return *this;
}

bygg::CSS::Element& bygg::CSS::Element::operator+=(const Property& property) {
    this->push_back(property);
    return *this;
}

bygg::CSS::Property& bygg::CSS::Element::operator[](const size_type& index) {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            return std::get<Properties>(this->children.at(i)).at(index);
        }
    }

    throw out_of_range("Index out of range");
}

bool bygg::CSS::Element::operator==(const bygg::CSS::Element& element) const {
    return this->get() == element.get();
}

bool bygg::CSS::Element::operator!=(const bygg::CSS::Element& element) const {
    return this->get() != element.get();
}

void bygg::CSS::Element::set(const bygg::string_type& tag, const bygg::CSS::Properties& properties, const Type type, const PseudoClass& pseudo) {
    this->selector = tag;
    this->children = {properties};
    this->type = type;
    this->pseudo = pseudo;
}

void bygg::CSS::Element::set_tag(const bygg::string_type& tag) {
    this->selector = tag;
}

void bygg::CSS::Element::set_tag(const HTML::Tag tag) {
    this->selector = resolve_tag(tag).first;
}

void bygg::CSS::Element::set_properties(const bygg::CSS::Properties& properties) {
    this->children = {properties};
}

void bygg::CSS::Element::set_pseudo(const PseudoClass &pseudo) {
    this->pseudo = pseudo;
}

void bygg::CSS::Element::set_type(const Type type) {
    this->type = type;
}

void bygg::CSS::Element::set(HTML::Tag tag, const bygg::CSS::Properties& properties, const Type type, const PseudoClass& pseudo) {
    this->set(resolve_tag(tag).first, properties, type, pseudo);
}

void bygg::CSS::Element::push_front(const Property& property) {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            std::get<Properties>(this->children.at(i)).push_front(property);
            return;
        }
    }

    throw internal_error("Failed to push property to front");
}

void bygg::CSS::Element::push_back(const Property& property) {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            std::get<Properties>(this->children.at(i)).push_back(property);
            return;
        }
    }

    throw internal_error("Failed to push property to back");
}

void bygg::CSS::Element::push_front(const Properties& properties) {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            auto& ref = std::get<Properties>(this->children.at(i));

            for (const auto& property : properties) {
                ref.push_front(property);
            }

            return;
        }
    }

    throw internal_error("Failed to push properties to front");
}

void bygg::CSS::Element::push_back(const Properties& properties) {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            auto& ref = std::get<Properties>(this->children.at(i));

            for (const auto& property : properties) {
                ref.push_back(property);
            }

            return;
        }
    }

    throw internal_error("Failed to push property to back");
}

void bygg::CSS::Element::push_front(const Element& element) {
    this->children.insert(this->children.begin(), element);
}

void bygg::CSS::Element::push_back(const Element& element) {
    this->children.emplace_back(element);
}

void bygg::CSS::Element::insert(const size_type index, const Property& property) {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            auto& ref = std::get<Properties>(this->children.at(i));

            if (index >= ref.size()) {
                throw out_of_range("Index out of range");
            }

            ref.insert(static_cast<long>(index), property);
            return;
        }
    }

    throw internal_error("Failed to insert property");
}

void bygg::CSS::Element::erase(const size_type index) {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            auto& ref = std::get<Properties>(this->children.at(i));

            if (index >= ref.size()) {
                throw bygg::out_of_range("Index out of range");
            }

            ref.erase(static_cast<long>(index));
            return;
        }
    }

    throw internal_error("Failed to erase property");
}

bygg::CSS::Property& bygg::CSS::Element::at(const size_type index) {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            return std::get<Properties>(this->children.at(i)).at(index);
        }
    }

    throw out_of_range("Index out of range");
}

bygg::CSS::Property bygg::CSS::Element::at(const size_type index) const {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            return std::get<Properties>(this->children.at(i)).at(index);
        }
    }

    throw out_of_range("Index out of range");
}

bygg::size_type bygg::CSS::Element::find(const Property& property) const {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            return std::get<Properties>(this->children.at(i)).find(property);
        }
    }

    return npos;
}

bygg::size_type bygg::CSS::Element::find(const bygg::string_type& str) const {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            return std::get<Properties>(this->children.at(i)).find(str);
        }
    }

    return npos;
}

bygg::CSS::Property& bygg::CSS::Element::front() {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            return std::get<Properties>(this->children.at(i)).front();
        }
    }

    throw internal_error{"Failed to get front property"};
}

bygg::CSS::Property& bygg::CSS::Element::back() {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            return std::get<Properties>(this->children.at(i)).back();
        }
    }

    throw internal_error{"Failed to get back property"};
}

bygg::CSS::Property bygg::CSS::Element::front() const {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            return std::get<Properties>(this->children.at(i)).front();
        }
    }

    throw internal_error{"Failed to get front property"};
}

bygg::CSS::Property bygg::CSS::Element::back() const {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            return std::get<Properties>(this->children.at(i)).back();
        }
    }

    throw internal_error{"Failed to get back property"};
}

bygg::size_type bygg::CSS::Element::size() const {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            return this->children.size() + std::get<Properties>(this->children.at(i)).size() - 1;
        }
    }

    throw internal_error{"Failed to get size of properties"};
}

bool bygg::CSS::Element::empty() const {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            return std::get<Properties>(this->children.at(i)).empty() && this->children.size() == 1;
        }
    }

    throw internal_error{"Failed to check if properties are empty"};
}

void bygg::CSS::Element::clear() {
    this->children.clear();
    this->properties.clear();
    this->elements.clear();
}

void bygg::CSS::Element::swap(const size_type index1, const size_type index2) {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            std::get<Properties>(this->children.at(i)).swap(index1, index2);
            return;
        }
    }

    throw internal_error("Failed to swap properties");
}

void bygg::CSS::Element::swap(const Property& property1, const Property& property2) {
    this->swap(this->find(property1), this->find(property2));
}

bygg::string_type bygg::CSS::Element::get(const Formatting formatting, const bygg::integer_type tabc) const {
    string_type ret{};

    if (formatting == bygg::CSS::Formatting::Pretty && !this->selector.empty()) {
        for (size_type i{0}; i < tabc; i++) {
            ret += "\t";
        }
    }

    switch (this->type) {
        case Type::Selector:
            break;
        case Type::Class:
            ret += ".";
            break;
        case Type::Id:
            ret += "#";
            break;
        case Type::Rule:
            ret += "@";
            break;
        default:
            break;
    }

    ret += !this->selector.empty() ? (this->selector) : "";
    ret += !this->pseudo.empty() ? (":" + this->pseudo + " {") : this->selector.empty() ? "" : " {";

    if (formatting == bygg::CSS::Formatting::Pretty || formatting == bygg::CSS::Formatting::Newline) {
        ret += "\n";
    }

    for (const auto& it : this->children) {
        if (!std::holds_alternative<Properties>(it)) {
            continue;
        }

        ret += std::get<Properties>(it).get(formatting, tabc + 1);
    }

    for (const auto& it : this->children) {
        if (!std::holds_alternative<Element>(it)) {
            continue;
        }

        ret += std::get<Element>(it).get(formatting, tabc + 1);
    }

    if (formatting == bygg::CSS::Formatting::Pretty && !this->selector.empty()) {
        for (size_type i{0}; i < tabc; i++) {
            ret += "\t";
        }
    }

    ret += (!this->selector.empty() ? "}" : "");

    if (formatting == bygg::CSS::Formatting::Pretty || formatting == bygg::CSS::Formatting::Newline) {
        ret += "\n";
    }

    return ret;
}

bygg::string_type bygg::CSS::Element::get_tag() const {
    return this->selector;
}

bygg::CSS::Properties bygg::CSS::Element::get_properties() const {
    ensure_has_properties();

    for (size_type i{0}; i < this->children.size(); i++) {
        if (std::holds_alternative<Properties>(this->children.at(i))) {
            return std::get<Properties>(this->children.at(i));
        }
    }

    throw internal_error("Failed to get properties");
}

bygg::CSS::PseudoClass bygg::CSS::Element::get_pseudo() const {
    return this->pseudo;
}

bygg::CSS::Type bygg::CSS::Element::get_type() const {
    return this->type;
}