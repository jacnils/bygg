/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <bygg/except.hpp>
#include <bygg/HTML/pseudocode_generator.hpp>
#include <functional>

bygg::string_type bygg::HTML::generate_pseudocode(const Section& section, const GeneratorOptions& options) {
    enum class SectionType {
        Section,
        Element,
        Empty,
    };

    const auto tolower = [](const string_type& input) -> string_type {
        string_type ret{};
        for (auto& c : input) {
            ret += static_cast<char>(std::tolower(c));
        }
        return ret;
    };

    const auto escape_invalid = [&options](const string_type& input) -> string_type {
        string_type ret{};

        for (auto& c : input) {
            if (c == '"') {
                ret += "\\\"";
            } else if (c == '\t') {
                if (options.sequence_mode == SequenceMode::Replace) {
                    ret += "\\t";
                } else if (options.sequence_mode == SequenceMode::Remove) {
                    continue;
                }
            } else if (c == '\n') {
                if (options.sequence_mode == SequenceMode::Replace) {
                    ret += "\\n";
                } else if (options.sequence_mode == SequenceMode::Remove) {
                    continue;
                }
            } else if (c == '\r') {
                if (options.sequence_mode == SequenceMode::Replace) {
                    ret += "\\r";
                } else if (options.sequence_mode == SequenceMode::Remove) {
                    continue;
                }
            } else {
                ret += c;
            }
        }

        return ret;
    };

    static const std::unordered_map<bygg::HTML::Type, string_type> type_map {
        {Type::Non_Closed, "bygg::HTML::Type::Non_Closed"},
        {Type::Non_Opened, "bygg::HTML::Type::Non_Opened"},
        {Type::Non_Self_Closing, "bygg::HTML::Type::Non_Self_Closing"},
        {Type::Self_Closing, "bygg::HTML::Type::Self_Closing"},
        {Type::Text, "bygg::HTML::Type::Text"},
        {Type::Text_No_Formatting, "bygg::HTML::Type::Text_No_Formatting"},
    };

    const auto get_element_type = [](const Section& i_section) -> std::unordered_map<size_type, SectionType> {
        std::unordered_map<size_type, SectionType> ret{};

        for (size_type i{0}; i < i_section.size(); i++) {
            try {
                static_cast<void>(i_section.at(i));
                ret[i] = SectionType::Element;
            } catch (bygg::out_of_range&) {
                try {
                    const auto& sub_section = i_section.at_section(i);
                    if (sub_section.size() > 0) {
                        ret[i] = SectionType::Section;
                    } else {
                        ret[i] = SectionType::Empty;
                    }
                } catch (bygg::out_of_range&) {
                    ret[i] = SectionType::Empty;
                }
            }
        }

        return ret;
    };

    const auto get_first_type = [&get_element_type](const Section& i_section) -> SectionType {
        const auto map = get_element_type(i_section);
        for (size_type i{0}; i < i_section.size(); i++) {
            if (map.find(i) != map.end()) {
                return map.at(i);
            }
        }
        return SectionType::Empty;
    };

    string_type pseudocode = "#include <bygg/bygg.hpp>\n\nint main() {\n";
    int tabc = 1;

    const auto append_tabs = [&pseudocode](const int tabc) {
        for (int i{0}; i < tabc; i++) pseudocode += "\t";
    };

    const auto append_properties = [&pseudocode, &escape_invalid](const auto& properties) {
        bool first_passed{false};
        for (const auto& it : properties) {
            if (first_passed) {
                pseudocode += ", ";
            }
            pseudocode += "bygg::HTML::Property{\"" + escape_invalid(it.get_key()) + "\", \"" + escape_invalid(it.get_value()) + "\"}";
            first_passed = true;
        }
    };

    const std::function<void(const Section&, int)> handle_section = [&](const Section& i_section, const int i_tabc) -> void {
        const auto map = get_element_type(i_section);
        for (size_type index = 0; index < i_section.size(); index++) {
            if (map.find(index) != map.end() && map.at(index) == SectionType::Empty) {
                continue;
            }

            append_tabs(i_tabc);
            if (map.find(index) != map.end() && map.at(index) == SectionType::Section) {
                try {
                    if (options.use_tag_enums == false) {
                        throw bygg::invalid_argument{"Invalid tag"};
                    }
                    pseudocode += "bygg::HTML::Section{" + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(i_section.at_section(index).get_tag()))) + ", bygg::HTML::make_properties(";
                } catch (bygg::invalid_argument&) {
                    pseudocode += "bygg::HTML::Section{\"" + escape_invalid(i_section.at_section(index).get_tag()) + "\", bygg::HTML::make_properties(";
                }
                append_properties(i_section.at_section(index).get_properties());

                const auto& _map = get_element_type(i_section.at_section(index));

                SectionType front_type = SectionType::Empty;

                if (_map.find(0) != _map.end()) {
                    front_type = _map.at(0);
                }

                if (front_type == SectionType::Section) {
                    pseudocode += "), bygg::HTML::SectionList {\n";
                    handle_section(i_section.at_section(index), i_tabc + 1);
                } else {
                    pseudocode += "), bygg::HTML::ElementList {\n";

                    // Handle all of the elements in the section
                    bygg::HTML::Section next_section = i_section.at_section(index);
                    for (size_type i{0}; i < next_section.size(); i++) {
                        if (_map.find(i) == _map.end()) {
                            continue;
                        }

                        if (_map.at(i) != SectionType::Element) {
                            continue;
                        }

                        bygg::HTML::Element element;

                        try {
                            element = next_section.at(i);
                        } catch (bygg::out_of_range&) {
                            continue;
                        }

                        append_tabs(i_tabc + 1);
                        try {
                            if (options.use_tag_enums == false) {
                                throw bygg::invalid_argument{"Invalid tag"};
                            }

                            pseudocode += "bygg::HTML::Element{" + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(next_section.at(i).get_tag()))) + ", bygg::HTML::make_properties(";
                            append_properties(next_section.at(i).get_properties());
                            pseudocode += "), \"" + escape_invalid(next_section.at(i).get_data()) + "\"},\n";
                        } catch (bygg::invalid_argument&) {
                            pseudocode += "bygg::HTML::Element{\"" + next_section.at(i).get_tag() + "\", bygg::HTML::make_properties(";
                            append_properties(next_section.at(i).get_properties());
                            pseudocode += "), \"" + escape_invalid(next_section.at(i).get_data()) + "\", " + type_map.at(next_section.at(i).get_type()) + "},\n";
                        }
                    }
                }

                append_tabs(i_tabc);
                pseudocode += "}},\n";
            } else {
                try {
                    if (options.use_tag_enums == false) {
                        throw bygg::invalid_argument{"Invalid tag"};
                    }

                    pseudocode += "bygg::HTML::Element{" + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(i_section.at(index).get_tag()))) + ", bygg::HTML::make_properties(";
                    append_properties(i_section.at(index).get_properties());
                    pseudocode += "), \"" + escape_invalid(i_section.at(index).get_data()) + "\"},\n";
                } catch (bygg::invalid_argument&) {
                    pseudocode += "bygg::HTML::Element{\"" + i_section.at(index).get_tag() + "\", bygg::HTML::make_properties(";
                    append_properties(i_section.at(index).get_properties());
                    pseudocode += "), \"" + escape_invalid(i_section.at(index).get_data()) + "\", " + type_map.at(i_section.at(index).get_type()) + "},\n";
                }
            }
        }
    };

    append_tabs(tabc);

    try {
        if (options.use_tag_enums == false) {
            throw bygg::invalid_argument{"Invalid tag"};
        }
        pseudocode += "bygg::HTML::Section root{" + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(section.get_tag())))  + ", bygg::HTML::make_properties(";
    } catch (bygg::invalid_argument&) {
        pseudocode += "bygg::HTML::Section root{\"" + section.get_tag() + "\", bygg::HTML::make_properties(";
    }

    append_properties(section.get_properties());
    pseudocode += get_first_type(section) == SectionType::Section ? "), bygg::HTML::SectionList {\n" : "), bygg::HTML::ElementList {\n";
    handle_section(section, tabc + 1);
    append_tabs(tabc);
    pseudocode += "}};\n}\n";

    return pseudocode;
}
