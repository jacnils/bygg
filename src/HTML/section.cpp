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

bygg::HTML::Section::variant_t bygg::HTML::Section::operator[](const bygg::integer_type& index) const {
    if (this->members.size() <= index) {
        throw out_of_range("Index out of range");
    }

    return this->members.at(index);
}

bygg::HTML::Section::variant_t& bygg::HTML::Section::operator[](const bygg::integer_type& index) {
    return this->get_any(index);
}

bool bygg::HTML::Section::operator==(const bygg::HTML::Section& section) const {
    return this->tag == section.tag &&
        this->properties == section.properties &&
        this->members == section.members;
}

bool bygg::HTML::Section::operator==(const bygg::HTML::Element& element) const {
    return std::any_of(this->get_elements().begin(), this->get_elements().end(),
                   [&element](const bygg::HTML::Element& it) {
                       return it.get() == element.get();
                   });
}

bool bygg::HTML::Section::operator!=(const bygg::HTML::Section& section) const {
    return this->tag != section.tag || this->properties != section.properties || this->members != section.members;
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
    this->members.insert(this->members.begin(), element);
}

void bygg::HTML::Section::push_front(const Section& section) {
    this->members.insert(this->members.begin(), section);
}

void bygg::HTML::Section::push_back(const Element& element) {
    this->members.push_back(element);
}

void bygg::HTML::Section::push_back(const Section& section) {
    this->members.push_back(section);
}

void bygg::HTML::Section::push_back(const Properties& properties) {
    for (const auto& it : properties) {
        this->properties.push_back(it);
    }
}

void bygg::HTML::Section::push_back(const Property &property) {
    this->properties.push_back(property);
}

void bygg::HTML::Section::push_back(const ElementList& elements) {
    for (const auto& it : elements) {
        this->push_back(it);
    }
}

void bygg::HTML::Section::push_back(const SectionList& sections) {
    for (const auto& it : sections) {
        this->push_back(it);
    }
}


void bygg::HTML::Section::erase(const size_type index) {
    if (this->members.size() <= index) {
        throw out_of_range("Index out of range");
    }

    this->members.erase(this->members.begin() + static_cast<long>(index));
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
    std::size_t i{0};
    for (const auto& it : this->members) {
        if (std::holds_alternative<Section>(it) && i == index) {
            throw invalid_argument("Index already occupied by a section");
        }
        ++i;
    }

    this->members.push_back(element);
}

void bygg::HTML::Section::insert(const size_type index, const Section& section) {
    this->members.insert(this->members.begin() + static_cast<long>(index), section);
}

bygg::HTML::Element bygg::HTML::Section::at(const size_type index) const {
    if (this->members.size() <= index) {
        throw out_of_range("Index out of range");
    }

    for (size_type i{0}; i < this->members.size(); i++) {
        if (i == index) {
            if (std::holds_alternative<Element>(this->members.at(i))) {
                return std::get<Element>(this->members.at(i));
            }
        }
    }

    throw out_of_range("Index does not contain an element");
}

bygg::HTML::Element& bygg::HTML::Section::at(const size_type index) {
    if (this->members.size() <= index) {
        throw out_of_range("Index out of range");
    }

    for (size_type i{0}; i < this->members.size(); i++) {
        if (i == index) {
            if (std::holds_alternative<Element>(this->members.at(i))) {
                return std::get<Element>(this->members.at(i));
            }
        }
    }

    throw out_of_range("Index does not contain an element");
}

bygg::HTML::Section bygg::HTML::Section::at_section(const size_type index) const {
    if (this->members.size() <= index) {
        throw out_of_range("Index out of range");
    }

    for (size_type i{0}; i < this->members.size(); i++) {
        if (i == index) {
            if (std::holds_alternative<Section>(this->members.at(i))) {
                return std::get<Section>(this->members.at(i));
            }
        }
    }

    throw out_of_range("Index does not contain a section");
}

bygg::HTML::Section& bygg::HTML::Section::at_section(const size_type index) {
    if (this->members.size() <= index) {
        throw out_of_range("Index out of range");
    }

    for (size_type i{0}; i < this->members.size(); i++) {
        if (i == index) {
            if (std::holds_alternative<Section>(this->members.at(i))) {
                return std::get<Section>(this->members.at(i));
            }
        }
    }

    throw out_of_range("Index does not contain a section");
}

bygg::size_type bygg::HTML::Section::find(const Element& element, const size_type begin, const FindParameters params) const {
    if ((params & FindParameters::Only_Sections)) {
        throw invalid_argument("Search parameters must not include FindParameters::Only_Sections");
    }

    size_type index{0};
    for (const auto& it : this->members) {
        if (std::holds_alternative<Element>(it) && std::get<Element>(it) == element && index >= begin) {
            return index;
        }
        ++index;
    }

    return npos;
}

bygg::size_type bygg::HTML::Section::find(const Section& section, const size_type begin, const FindParameters params) const {
    if ((params & FindParameters::Only_Elements)) {
        throw invalid_argument("Search parameters must not include FindParameters::Only_Elements");
    }

    size_type index{0};
    for (const auto& it : this->members) {
        if (std::holds_alternative<Section>(it) && std::get<Section>(it) == section && index >= begin) {
            return index;
        }
        ++index;
    }

    return npos;
}

bygg::size_type bygg::HTML::Section::find(const bygg::string_type& str, const size_type begin, const FindParameters params) const {
    if ((params & FindParameters::Only_Sections) && (params & FindParameters::Only_Elements)) {
        throw invalid_argument("Search parameters must not include both FindParameters::Only_Sections and FindParameters::Only_Elements");
    }

    if (!(params & FindParameters::Only_Sections)) {
        size_type index{};
        for (const auto& it : this->members) {
            if (!std::holds_alternative<Element>(it)) {
                ++index;
                continue;
            }

            if (index >= begin) {
                const Element element = std::get<Element>(it);

                if (((element.get_tag() == str && (params & FindParameters::Search_Tag)) ||
                    (element.get_data() == str && (params & FindParameters::Search_Data)) ||
                    (element.get() == str && (params & FindParameters::Search_Deserialized)) ||
                    (element.get_tag().find(str) != string_type::npos && !(params & FindParameters::Exact) && (params & FindParameters::Search_Tag)) ||
                    (element.get_data().find(str) != string_type::npos && !(params & FindParameters::Exact) && (params & FindParameters::Search_Data)) ||
                    (element.get().find(str) != string_type::npos && !(params & FindParameters::Exact) && (params & FindParameters::Search_Deserialized)))) {
                    return index;
                }
            }

            ++index;
        }
    }

    if (!(params & FindParameters::Only_Elements)) {
        size_type index{};
        for (const auto& it : this->members) {
            if (!std::holds_alternative<Section>(it)) {
                ++index;
                continue;
            }

            if (index >= begin) {
                const Section section = std::get<Section>(it);

                if (((section.get_tag() == str && (params & FindParameters::Search_Tag)) ||
                    (section.get() == str && (params & FindParameters::Search_Deserialized)) ||
                    (section.get_tag().find(str) != string_type::npos && !(params & FindParameters::Exact) && (params & FindParameters::Search_Tag)) ||
                    (section.get().find(str) != string_type::npos && !(params & FindParameters::Exact) && (params & FindParameters::Search_Deserialized)))) {
                    return index;
                }
            }

            ++index;
        }
    }

    return npos;
}

bygg::size_type bygg::HTML::Section::find(const bygg::HTML::Tag tag, const size_type begin, const FindParameters params) const {
    if ((params & FindParameters::Only_Sections) && (params & FindParameters::Only_Elements)) {
        throw invalid_argument("Search parameters must not include both FindParameters::Only_Sections and FindParameters::Only_Elements");
    }

    if (!(params & FindParameters::Only_Sections)) {
        size_type index{};

        for (const auto& it : this->members) {
            if (!std::holds_alternative<Element>(it)) {
                ++index;
                continue;
            }

            if (index >= begin) {
                const Element element = std::get<Element>(it);

                if (element.get_tag() == resolve_tag(tag).first) {
                    return index;
                }
            }

            ++index;
        }
    }

    if (!(params & FindParameters::Only_Elements)) {
        size_type index{};

        for (const auto& it : this->members) {
            if (!std::holds_alternative<Section>(it)) {
                ++index;
                continue;
            }

            if (index >= begin) {
                const Section section = std::get<Section>(it);

                if (section.get_tag() == resolve_tag(tag).first) {
                    return index;
                }
            }

            ++index;
        }
    }

    return npos;
}

bygg::size_type bygg::HTML::Section::find(const bygg::HTML::Properties& properties, const size_type begin, const FindParameters params) const {
    if (!(params & FindParameters::Search_Properties)) {
        throw invalid_argument("Search parameters must include FindParameters::Search_Properties");
    }
    if ((params & FindParameters::Only_Sections) && (params & FindParameters::Only_Elements)) {
        throw invalid_argument("Search parameters must not include both FindParameters::Only_Sections and FindParameters::Only_Elements");
    }

    if (!(params & FindParameters::Only_Sections)) {
        size_type index{0};

        for (const auto& it : this->members) {
            if (!std::holds_alternative<Element>(it)) {
                ++index;
                continue;
            }

            if (index >= begin) {
                const Element element = std::get<Element>(it);

                if (element.get_properties() == properties) {
                    return index;
                }
            }

            ++index;
        }
    }

    if (!(params & FindParameters::Only_Elements)) {
        size_type index{};

        for (const auto& it : this->members) {
            if (!std::holds_alternative<Section>(it)) {
                ++index;
                continue;
            }

            if (index >= begin) {
                const Section section = std::get<Section>(it);

                if (section.get_properties() == properties) {
                    return index;
                }
            }

            ++index;
        }
    }

    return npos;
}

bygg::size_type bygg::HTML::Section::find(const bygg::HTML::Property& property, const size_type begin, const FindParameters params) const {
    if (!(params & FindParameters::Search_Properties)) {
        throw invalid_argument("Search parameters must include FindParameters::Search_Properties");
    }
    if ((params & FindParameters::Only_Sections) && (params & FindParameters::Only_Elements)) {
        throw invalid_argument("Search parameters must not include both FindParameters::Only_Sections and FindParameters::Only_Elements");
    }

    if (!(params & FindParameters::Only_Sections)) {
        size_type index{};
        for (const auto& it : this->members) {
            if (!std::holds_alternative<Element>(it)) {
                ++index;
                continue;
            }

            if (index >= begin) {
                const Element element = std::get<Element>(it);

                if (element.get_properties().find(property) != npos) {
                    return index;
                }
            }

            ++index;
        }
    }

    if (!(params & FindParameters::Only_Elements)) {
        for (size_type i{0}; i < this->members.size(); i++) {
            if (!std::holds_alternative<Section>(this->members.at(i))) {
                ++i;
                continue;
            }

            if (i >= begin) {
                const Section section = std::get<Section>(this->members.at(i));

                if (section.get_properties().find(property) != npos) {
                    return i;
                }
            }
        }
    }

    return npos;
}

bygg::HTML::Element bygg::HTML::Section::front() const {
    if (this->members.empty()) {
        throw out_of_range("Index out of range");
    }

    if (std::holds_alternative<Element>(this->members.front())) {
        return std::get<Element>(this->members.front());
    }

    throw out_of_range("Front does not contain an element");
}

bygg::HTML::Element& bygg::HTML::Section::front() {
    if (this->members.empty()) {
        throw out_of_range("Index out of range");
    }

    if (std::holds_alternative<Element>(this->members.front())) {
        return std::get<Element>(this->members.front());
    }

    throw out_of_range("Front does not contain an element");
}

bygg::HTML::Section bygg::HTML::Section::front_section() const {
    if (this->members.empty()) {
        throw out_of_range("Index out of range");
    }

    if (std::holds_alternative<Section>(this->members.front())) {
        return std::get<Section>(this->members.front());
    }

    throw out_of_range("Front does not contain a section");
}

bygg::HTML::Section& bygg::HTML::Section::front_section() {
    if (this->members.empty()) {
        throw out_of_range("Index out of range");
    }

    if (std::holds_alternative<Section>(this->members.front())) {
        return std::get<Section>(this->members.front());
    }

    throw out_of_range("Front does not contain a section");
}

bygg::HTML::Element bygg::HTML::Section::back() const {
    if (this->members.empty()) {
        throw out_of_range("Index out of range");
    }

    if (std::holds_alternative<Element>(this->members.back())) {
        return std::get<Element>(this->members.back());
    }

    if (std::holds_alternative<Section>(this->members.back())) {
        throw out_of_range("Back does not contain an element");
    }

    throw out_of_range("Back does not contain an element");
}

bygg::HTML::Element& bygg::HTML::Section::back() {
    if (this->members.empty()) {
        throw out_of_range("Index out of range");
    }

    if (std::holds_alternative<Element>(this->members.back())) {
        return std::get<Element>(this->members.back());
    }

    throw out_of_range("Back does not contain an element");
}

bygg::HTML::Section bygg::HTML::Section::back_section() const {
    if (this->members.empty()) {
        throw out_of_range("Index out of range");
    }

    if (std::holds_alternative<Section>(this->members.back())) {
        return std::get<Section>(this->members.back());
    }

    throw out_of_range("Back does not contain a section");
}

bygg::HTML::Section& bygg::HTML::Section::back_section() {
    if (this->members.empty()) {
        throw out_of_range("Index out of range");
    }

    if (std::holds_alternative<Section>(this->members.back())) {
        return std::get<Section>(this->members.back());
    }

    throw out_of_range("Back does not contain a section");
}

bygg::size_type bygg::HTML::Section::size() const {
    return this->members.size();
}

void bygg::HTML::Section::clear() {
    this->tag.clear();
    this->properties.clear();
    this->members.clear();
    this->sections.clear();
    this->elements.clear();
}

bool bygg::HTML::Section::empty() const {
    return this->members.empty();
}

bygg::HTML::ElementList bygg::HTML::Section::get_elements() const {
    bygg::HTML::ElementList ret{};
    for (const auto& it : this->members) {
        if (std::holds_alternative<Element>(it)) {
            ret.push_back(std::get<Element>(it));
        }
    }
    return ret;
}

bygg::HTML::SectionList bygg::HTML::Section::get_sections() const {
    bygg::HTML::SectionList ret{};

    for (const auto& it : this->members) {
        if (std::holds_alternative<Section>(it)) {
            ret.push_back(std::get<Section>(it));
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
            if (c_sect->tag.empty() && c_sect->properties.empty() && c_sect->members.empty()) {
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

        while (c_entry.index < c_sect->members.size()) {
            const auto& member = c_sect->members[c_entry.index];

            if (std::holds_alternative<Element>(member)) {
                const auto& element = std::get<Element>(member);
                ret << element.get(formatting, c_sect->tag.empty() ? c_tabc : ++c_tabc);
                c_entry.index++;
                processed = true;
                break;
            } else if (std::holds_alternative<Section>(member)) {
                const auto& section = std::get<Section>(member);
                s_stack.push({&section, c_sect->tag.empty() ? c_tabc : ++c_tabc, false, 0});
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
    if (this->members.size() <= index1 || this->members.size() <= index2) {
        throw out_of_range("Index out of range");
    }

    std::swap(this->members[index1], this->members[index2]);
}

void bygg::HTML::Section::swap(const Element& element1, const Element& element2) {
    this->swap(this->find(element1), this->find(element2));
}

void bygg::HTML::Section::swap(const Section& section1, const Section& section2) {
    this->swap(this->find(section1), this->find(section2));
}