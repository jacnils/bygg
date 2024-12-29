/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <algorithm>
#include <bygg/except.hpp>
#include <bygg/HTML/properties.hpp>

bygg::HTML::Properties& bygg::HTML::Properties::operator=(const bygg::HTML::Property& property) {
    this->properties = {property};
    return *this;
}

bygg::HTML::Properties& bygg::HTML::Properties::operator=(const bygg::HTML::Properties& properties) {
    this->set(properties.get_properties());
    return *this;
}

bygg::HTML::Properties& bygg::HTML::Properties::operator=(const std::vector<bygg::HTML::Property>& properties) {
    this->set(properties);
    return *this;
}

bygg::HTML::Property& bygg::HTML::Properties::operator[](const size_type& index) {
    return this->at(index);
}

bygg::HTML::Property bygg::HTML::Properties::operator[](const size_type& index) const {
    return this->at(index);
}

bool bygg::HTML::Properties::operator==(const bygg::HTML::Properties& properties) const {
    return this->properties == properties.get_properties();
}

bool bygg::HTML::Properties::operator==(const bygg::HTML::Property& property) const {
    return std::any_of(this->properties.begin(), this->properties.end(),
                   [&property](const bygg::HTML::Property& it) {
                       return it.get_key() == property.get_key() && it.get_value() == property.get_value();
                   });
}

bool bygg::HTML::Properties::operator!=(const bygg::HTML::Properties& properties) const {
    return this->properties != properties.get_properties();
}

bool bygg::HTML::Properties::operator!=(const bygg::HTML::Property& property) const {
    return std::all_of(this->properties.begin(), this->properties.end(), [&property](const bygg::HTML::Property& it) {
        return it.get_key() == property.get_key() && it.get_value() == property.get_value();
        });
}

bygg::HTML::Properties& bygg::HTML::Properties::operator+=(const bygg::HTML::Property& property) {
    this->push_back(property);
    return *this;
}

bygg::HTML::Properties& bygg::HTML::Properties::operator+=(const bygg::HTML::Properties& properties) {
    for (const bygg::HTML::Property& it : properties) {
        this->push_back(it);
    }

    return *this;
}

std::vector<bygg::HTML::Property> bygg::HTML::Properties::get_properties() const {
    return this->properties;
}

bygg::HTML::Property& bygg::HTML::Properties::at(const size_type index) {
    if (index >= this->properties.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    return this->properties.at(index);
}

bygg::HTML::Property bygg::HTML::Properties::at(const size_type index) const {
    if (index >= this->properties.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    return this->properties.at(index);
}

void bygg::HTML::Properties::set(const std::vector<bygg::HTML::Property>& properties) {
    this->properties = properties;
}

void bygg::HTML::Properties::insert(const size_type index, const bygg::HTML::Property& property) {
    if (index >= this->properties.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    this->properties.insert(this->properties.begin() + static_cast<long>(index), property);
}

void bygg::HTML::Properties::erase(const size_type index) {
    if (index >= this->properties.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    this->properties.erase(this->properties.begin() + static_cast<long>(index));
}

void bygg::HTML::Properties::push_front(const bygg::HTML::Property& property) {
    this->properties.insert(this->properties.begin(), property);
}

void bygg::HTML::Properties::push_back(const bygg::HTML::Property& property) {
    this->properties.push_back(property);
}

bygg::size_type bygg::HTML::Properties::find(const bygg::HTML::Property& property, const size_type begin) const {
    for (size_type i{0}; i < this->properties.size(); i++) {
        if ((this->properties.at(i).get_value().find(property.get_value()) != bygg::string_type::npos
            || this->properties.at(i).get_key().find(property.get_key()) != bygg::string_type::npos) && i >= begin) {
            return i;
        }
    }

    return npos;
}

bygg::size_type bygg::HTML::Properties::find(const bygg::string_type& str, const size_type begin) const {
    for (size_type i{0}; i < this->properties.size(); i++) {
        if ((this->properties.at(i).get_key().find(str) != bygg::string_type::npos ||
            this->properties.at(i).get_value().find(str) != bygg::string_type::npos) && i >= begin) {
            return i;
        }
    }

    return npos;
}

bygg::HTML::Property bygg::HTML::Properties::front() const {
    return this->properties.front();
}

bygg::HTML::Property bygg::HTML::Properties::back() const {
    return this->properties.back();
}

bygg::HTML::Property& bygg::HTML::Properties::front() {
    return this->properties.front();
}

bygg::HTML::Property& bygg::HTML::Properties::back() {
    return this->properties.back();
}

bygg::size_type bygg::HTML::Properties::size() const {
    return this->properties.size();
}

void bygg::HTML::Properties::clear() {
    this->properties.clear();
}

bool bygg::HTML::Properties::empty() const {
    return this->properties.empty();
}

void bygg::HTML::Properties::swap(const size_type index1, const size_type index2) {
    if (index1 >= this->properties.size() || index2 >= this->properties.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    std::swap(this->properties[index1], this->properties[index2]);
}

void bygg::HTML::Properties::swap(const bygg::HTML::Property& property1, const bygg::HTML::Property& property2) {
    this->swap(this->find(property1), this->find(property2));
}