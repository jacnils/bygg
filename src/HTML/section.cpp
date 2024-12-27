/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <algorithm>
#include <sstream>
#include <stack>

#include <bygg/except.hpp>
#include <bygg/HTML/tag.hpp>
#include <bygg/HTML/section.hpp>

bygg::HTML::Section& bygg::HTML::Section::operator=(const bygg::HTML::Section& section) = default;

bygg::HTML::Section& bygg::HTML::Section::operator+=(const bygg::HTML::Element& element) {
    this->push_back(element);
    return *this;
}

bygg::HTML::Section& bygg::HTML::Section::operator+=(const bygg::HTML::Section& section) {
    this->push_back(section);
    return *this;
}

bygg::HTML::Element bygg::HTML::Section::operator[](const bygg::integer_type& index) const {
    return this->at(index);
}

bygg::HTML::Element& bygg::HTML::Section::operator[](const bygg::integer_type& index) {
    return this->at(index);
}

std::unordered_map<bygg::string_type, bygg::HTML::Element> bygg::HTML::Section::operator[](const bygg::string_type& tag) const {
    std::unordered_map<bygg::string_type, bygg::HTML::Element> ret{};

    for (const Element& it : this->get_elements()) {
        if (it.get_tag() == tag) {
            ret[it.get_data()] = it;
        }
    }

    return ret;
}

std::unordered_map<bygg::string_type, bygg::HTML::Element> bygg::HTML::Section::operator[](const Tag tag) const {
    std::unordered_map<bygg::string_type, bygg::HTML::Element> ret{};

    for (const Element& it : this->get_elements()) {
        if (it.get_tag() == resolve_tag(tag).first) {
            ret[it.get_data()] = it;
        }
    }

    return ret;
}

bool bygg::HTML::Section::operator==(const bygg::HTML::Section& section) const {
    return this->tag == section.tag && this->properties == section.properties && this->elements == section.elements && this->sections == section.sections && this->index == section.index;
}

bool bygg::HTML::Section::operator==(const bygg::HTML::Element& element) const {
    return std::any_of(this->get_elements().begin(), this->get_elements().end(),
                   [&element](const bygg::HTML::Element& it) {
                       return it.get() == element.get();
                   });
}

bool bygg::HTML::Section::operator!=(const bygg::HTML::Section& section) const {
    return this->tag != section.tag || this->properties != section.properties || this->elements != section.elements || this->sections != section.sections || this->index != section.index;
}

bool bygg::HTML::Section::operator!=(const bygg::HTML::Element& element) const {
    return std::any_of(this->get_elements().begin(), this->get_elements().end(), [&element](const Element& it) {
        return it.get() == element.get();
    });
}

void bygg::HTML::Section::set(const bygg::string_type& tag, const Properties& properties) {
    this->tag = tag;
    this->properties = properties;
}

void bygg::HTML::Section::set_tag(const bygg::string_type& tag) {
    this->tag = tag;
}

void bygg::HTML::Section::set_tag(const Tag tag) {
    std::pair<bygg::string_type, bygg::HTML::Type> resolved{resolve_tag(tag)};
    this->tag = resolved.first;
}

void bygg::HTML::Section::set_properties(const Properties& properties) {
    this->properties = properties;
}

void bygg::HTML::Section::set(const Tag tag, const Properties& properties) {
    this->tag = resolve_tag(tag).first;
    this->properties = properties;
}

void bygg::HTML::Section::push_front(const Element& element) {
    for (size_type i{this->index}; i > 0; i--) {
        this->elements[i] = this->elements.at(i - 1);
    }

    this->elements[0] = element;
    this->index++;
}

void bygg::HTML::Section::push_front(const Section& section) {
    for (size_type i{this->index}; i > 0; i--) {
        this->sections.at(i) = this->sections.at(i - 1);
    }

    this->sections[0] = section;
    this->index++;
}

void bygg::HTML::Section::push_back(const Element& element) {
    this->elements[this->index++] = element;
}

void bygg::HTML::Section::push_back(const Section& section) {
    this->sections[this->index++] = section;
}

void bygg::HTML::Section::push_back(const Properties& properties) {
    for (const auto& it : properties) {
        this->properties.push_back(it);
    }
}

void bygg::HTML::Section::push_back(const Property &property) {
    this->properties.push_back(property);
}

void bygg::HTML::Section::push_back(const ElementList &elements) {
    for (const auto& it : elements) {
        this->push_back(it);
    }
}

void bygg::HTML::Section::push_back(const SectionList &sections) {
    for (const auto& it : sections) {
        this->push_back(it);
    }
}


void bygg::HTML::Section::erase(const size_type index) {
    bool erased{false};

    if (this->elements.find(index) != this->elements.end()) {
        this->elements.erase(index);
        erased = true;
    } else if (this->sections.find(index) != this->sections.end()) {
        this->sections.erase(index);
        erased = true;
    }

    if (!erased) {
        throw out_of_range("Index out of range");
    }
}

void bygg::HTML::Section::erase(const Section& section) {
    for (size_type i{0}; i < this->size(); i++) {
        const Section it = this->get_sections().at(i);

        if (it.get() == section.get()) {
            this->erase(i);
            return;
        }
    }

    throw out_of_range("Section not found");
}

void bygg::HTML::Section::erase(const Element& element) {
    for (size_type i{0}; i < this->size(); i++) {
        const Element it = this->get_elements().at(i);

        if (it.get() == element.get()) {
            this->erase(i);
            return;
        }
    }

    throw out_of_range("Element not found");
}

void bygg::HTML::Section::insert(const size_type index, const Element& element) {
    if (this->sections.find(index) != this->sections.end()) {
        throw invalid_argument("Index already occupied by a section");
    } else {
        this->elements[index] = element;
    }

    this->index = std::max(this->index, index) + 1;
}

void bygg::HTML::Section::insert(const size_type index, const Section& section) {
    this->sections[index] = section;
    this->index = std::max(this->index, index) + 1;
}

bygg::HTML::Element bygg::HTML::Section::at(const size_type index) const {
    if (this->elements.find(index) != this->elements.end()) {
        return this->elements.at(index);
    }

    throw out_of_range("Index out of range");
}

bygg::HTML::Element& bygg::HTML::Section::at(const size_type index) {
    if (this->elements.find(index) != this->elements.end()) {
        return this->elements.at(index);
    }

    throw out_of_range("Index out of range");
}

bygg::HTML::Section bygg::HTML::Section::at_section(const size_type index) const {
    if (this->sections.find(index) != this->sections.end()) {
        return this->sections.at(index);
    }

    throw out_of_range("Index out of range");
}

bygg::HTML::Section& bygg::HTML::Section::at_section(const size_type index) {
    if (this->sections.find(index) != this->sections.end()) {
        return this->sections.at(index);
    }

    throw out_of_range("Index out of range");
}

bygg::size_type bygg::HTML::Section::find(const Element& element, const FindParameters params) const {
    if ((params & FindParameters::Only_Sections)) {
        throw invalid_argument("Search parameters must not include FindParameters::Only_Sections");
    }

    for (const auto& it : this->elements) {
        if (it.second == element) {
            return it.first;
        }
    }

    return npos;
}

bygg::size_type bygg::HTML::Section::find(const Section& section, const FindParameters params) const {
    if ((params & FindParameters::Only_Elements)) {
        throw invalid_argument("Search parameters must not include FindParameters::Only_Elements");
    }

    for (const auto& it : this->sections) {
        if (it.second == section) {
            return it.first;
        }
    }

    return npos;
}

bygg::size_type bygg::HTML::Section::find(const bygg::string_type& str, const FindParameters params) const {
    if ((params & FindParameters::Only_Sections) && (params & FindParameters::Only_Elements)) {
        throw invalid_argument("Search parameters must not include both FindParameters::Only_Sections and FindParameters::Only_Elements");
    }

    if (!(params & FindParameters::Only_Sections)) {
        for (const auto& it : this->elements) {
            if ((it.second.get_tag() == str && (params & FindParameters::Search_Tag)) ||
                (it.second.get_data() == str && (params & FindParameters::Search_Data)) ||
                (it.second.get() == str && (params & FindParameters::Search_Deserialized)) ||
                (it.second.get_tag().find(str) != string_type::npos && !(params & FindParameters::Exact) && (params & FindParameters::Search_Tag)) ||
                (it.second.get_data().find(str) != string_type::npos && !(params & FindParameters::Exact) && (params & FindParameters::Search_Data)) ||
                (it.second.get().find(str) != string_type::npos && !(params & FindParameters::Exact) && (params & FindParameters::Search_Deserialized))) {
                return it.first;
                }
        }
    }

    if (!(params & FindParameters::Only_Elements)) {
        for (const auto& it : this->sections) {
            if ((it.second.get_tag() == str && (params & FindParameters::Search_Tag)) ||
                (it.second.get() == str && (params & FindParameters::Search_Deserialized)) ||
                (it.second.get_tag().find(str) != string_type::npos && !(params & FindParameters::Exact) && (params & FindParameters::Search_Tag)) ||
                (it.second.get().find(str) != string_type::npos && !(params & FindParameters::Exact) && (params & FindParameters::Search_Deserialized))) {
                return it.first;
            }
        }
    }

    return npos;
}

bygg::size_type bygg::HTML::Section::find(const bygg::HTML::Tag tag, const FindParameters params) const {
    if ((params & FindParameters::Only_Sections) && (params & FindParameters::Only_Elements)) {
        throw invalid_argument("Search parameters must not include both FindParameters::Only_Sections and FindParameters::Only_Elements");
    }

    if (!(params & FindParameters::Only_Sections)) {
        for (const auto& it : this->elements) {
            if (it.second.get_tag() == resolve_tag(tag).first) {
                return it.first;
            }
        }
    }
    if (!(params & FindParameters::Only_Elements)) {
        for (const auto& it : this->sections) {
            if (it.second.get_tag() == resolve_tag(tag).first) {
                return it.first;
            }
        }
    }

    return npos;
}

bygg::size_type bygg::HTML::Section::find(const bygg::HTML::Properties& properties, const FindParameters params) const {
    if (!(params & FindParameters::Search_Properties)) {
        throw invalid_argument("Search parameters must include FindParameters::Search_Properties");
    }
    if ((params & FindParameters::Only_Sections) && (params & FindParameters::Only_Elements)) {
        throw invalid_argument("Search parameters must not include both FindParameters::Only_Sections and FindParameters::Only_Elements");
    }

    if (!(params & FindParameters::Only_Sections)) {
        for (const auto& it : this->elements) {
            if (it.second.get_properties() == properties) {
                return it.first;
            }
        }
    }

    if (!(params & FindParameters::Only_Elements)) {
        for (const auto& it : this->sections) {
            if (it.second.get_properties() == properties) {
                return it.first;
            }
        }
    }

    return npos;
}

bygg::size_type bygg::HTML::Section::find(const bygg::HTML::Property& property, const FindParameters params) const {
    if (!(params & FindParameters::Search_Properties)) {
        throw invalid_argument("Search parameters must include FindParameters::Search_Properties");
    }
    if ((params & FindParameters::Only_Sections) && (params & FindParameters::Only_Elements)) {
        throw invalid_argument("Search parameters must not include both FindParameters::Only_Sections and FindParameters::Only_Elements");
    }

    if (!(params & FindParameters::Only_Sections)) {
        for (const auto& it : this->elements) {
            if (it.second.get_properties().find(property) != Property::npos) {
                return it.first;
            }
        }
    }
    if (!(params & FindParameters::Only_Elements)) {
        for (const auto& it : this->sections) {
            if (it.second.get_properties().find(property) != Property::npos) {
                return it.first;
            }
        }
    }

    return npos;
}

bygg::HTML::Element bygg::HTML::Section::front() const {
    if (this->elements.find(0) != this->elements.end()) {
        return this->elements.at(0);
    }

    throw out_of_range("Index out of range");
}

bygg::HTML::Element& bygg::HTML::Section::front() {
    if (this->elements.find(0) != this->elements.end()) {
        return this->elements.at(0);
    }

    throw out_of_range("Index out of range");
}

bygg::HTML::Section bygg::HTML::Section::front_section() const {
    if (this->sections.find(0) != this->sections.end()) {
        return this->sections.at(0);
    }

    throw out_of_range("Index out of range");
}

bygg::HTML::Section& bygg::HTML::Section::front_section() {
    if (this->sections.find(0) != this->sections.end()) {
        return this->sections.at(0);
    }

    throw out_of_range("Index out of range");
}

bygg::HTML::Element bygg::HTML::Section::back() const {
    if (this->elements.find(this->index - 1) != this->elements.end()) {
        return this->elements.at(this->index - 1);
    }

    throw out_of_range("Index out of range");
}

bygg::HTML::Element& bygg::HTML::Section::back() {
    if (this->elements.find(this->index - 1) != this->elements.end()) {
        return this->elements.at(this->index - 1);
    }

    throw out_of_range("Index out of range");
}

bygg::HTML::Section bygg::HTML::Section::back_section() const {
    if (this->sections.find(this->index - 1) != this->sections.end()) {
        return this->sections.at(this->index - 1);
    }

    throw out_of_range("Index out of range");
}

bygg::HTML::Section& bygg::HTML::Section::back_section() {
    if (this->sections.find(this->index - 1) != this->sections.end()) {
        return this->sections.at(this->index - 1);
    }

    throw out_of_range("Index out of range");
}

bygg::size_type bygg::HTML::Section::size() const {
    return this->index;
}

void bygg::HTML::Section::clear() {
    this->tag.clear();
    this->properties.clear();
    this->elements.clear();
    this->sections.clear();
    this->index = 0;
}

bool bygg::HTML::Section::empty() const {
    return this->index == 0;
}

bygg::HTML::ElementList bygg::HTML::Section::get_elements() const {
    bygg::HTML::ElementList ret{};
    ret.reserve(this->index);
    for (size_type i{0}; i < this->index; i++) {
        if (this->elements.find(i) != this->elements.end()) {
            ret.push_back(this->elements.at(i));
        }
    }
    return ret;
}

bygg::HTML::SectionList bygg::HTML::Section::get_sections() const {
    bygg::HTML::SectionList ret{};
    ret.reserve(this->index);

    for (size_type i{0}; i < this->index; i++) {
        if (this->sections.find(i) != this->sections.end()) {
            ret.push_back(this->sections.at(i));
        }
    }

    return ret;
}

bygg::string_type bygg::HTML::Section::get(const Formatting formatting, const bygg::integer_type tabc) const {
    std::ostringstream ret{};

    struct Entry {
        const Section* section{nullptr};
        bygg::integer_type tabc{tabc};
        bool processed{false};
        size_type index{};
    };

    std::stack<Entry> s_stack{};
    s_stack.push({this, tabc, false});

    while (!s_stack.empty()) {
        Entry& c_entry{s_stack.top()};
        const Section* c_sect{c_entry.section};
        bygg::integer_type c_tabc{c_entry.tabc};

        if (!c_entry.processed) {
            if (c_sect->tag.empty() && c_sect->properties.empty() && c_sect->sections.empty() && c_sect->elements.empty()) {
                s_stack.pop();
                continue;
            }

            if (formatting == bygg::HTML::Formatting::Pretty && !c_sect->tag.empty()) {
                for (size_type i{0}; i < c_tabc; i++) {
                    ret << "\t";
                }
            }

            if (!c_sect->tag.empty()) {
                ret << "<" + c_sect->tag;

                for (const Property& it : c_sect->properties.get_properties()) {
                    if (!it.get_key().empty() && !it.get_value().empty()) {
                        ret << " " + it.get_key() + "=\"" + it.get_value() + "\"";
                    }
                }

                ret << ">";

                if (formatting == bygg::HTML::Formatting::Pretty || formatting == bygg::HTML::Formatting::Newline) {
                    ret << "\n";
                }
            }

            c_entry.processed = true;
        }

        bool processed = false;

        while (c_entry.index < c_sect->index) {
            if (c_sect->elements.find(c_entry.index) != c_sect->elements.end()) {

                ret << c_sect->elements.at(c_entry.index).get(formatting, c_sect->tag.empty() ? c_tabc : ++c_tabc);

                c_entry.index++;
                processed = true;
                break;
            } else if (c_sect->sections.find(c_entry.index) != c_sect->sections.end()) {
                s_stack.push({&c_sect->sections.at(c_entry.index), c_sect->tag.empty() ? c_tabc : ++c_tabc, false, 0});
                c_entry.index++;
                processed = true;
                break;
            }
            c_entry.index++;
        }

        if (!processed) {
            if (!c_sect->tag.empty()) {
                if (formatting == bygg::HTML::Formatting::Pretty) {
                    for (size_type i{0}; i < c_tabc; i++) {
                        ret << "\t";
                    }
                }
                ret << "</" + c_sect->tag + ">";

                if (formatting == bygg::HTML::Formatting::Pretty || formatting == bygg::HTML::Formatting::Newline) {
                    ret << "\n";
                }
            }

            s_stack.pop();
        }
    }

    bygg::string_type return_string{ret.str()};

    if (return_string.back() == '\n') {
        return_string.pop_back();
    }

    return return_string;
}

bygg::string_type bygg::HTML::Section::get_tag() const {
    return this->tag;
}

bygg::HTML::Properties bygg::HTML::Section::get_properties() const {
    return this->properties;
}

void bygg::HTML::Section::swap(const size_type index1, const size_type index2) {
    if (this->elements.find(index1) != this->elements.end() && this->elements.find(index2) != this->elements.end()) {
        std::swap(this->elements[index1], this->elements[index2]);
    } else if (this->sections.find(index1) != this->sections.end() && this->sections.find(index2) != this->sections.end()) {
        std::swap(this->sections[index1], this->sections[index2]);
    } else {
        throw out_of_range("Index out of range");
    }
}

void bygg::HTML::Section::swap(const Element& element1, const Element& element2) {
    this->swap(this->find(element1), this->find(element2));
}

void bygg::HTML::Section::swap(const Section& section1, const Section& section2) {
    this->swap(this->find(section1), this->find(section2));
}