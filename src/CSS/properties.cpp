/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <algorithm>
#include <bygg/except.hpp>
#include <bygg/CSS/properties.hpp>
#include <bygg/CSS/formatting_enum.hpp>

bygg::CSS::Properties& bygg::CSS::Properties::operator=(const bygg::CSS::Property& property) {
    this->properties = {property};
    return *this;
}

bygg::CSS::Properties& bygg::CSS::Properties::operator=(const bygg::CSS::Properties& properties) {
    this->set(properties.get_properties());
    return *this;
}

bygg::CSS::Properties& bygg::CSS::Properties::operator=(const bygg::CSS::PropertyList& properties) {
    this->set(properties);
    return *this;
}

bygg::CSS::Property& bygg::CSS::Properties::operator[](const size_type& index) {
    return this->properties.at(index);
}

bygg::CSS::Property bygg::CSS::Properties::operator[](const size_type& index) const {
    return this->properties.at(index);
}

bool bygg::CSS::Properties::operator==(const bygg::CSS::Properties& properties) const {
    return this->properties == properties.get_properties();
}

bool bygg::CSS::Properties::operator==(const bygg::CSS::Property& property) const {
    return std::any_of(this->properties.begin(), this->properties.end(),
                   [&property](const bygg::CSS::Property& it) {
                       return it.get_pair() == property.get_pair();
                   });
}

bool bygg::CSS::Properties::operator!=(const bygg::CSS::Properties& properties) const {
    return this->properties != properties.get_properties();
}

bool bygg::CSS::Properties::operator!=(const bygg::CSS::Property& property) const {
    return std::all_of(this->properties.begin(), this->properties.end(), [&property](const bygg::CSS::Property& it) {
        return it.get_pair() == property.get_pair();
        });
}

bygg::CSS::Properties& bygg::CSS::Properties::operator+=(const bygg::CSS::Property& property) {
    this->push_back(property);
    return *this;
}

bygg::CSS::Properties& bygg::CSS::Properties::operator+=(const bygg::CSS::Properties& properties) {
    for (const bygg::CSS::Property& it : properties) {
        this->push_back(it);
    }

    return *this;
}

bygg::CSS::PropertyList bygg::CSS::Properties::get_properties() const {
    return this->properties;
}


bygg::string_type bygg::CSS::Properties::get(bygg::CSS::Formatting formatting, bygg::integer_type tabc) const {
    string_type ret;

    for (const bygg::CSS::Property& property : this->properties) {
        ret += property.get(formatting, tabc);
    }

    return ret;
}

bygg::CSS::Property& bygg::CSS::Properties::at(const size_type index) {
    if (index >= this->properties.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    return this->properties.at(index);
}

bygg::CSS::Property bygg::CSS::Properties::at(const size_type index) const {
    if (index >= this->properties.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    return this->properties.at(index);
}

void bygg::CSS::Properties::set(const bygg::CSS::PropertyList& properties) {
    this->properties = properties;
}

void bygg::CSS::Properties::insert(const size_type index, const bygg::CSS::Property& property) {
    if (index >= this->properties.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    this->properties.insert(this->properties.begin() + static_cast<long>(index), property);
}

void bygg::CSS::Properties::erase(const size_type index) {
    if (index >= this->properties.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    this->properties.erase(this->properties.begin() + static_cast<long>(index));
}

void bygg::CSS::Properties::push_front(const bygg::CSS::Property& property) {
    this->properties.insert(this->properties.begin(), property);
}

void bygg::CSS::Properties::push_back(const bygg::CSS::Property& property) {
    this->properties.push_back(property);
}

bygg::size_type bygg::CSS::Properties::find(const bygg::CSS::Property& property) const {
    for (size_type i{0}; i < this->properties.size(); i++) {
        if (this->properties.at(i).get_value().find(property.get_value()) != bygg::string_type::npos
            || this->properties.at(i).get_key().find(property.get_key()) != bygg::string_type::npos) {
            return i;
        }
    }

    return npos;
}

bygg::size_type bygg::CSS::Properties::find(const bygg::string_type& str) const {
    for (size_type i{0}; i < this->properties.size(); i++) {
        if (this->properties.at(i).get_key().find(str) != bygg::string_type::npos ||
            this->properties.at(i).get_value().find(str) != bygg::string_type::npos) {
            return i;
        }
    }

    return npos;
}

bygg::CSS::Property& bygg::CSS::Properties::front() {
    return this->properties.front();
}

bygg::CSS::Property& bygg::CSS::Properties::back() {
    return this->properties.back();
}

bygg::CSS::Property bygg::CSS::Properties::front() const {
    return this->properties.front();
}

bygg::CSS::Property bygg::CSS::Properties::back() const {
    return this->properties.back();
}

bygg::size_type bygg::CSS::Properties::size() const {
    return this->properties.size();
}

void bygg::CSS::Properties::clear() {
    this->properties.clear();
}

bool bygg::CSS::Properties::empty() const {
    return this->properties.empty();
}

void bygg::CSS::Properties::swap(const size_type index1, const size_type index2) {
    if (index1 >= this->properties.size() || index2 >= this->properties.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    std::swap(this->properties[index1], this->properties[index2]);
}

void bygg::CSS::Properties::swap(const bygg::CSS::Property& property1, const bygg::CSS::Property& property2) {
    this->swap(this->find(property1), this->find(property2));
}