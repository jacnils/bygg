/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <bygg/except.hpp>
#include <bygg/CSS/stylesheet.hpp>

void bygg::CSS::Stylesheet::set_elements(const bygg::CSS::ElementList& elements) {
    this->elements = elements;
}

void bygg::CSS::Stylesheet::push_front(const Element& element) {
    this->elements.insert(this->elements.begin(), element);
}

void bygg::CSS::Stylesheet::push_back(const Element& element) {
    this->elements.push_back(element);
}

void bygg::CSS::Stylesheet::insert(const size_type index, const Element& element) {
    if (index >= this->elements.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    this->elements.insert(this->elements.begin() + static_cast<long>(index), element);
}

void bygg::CSS::Stylesheet::erase(const size_type index) {
    if (index >= this->elements.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    this->elements.erase(this->elements.begin() + static_cast<long>(index));
}

bygg::CSS::Stylesheet& bygg::CSS::Stylesheet::operator=(const bygg::CSS::Stylesheet& stylesheet) {
    this->set_elements(stylesheet.get_elements());
    return *this;
}

bygg::CSS::Stylesheet& bygg::CSS::Stylesheet::operator+=(const Element& element) {
    this->push_back(element);
    return *this;
}

bygg::CSS::Element bygg::CSS::Stylesheet::operator[](const bygg::integer_type& index) const {
    return this->at(index);
}

bygg::CSS::Element& bygg::CSS::Stylesheet::operator[](const bygg::integer_type& index) {
    return this->at(index);
}

bool bygg::CSS::Stylesheet::operator==(const bygg::CSS::Stylesheet& stylesheet) const {
    return this->get_elements() == stylesheet.get_elements();
}

bool bygg::CSS::Stylesheet::operator!=(const bygg::CSS::Stylesheet& stylesheet) const {
    return this->get_elements() != stylesheet.get_elements();
}

bygg::CSS::Element& bygg::CSS::Stylesheet::at(const size_type index) {
    if (index >= this->elements.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    return this->elements.at(index);
}

bygg::CSS::Element bygg::CSS::Stylesheet::at(const size_type index) const {
    if (index >= this->elements.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    return this->elements.at(index);
}

bygg::size_type bygg::CSS::Stylesheet::find(const Element& element) const {
    for (size_type i{0}; i < this->elements.size(); i++) {
        if (this->elements.at(i).get() == element.get()) {
            return i;
        }
    }

    return bygg::CSS::Stylesheet::npos;
}

bygg::size_type bygg::CSS::Stylesheet::find(const bygg::string_type& str) const {
    for (size_type i{0}; i < this->elements.size(); i++) {
        if (this->elements.at(i).get() == str || this->elements.at(i).get_tag() == str) {
            return i;
        }
    }

    return bygg::CSS::Stylesheet::npos;
}

bygg::size_type bygg::CSS::Stylesheet::size() const {
    return this->elements.size();
}

bool bygg::CSS::Stylesheet::empty() const {
    return this->elements.empty();
}

void bygg::CSS::Stylesheet::clear() {
    this->elements.clear();
}

bygg::CSS::Element bygg::CSS::Stylesheet::front() const {
    return this->elements.front();
}

bygg::CSS::Element bygg::CSS::Stylesheet::back() const {
    return this->elements.back();
}

bygg::CSS::Element& bygg::CSS::Stylesheet::front() {
    return this->elements.front();
}

bygg::CSS::Element& bygg::CSS::Stylesheet::back() {
    return this->elements.back();
}

void bygg::CSS::Stylesheet::swap(const size_type index1, const size_type index2) {
    if (index1 >= this->elements.size() || index2 >= this->elements.size()) {
        throw bygg::out_of_range("Index out of range");
    }

    std::swap(this->elements[index1], this->elements[index2]);
}

void bygg::CSS::Stylesheet::swap(const Element& element1, const Element& element2) {
    this->swap(this->find(element1), this->find(element2));
}

std::vector<bygg::CSS::Element> bygg::CSS::Stylesheet::get_elements() const {
    return this->elements;
}

bygg::string_type bygg::CSS::Stylesheet::get(const Formatting formatting, const bygg::integer_type tabc) const {
    bygg::string_type ret{};

    for (const Element& it : this->elements) {
        ret += it.get(formatting, tabc);
    }

    return ret;
}