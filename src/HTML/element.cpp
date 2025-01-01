/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <bygg/HTML/element.hpp>

bygg::HTML::Element& bygg::HTML::Element::operator=(const bygg::HTML::Element& element) {
    this->set(element.get_tag(), element.properties, element.get_data(), element.type);
    return *this;
}

bygg::HTML::Element& bygg::HTML::Element::operator+=(const bygg::string_type& data) {
    this->data += data;
    return *this;
}

bool bygg::HTML::Element::operator==(const bygg::HTML::Element& element) const {
    return this->tag == element.get_tag() && this->properties == element.properties && this->data == element.get_data() && this->type == element.type;
}

bool bygg::HTML::Element::operator!=(const bygg::HTML::Element& element) const {
    return this->tag != element.get_tag() || this->properties != element.properties || this->data != element.get_data() || this->type != element.type;
}

void bygg::HTML::Element::set(const bygg::string_type& tag, const Properties& properties, const bygg::string_type& data, const Type type) {
    this->set_tag(tag);
    this->set_properties(properties);
    this->set_data(data);
    this->set_type(type);
}

void bygg::HTML::Element::set(const Tag tag, const Properties& properties, const bygg::string_type& data) {
    this->set_tag(tag);
    this->set_properties(properties);
    this->set_data(data);
}

void bygg::HTML::Element::set_tag(const bygg::string_type& tag) {
    this->tag = tag;
}

void bygg::HTML::Element::set_tag(const Tag tag) {
    std::pair<bygg::string_type, bygg::HTML::Type> resolved{resolve_tag(tag)};
    this->tag = resolved.first;
    this->type = resolved.second;
}

void bygg::HTML::Element::set_data(const bygg::string_type& data) {
    this->data = data;
}

void bygg::HTML::Element::set_type(const Type type) {
    this->type = type;
}

void bygg::HTML::Element::set_properties(const Properties& properties) {
    this->properties = properties;
}

bygg::string_type bygg::HTML::Element::get(const Formatting formatting, const bygg::integer_type tabc) const {
    bygg::string_type ret{};

    if (this->type == bygg::HTML::Type::Text_No_Formatting) {
        return this->data;
    } else if (this->type == bygg::HTML::Type::Text) {
        for (size_type i{0}; i < tabc; i++) {
            ret += "\t";
        }

        return ret + this->data;
    }

    if (formatting == bygg::HTML::Formatting::Pretty) {
        for (size_type i{0}; i < tabc; i++) {
            ret += "\t";
        }
    }

    if (this->type == bygg::HTML::Type::Closing) {
        ret += "</" + this->tag;
    } else {
        ret += "<" + this->tag;
    }

    for (const Property& it : this->properties.get_properties()) {
        if (it.get_key().empty() || it.get_value().empty()) {
            continue;
        }

        ret += " " + it.get();
    }

    if (this->type != bygg::HTML::Type::Standalone && this->type != bygg::HTML::Type::Closing) {
        ret += ">";
    }

    if (this->type == bygg::HTML::Type::Data) {
        ret += this->data + "</" + this->tag + ">";
    } else if (this->type == bygg::HTML::Type::Standalone) {
        ret += this->data + "/>";
    } else if (this->type == bygg::HTML::Type::Closing) {
        ret += ">";
    }

    if (formatting == bygg::HTML::Formatting::Pretty || formatting == bygg::HTML::Formatting::Newline) {
        ret += "\n";
    }

    return ret;
}

bygg::string_type bygg::HTML::Element::get_tag() const {
    return this->tag;
}

bygg::string_type bygg::HTML::Element::get_data() const {
    return this->data;
}

bygg::HTML::Type bygg::HTML::Element::get_type() const {
    return this->type;
}

bygg::HTML::Properties bygg::HTML::Element::get_properties() const {
    return this->properties;
}

bool bygg::HTML::Element::empty() const {
    return this->tag.empty() && this->data.empty() && this->properties.empty();
}

void bygg::HTML::Element::clear() {
    this->tag.clear();
    this->data.clear();
    this->properties.clear();
}