/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <bygg/HTML/property.hpp>

bygg::string_type bygg::HTML::Property::get_key() const {
    return this->property.first;
}

bygg::string_type bygg::HTML::Property::get_value() const {
    return this->property.second;
}

std::pair<bygg::string_type, bygg::string_type> bygg::HTML::Property::get_pair() const {
    return this->property;
}

bygg::string_type bygg::HTML::Property::get() const {
    return this->get_key() + "=\"" + this->get_value() + "\"";
}

void bygg::HTML::Property::set_key(const bygg::string_type& key) {
    this->property.first = key;
}

void bygg::HTML::Property::set_value(const bygg::string_type& value) {
    this->property.second = value;
}

void bygg::HTML::Property::set(const std::pair<bygg::string_type, bygg::string_type>& property) {
    this->property = property;
}

bygg::HTML::Property& bygg::HTML::Property::operator=(const bygg::HTML::Property& property) {
    this->set(property.get_pair());
    return *this;
}

bool bygg::HTML::Property::operator==(const bygg::HTML::Property& property) const {
    return this->property == property.get_pair();
}

bool bygg::HTML::Property::operator!=(const bygg::HTML::Property& property) const {
    return this->property != property.get_pair();
}

void bygg::HTML::Property::clear() {
    this->property = {};
}

bool bygg::HTML::Property::empty() const {
    return this->property.first.empty() && this->property.second.empty();
}