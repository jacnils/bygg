/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <bygg/HTML/section.hpp>
#include <bygg/HTML/document.hpp>

bygg::string_type bygg::HTML::Document::get(const Formatting formatting, const bygg::integer_type tabc) const {
    return this->doctype + (formatting == Formatting::Pretty ? "\n" : formatting == Formatting::Newline ? "\n" : "") + this->document.get(formatting, tabc);
}

bygg::HTML::Section bygg::HTML::Document::get_section() const {
    return this->document;
}

bygg::HTML::Section& bygg::HTML::Document::get_section() {
    return this->document;
}

void bygg::HTML::Document::set(const bygg::HTML::Section& document) {
    this->document = document;
}

bygg::size_type bygg::HTML::Document::size() const {
    return this->document.size();
}

void bygg::HTML::Document::set_doctype(const bygg::string_type& doctype) {
    this->doctype = doctype;
}

void bygg::HTML::Document::clear() {
    this->doctype.clear();
    this->document.clear();
}

bool bygg::HTML::Document::empty() const {
    return this->doctype.empty() && this->document.empty();
}

bygg::HTML::Document& bygg::HTML::Document::operator=(const bygg::HTML::Document& document) {
    this->set(document.get_section());
    this->set_doctype(document.get_doctype());
    return *this;
}

bygg::HTML::Document& bygg::HTML::Document::operator=(const bygg::HTML::Section& section) {
    this->set(section);
    return *this;
}

bool bygg::HTML::Document::operator==(const bygg::HTML::Document& document) const {
    return this->get() == document.get();
}

bool bygg::HTML::Document::operator==(const bygg::HTML::Section& section) const {
    return this->document == section;
}

bool bygg::HTML::Document::operator!=(const bygg::HTML::Document& document) const {
    return this->doctype != document.get_doctype() || this->document != document.document;
}

bool bygg::HTML::Document::operator!=(const bygg::HTML::Section& section) const {
    return this->document != section;
}

bygg::string_type bygg::HTML::Document::get_doctype() const {
    return this->doctype;
}