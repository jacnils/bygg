/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <bygg/except.hpp>
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

void bygg::HTML::Element::set(const bygg::string_type& tag, const Properties& properties, const bygg::string_type& data, const Type type, ElementParameters params) {
    this->set_tag(tag);
    this->set_properties(properties);
    this->set_data(data);
    this->set_type(type);
    this->set_params(params);
}

void bygg::HTML::Element::set(const Tag tag, const Properties& properties, const bygg::string_type& data, ElementParameters params) {
    this->set_tag(tag);
    this->set_properties(properties);
    this->set_data(data);
    this->set_params(params);
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

void bygg::HTML::Element::set_params(ElementParameters params) {
    this->params = params;
}

bygg::string_type bygg::HTML::Element::get(const Formatting formatting, const bygg::integer_type tabc) const {
    bygg::string_type ret{};

    static const std::vector<std::pair<ElementParameters, ElementParameters>> invalid_combinations{
        std::make_pair<ElementParameters, ElementParameters>(ElementParameters::Replace_Newlines, ElementParameters::Erase_Newlines),
        std::make_pair<ElementParameters, ElementParameters>(ElementParameters::Replace_Tabs, ElementParameters::Erase_Tabs),
        std::make_pair<ElementParameters, ElementParameters>(ElementParameters::Replace_Spaces, ElementParameters::Erase_Spaces),
        std::make_pair<ElementParameters, ElementParameters>(ElementParameters::Erase_Spaces, ElementParameters::Erase_Multi_Spaces),
        std::make_pair<ElementParameters, ElementParameters>(ElementParameters::Replace_Left_Brackets, ElementParameters::Erase_Left_Brackets),
        std::make_pair<ElementParameters, ElementParameters>(ElementParameters::Replace_Right_Brackets, ElementParameters::Erase_Right_Brackets),
        std::make_pair<ElementParameters, ElementParameters>(ElementParameters::Replace_Single_Quotes, ElementParameters::Erase_Single_Quotes),
        std::make_pair<ElementParameters, ElementParameters>(ElementParameters::Replace_Double_Quotes, ElementParameters::Erase_Double_Quotes)
    };

    for (const auto& it : invalid_combinations) {
        if (this->params & it.first && this->params & it.second) {
            throw bygg::invalid_argument("You cannot both erase and replace the same character.");
        }
    }

    const auto remove_necessary = [this](string_type str) -> string_type {
        string_type string = std::move(str);

        for (const auto& it :
            {
                std::make_pair<string_type, bool>(string_type("\t"), this->params & ElementParameters::Erase_Tabs),
                std::make_pair<string_type, bool>(string_type(" "), this->params & ElementParameters::Erase_Spaces),
                std::make_pair<string_type, bool>(string_type("\n"), this->params & ElementParameters::Erase_Newlines),
                std::make_pair<string_type, bool>(string_type("<"), this->params & ElementParameters::Erase_Left_Brackets),
                std::make_pair<string_type, bool>(string_type(">"), this->params & ElementParameters::Erase_Right_Brackets),
                std::make_pair<string_type, bool>(string_type("'"), this->params & ElementParameters::Erase_Single_Quotes),
                std::make_pair<string_type, bool>(string_type("\""), this->params & ElementParameters::Erase_Double_Quotes)
            })
        {
            if (it.second) {
                size_type pos{};
                while ((pos = string.find(it.first, pos)) != string_type::npos) {
                    string.erase(pos, it.first.length());
                }
            }
        }

        if (this->params & ElementParameters::Erase_Multi_Spaces) {
            for (size_type i{0}; i < string.length(); i++) {
                if (string[i] == ' ') {
                    size_type j{i + 1};
                    while (j < string.length() && string[j] == ' ') {
                        string.erase(j, 1);
                    }
                }
            }

            if (string.front() == ' ') {
                string.erase(0, 1);
            }
            if (string.back() == ' ') {
                string.pop_back();
            }
        }

        for (const auto& it : {
                std::make_tuple(string_type("\n"), string_type("&#10;"), this->params & ElementParameters::Replace_Newlines),
                std::make_tuple(string_type("\t"), string_type("&#9;"), this->params & ElementParameters::Replace_Tabs),
                std::make_tuple(string_type("<"), string_type("&lt;"), this->params & ElementParameters::Replace_Left_Brackets),
                std::make_tuple(string_type(">"), string_type("&gt;"), this->params & ElementParameters::Replace_Right_Brackets),
                std::make_tuple(string_type("'"), string_type("&apos;"), this->params & ElementParameters::Replace_Single_Quotes),
                std::make_tuple(string_type("\""), string_type("&quot;"), this->params & ElementParameters::Replace_Double_Quotes),
                std::make_tuple(string_type(" "), string_type("&nbsp;"), this->params & ElementParameters::Replace_Spaces),
            })
        {
            size_type pos{};
            if (std::get<2>(it)) {
                while ((pos = string.find(std::get<0>(it), pos)) != string_type::npos) {
                    string.replace(pos, std::get<0>(it).length(), std::get<1>(it));
                }
            }
        }

        return string;
    };

    string_type data_string = remove_necessary(this->data);

    if (this->type == bygg::HTML::Type::Text_No_Formatting) {
        return data_string;
    } else if (this->type == bygg::HTML::Type::Text) {
        for (size_type i{0}; i < tabc; i++) {
            ret += "\t";
        }

        return ret + data_string;
    }

    if (formatting == bygg::HTML::Formatting::Pretty) {
        for (size_type i{0}; i < tabc; i++) {
            ret += "\t";
        }
    }

    if (this->type == bygg::HTML::Type::Closing && !this->tag.empty()) {
        ret += "</" + this->tag;
    } else if (!this->tag.empty()) {
        ret += "<" + this->tag;
    }

    for (const Property& it : this->properties.get_properties()) {
        if (it.get_key().empty() || it.get_value().empty() || this->tag.empty()) {
            continue;
        }

        ret += " " + it.get();
    }

    if (this->type != bygg::HTML::Type::Standalone && this->type != bygg::HTML::Type::Closing && !this->tag.empty()) {
        ret += ">";
    }

    if (this->type == bygg::HTML::Type::Data && !this->tag.empty()) {
        ret += data_string + "</" + this->tag + ">";
    } else if (this->type == bygg::HTML::Type::Standalone && !this->tag.empty()) {
        ret += data_string + "/>";
    } else if (this->type == bygg::HTML::Type::Closing && !this->tag.empty()) {
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

bygg::HTML::ElementParameters bygg::HTML::Element::get_params() const {
    return this->params;
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
    this->params = _default_element_parameters;
}