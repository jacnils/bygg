/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <sstream>
#include <bygg/types.hpp>
#include <bygg/except.hpp>
#include <bygg/CSS/function.hpp>

void bygg::CSS::Function::push_back(const string_type& parameter) {
    parameters.push_back(parameter);
}

void bygg::CSS::Function::emplace_back(string_type&& parameter) {
    parameters.emplace_back(std::move(parameter));
}

void bygg::CSS::Function::push_back(double parameter) {
    parameters.push_back(std::to_string(parameter));
}

void bygg::CSS::Function::emplace_back(double parameter) {
    parameters.emplace_back(std::to_string(parameter));
}

void bygg::CSS::Function::push_back(int parameter) {
    parameters.push_back(std::to_string(parameter));
}

void bygg::CSS::Function::emplace_back(int parameter) {
    parameters.emplace_back(std::to_string(parameter));
}

void bygg::CSS::Function::pop_back() {
    if (parameters.empty()) {
        throw out_of_range("No elements to pop");
    }
    parameters.pop_back();
}

[[nodiscard]] bygg::string_type bygg::CSS::Function::at(size_type index) const {
    if (index >= parameters.size()) {
        throw out_of_range("Index out of range");
    }
    return parameters.at(index);
}

[[nodiscard]] bygg::string_type& bygg::CSS::Function::at(size_type index) {
    if (index >= parameters.size()) {
        throw out_of_range("Index out of range");
    }
    return parameters.at(index);
}

[[nodiscard]] bygg::size_type bygg::CSS::Function::find(const string_type& parameter, size_type start) const {
    if (start >= parameters.size()) {
        return npos;
    }
    for (size_t i = start; i < parameters.size(); ++i) {
        if (parameters[i] == parameter) {
            return i;
        }
    }
    return npos;
}

[[nodiscard]] bygg::size_type bygg::CSS::Function::find(const string_type& parameter, size_type start) {
    if (start >= parameters.size()) {
        return npos;
    }
    for (size_t i = start; i < parameters.size(); ++i) {
        if (parameters[i] == parameter) {
            return i;
        }
    }
    return npos;
}

[[nodiscard]] bygg::string_type& bygg::CSS::Function::operator[](size_type index) {
    return parameters[index];
}

[[nodiscard]] bygg::string_type bygg::CSS::Function::operator[](size_type index) const {
    return parameters[index];
}

[[nodiscard]] bygg::string_type& bygg::CSS::Function::back() {
    return parameters.back();
}

[[nodiscard]] bygg::string_type bygg::CSS::Function::back() const {
    return parameters.back();
}
[[nodiscard]] bygg::string_type bygg::CSS::Function::front() const {
    return parameters.front();
}

[[nodiscard]] bygg::string_type& bygg::CSS::Function::front() {
    return parameters.front();
}

[[nodiscard]] bygg::size_type bygg::CSS::Function::size() const {
    return parameters.size();
}

[[nodiscard]] bool bygg::CSS::Function::empty() const {
    return parameters.empty();
}

[[nodiscard]] bygg::string_type bygg::CSS::Function::get_name() const {
    return name;
}

[[nodiscard]] std::vector<bygg::string_type> bygg::CSS::Function::get_parameters() const {
    return parameters;
}

[[nodiscard]] bygg::string_type bygg::CSS::Function::get() const {
    if (name.empty()) {
        throw invalid_argument("Function name is empty");
    }
    if (parameters.empty()) {
        return name + "()";
    }
    std::ostringstream oss;
    oss << name << "(";
    for (size_t i = 0; i < parameters.size(); ++i) {
        oss << parameters[i];
        if (i != parameters.size() - 1) {
            oss << ", ";
        }
    }
    oss << ")";
    return oss.str();
}

const char* bygg::CSS::Function::c_str() const { // NOLINT
     this->cache = get();
     return this->cache.c_str();
}

void bygg::CSS::Function::clear() {
    name.clear();
    parameters.clear();
}

void bygg::CSS::Function::set_name(const bygg::string_type& name) {
    this->name = name;
}

void bygg::CSS::Function::set_parameters(const std::vector<bygg::string_type>& parameters) {
    this->parameters = parameters;
}

void bygg::CSS::Function::set(const string_type& name, const std::vector<string_type>& parameters) {
    this->name = name;
    this->parameters = parameters;
}

bool bygg::CSS::Function::operator==(const Function& other) const {
    return name == other.name && parameters == other.parameters;
}

bool bygg::CSS::Function::operator==(const string_type& other) const {
    return get() == other;
}

bygg::CSS::Function& bygg::CSS::Function::operator=(const Function& other) {
    name = other.name;
    parameters = other.parameters;
    return *this;
}

bygg::CSS::Function::operator bygg::string_type() const { // NOLINT
    return get();
}

void bygg::CSS::Function::operator+=(const string_type& parameter) {
    push_back(parameter);
}

void bygg::CSS::Function::operator+=(const Function& other) {
    for (const auto& parameter : other.parameters) {
        push_back(parameter);
    }
}

bygg::CSS::Function bygg::CSS::Function::operator+(const string_type& parameter) const {
    Function function = *this;
    function += parameter;
    return function;
}

[[nodiscard]] bygg::CSS::Function::iterator bygg::CSS::Function::begin() { return parameters.begin(); }
[[nodiscard]] bygg::CSS::Function::iterator bygg::CSS::Function::end() { return parameters.end(); }
[[nodiscard]] bygg::CSS::Function::const_iterator bygg::CSS::Function::begin() const { return parameters.begin(); }
[[nodiscard]] bygg::CSS::Function::const_iterator bygg::CSS::Function::end() const { return parameters.end(); }
[[nodiscard]] bygg::CSS::Function::const_iterator bygg::CSS::Function::cbegin() const { return parameters.cbegin(); }
[[nodiscard]] bygg::CSS::Function::const_iterator bygg::CSS::Function::cend() const { return parameters.cend(); }
[[nodiscard]] bygg::CSS::Function::reverse_iterator bygg::CSS::Function::rbegin() { return parameters.rbegin(); }
[[nodiscard]] bygg::CSS::Function::reverse_iterator bygg::CSS::Function::rend() { return parameters.rend(); }
[[nodiscard]] bygg::CSS::Function::const_reverse_iterator bygg::CSS::Function::crbegin() const { return parameters.crbegin(); }
[[nodiscard]] bygg::CSS::Function::const_reverse_iterator bygg::CSS::Function::crend() const { return parameters.crend(); }
