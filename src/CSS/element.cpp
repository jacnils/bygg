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
    this->element = element;
    return *this;
}

bygg::CSS::Element& bygg::CSS::Element::operator+=(const Property& property) {
    this->push_back(property);
    return *this;
}

bygg::CSS::Property& bygg::CSS::Element::operator[](const size_type& index) {
    return this->element.second[index];
}

bool bygg::CSS::Element::operator==(const bygg::CSS::Element& element) const {
    return this->get() == element.get();
}

bool bygg::CSS::Element::operator!=(const bygg::CSS::Element& element) const {
    return this->get() != element.get();
}

void bygg::CSS::Element::set(const bygg::string_type& tag, const bygg::CSS::Properties& properties, const Type type, const PseudoClass& pseudo) {
    this->element.first = tag;
    this->element.second = properties;
    this->type = type;
    this->pseudo = pseudo;
}

void bygg::CSS::Element::set_tag(const bygg::string_type& tag) {
    this->element.first = tag;
}

void bygg::CSS::Element::set_tag(const HTML::Tag tag) {
    this->element.first = resolve_tag(tag).first;
}

void bygg::CSS::Element::set_properties(const bygg::CSS::Properties& properties) {
    this->element.second = properties;
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
    this->element.second.push_front(property);
}

void bygg::CSS::Element::push_back(const Property& property) {
    this->element.second.push_back(property);
}

void bygg::CSS::Element::insert(const size_type index, const Property& property) {
    if (index >= this->element.second.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    this->element.second.insert(static_cast<long>(index), property);
}

void bygg::CSS::Element::erase(const size_type index) {
    if (index >= this->element.second.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    this->element.second.erase(static_cast<long>(index));
}

bygg::CSS::Property& bygg::CSS::Element::at(const size_type index) {
    if (index >= this->element.second.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    return this->element.second.at(index);
}

bygg::CSS::Property bygg::CSS::Element::at(const size_type index) const {
    if (index >= this->element.second.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    return this->element.second.at(index);
}

bygg::size_type bygg::CSS::Element::find(const Property& property) const {
    for (size_type i{0}; i < this->element.second.size(); i++) {
        if (this->element.second.at(i).get_key() == property.get_key() ||
            this->element.second.at(i).get_value() == property.get_value()) {
            return i;
        }
    }

    return npos;
}

bygg::size_type bygg::CSS::Element::find(const bygg::string_type& str) const {
    for (size_type i{0}; i < this->element.second.size(); i++) {
        if (this->element.second.at(i).get_key() == str || this->element.second.at(i).get_value() == str) {
            return i;
        }
    }

    return npos;
}

bygg::CSS::Property& bygg::CSS::Element::front() {
    return this->element.second.front();
}

bygg::CSS::Property& bygg::CSS::Element::back() {
    return this->element.second.back();
}

bygg::CSS::Property bygg::CSS::Element::front() const {
    return this->element.second.front();
}

bygg::CSS::Property bygg::CSS::Element::back() const {
    return this->element.second.back();
}

bygg::size_type bygg::CSS::Element::size() const {
    return this->element.second.size();
}

bool bygg::CSS::Element::empty() const {
    return this->element.second.empty();
}

void bygg::CSS::Element::clear() {
    this->element.first.clear();
    this->element.second.clear();
}

void bygg::CSS::Element::swap(const size_type index1, const size_type index2) {
    if (index1 >= this->element.second.size() || index2 >= this->element.second.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    std::swap(this->element.second[index1], this->element.second[index2]);
}

void bygg::CSS::Element::swap(const Property& property1, const Property& property2) {
    this->swap(this->find(property1), this->find(property2));
}

bygg::string_type bygg::CSS::Element::get(const Formatting formatting, const bygg::integer_type tabc) const {
    string_type ret{};

    if (formatting == bygg::CSS::Formatting::Pretty && !this->element.first.empty()) {
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
        case Type::Property:
            ret += "@";
            break;
        default:
            break;
    }

    ret += !this->element.first.empty() ? (this->element.first) : "";
    ret += !this->pseudo.empty() ? (":" + this->pseudo + " {") : this->element.first.empty() ? "" : " {";

    if (formatting == bygg::CSS::Formatting::Pretty || formatting == bygg::CSS::Formatting::Newline) {
        ret += "\n";
    }

    ret += this->element.second.get(formatting, tabc + 1) + (!this->element.first.empty() ? "}" : "");

    if (formatting == bygg::CSS::Formatting::Pretty || formatting == bygg::CSS::Formatting::Newline) {
        ret += "\n";
    }

    return ret;
}

bygg::string_type bygg::CSS::Element::get_tag() const {
    return this->element.first;
}

bygg::CSS::Properties bygg::CSS::Element::get_properties() const {
    return this->element.second;
}

bygg::CSS::PseudoClass bygg::CSS::Element::get_pseudo() const {
    return this->pseudo;
}

bygg::CSS::Type bygg::CSS::Element::get_type() const {
    return this->type;
}