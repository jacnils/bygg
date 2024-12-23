/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <bygg/except.hpp>
#include <bygg/HTML/pseudocode_generator.hpp>
#include <functional>

/* This entire function is a massive mess. It started out quite simple, but as I added more and more
 * features to the GeneratorOptions struct, it became much more complex and difficult to read.
 * So, as a TODO, I should refactor this function to make it more readable and maintainable.
 */
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
                    if (!sub_section.empty()) {
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

    string_type pseudocode{};
    int tabc{};

    if (options.include_main) {
        pseudocode = "#include <bygg/bygg.hpp>\n\nint main() {\n";
        tabc = 1;
    }

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
                    pseudocode += "bygg::HTML::Section{" + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(i_section.at_section(index).get_tag())));
                } catch (bygg::invalid_argument&) {
                    pseudocode += "bygg::HTML::Section{\"" + escape_invalid(i_section.at_section(index).get_tag()) + "\"";
                }

                const auto& _section{ i_section.at_section(index) };
                const auto& _properties{ _section.get_properties() };
                if (_properties.size() > 1 || options.use_implicit_property == false) {
                    if (options.use_make_properties) {
                        pseudocode += ", bygg::HTML::make_properties(";
                        append_properties(_properties);
                        pseudocode += ")";
                    } else {
                        pseudocode += ", bygg::HTML::Properties{";
                        append_properties(_properties);
                        pseudocode += "}";
                    }
                } else if (_properties.size() == 1) {
                    pseudocode += ", bygg::HTML::Property{\"" + escape_invalid(_properties.at(0).get_key()) + "\", \"" + escape_invalid(_properties.at(0).get_value()) + "\"}";
                } else if (options.use_empty_properties) {
                    pseudocode += options.use_make_properties ? ", bygg::HTML::make_properties()" : ", bygg::HTML::Properties{}";
                }

                const auto& _map = get_element_type(_section);

                SectionType front_type = SectionType::Empty;

                if (_map.find(0) != _map.end()) {
                    front_type = _map.at(0);
                }

                if (front_type == SectionType::Section) {
                    pseudocode += options.use_lists ? "bygg::HTML::SectionList {\n" : ",\n";

                    handle_section(i_section.at_section(index), i_tabc + 1);
                } else {
                    pseudocode += options.use_lists ? "bygg::HTML::ElementList {\n" : ",\n";

                    // handle all the elements in the section
                    bygg::HTML::Section next_section = i_section.at_section(index);
                    for (size_type i{0}; i < next_section.size(); i++) {
                        if (_map.find(i) == _map.end()) {
                            continue;
                        }

                        if (_map.at(i) != SectionType::Element) {
                            continue;
                        }

                        try {
                            static_cast<void>(next_section.at(i));
                        } catch (bygg::out_of_range&) {
                            continue;
                        }

                        append_tabs(i_tabc + 1);
                        try {
                            if (options.use_tag_enums == false) {
                                throw bygg::invalid_argument{"Invalid tag"};
                            }

                            const auto& properties{next_section.at(i).get_properties()};
                            if (!properties.empty() || options.use_empty_properties) {
                                pseudocode += "bygg::HTML::Element{" + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(next_section.at(i).get_tag())));

                                if (properties.size() == 1 && options.use_implicit_property) {
                                    pseudocode += ", bygg::HTML::Property{\"" + escape_invalid(properties.at(0).get_key()) + "\", \"" + escape_invalid(properties.at(0).get_value()) + "\"}, \"" + escape_invalid(next_section.at(i).get_data()) + "\"},\n";
                                } else {
                                    pseudocode += options.use_make_properties ? ", bygg::HTML::make_properties(" : ", bygg::HTML::Properties{";
                                    append_properties(properties);
                                    pseudocode += options.use_make_properties ? ")," : "},";
                                    pseudocode += " \"" + escape_invalid(next_section.at(i).get_data()) + "\"},\n";
                                }
                            } else {
                                pseudocode += "bygg::HTML::Element{" + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(next_section.at(i).get_tag()))) + ", \"" + escape_invalid(next_section.at(i).get_data()) + "\"},\n";
                            }
                        } catch (bygg::invalid_argument&) {
                            const auto& properties{next_section.at(i).get_properties()};

                            if (!properties.empty() || options.use_empty_properties) {
                                if (properties.size() == 1 && options.use_implicit_property) {
                                    pseudocode += "bygg::HTML::Element{\"" + next_section.at(i).get_tag() + "\", bygg::HTML::Property{\"" + escape_invalid(properties.at(0).get_key()) + "\", \"" + escape_invalid(properties.at(0).get_value()) + "\"}, \"" + escape_invalid(next_section.at(i).get_data()) + "\"},\n";
                                } else {
                                    pseudocode += "bygg::HTML::Element{\"" + next_section.at(i).get_tag() + "\",";
                                    pseudocode += options.use_make_properties ? "bygg::HTML::make_properties(" : "bygg::HTML::Properties{";
                                    append_properties(properties);
                                    pseudocode += "), \"" + escape_invalid(next_section.at(i).get_data()) + "\", " + type_map.at(next_section.at(i).get_type()) + "},\n";
                                }
                            } else {
                                pseudocode += "bygg::HTML::Element{\"" + next_section.at(i).get_tag() + "\", \"" + escape_invalid(next_section.at(i).get_data()) + "\", " + type_map.at(next_section.at(i).get_type()) + "},\n";
                            }
                        }
                    }
                }

                append_tabs(i_tabc);
                pseudocode += options.use_lists ? "}},\n" : "},\n";
            } else {
                try {
                    // automatically catch
                    if (options.use_tag_enums == false) {
                        throw bygg::invalid_argument{"Invalid tag"};
                    }

                    const auto& properties{i_section.at(index).get_properties()};
                    pseudocode += "bygg::HTML::Element{" + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(i_section.at(index).get_tag())));
                    if (properties.size() == 1 && options.use_implicit_property) {
                        pseudocode += ", bygg::HTML::Property{\"" + escape_invalid(properties.at(0).get_key()) + "\", \"" + escape_invalid(properties.at(0).get_value()) + "\"}";
                        continue;
                    } else if (!properties.empty()) {
                        pseudocode += options.use_make_properties ? ", bygg::HTML::make_properties(" : ", bygg::HTML::Properties{";
                        append_properties(properties);
                        pseudocode += options.use_make_properties ? ")" : "}";
                    } else if (options.use_empty_properties) {
                        pseudocode += options.use_make_properties ? ", bygg::HTML::make_properties()" : ", bygg::HTML::Properties{}";
                    }
                    pseudocode += ", \"" + escape_invalid(i_section.at(index).get_data()) + "\"},\n";
                } catch (bygg::invalid_argument&) {
                    const auto& properties{i_section.at(index).get_properties()};
                    if (properties.size() == 1 && options.use_implicit_property) {
                        pseudocode += "bygg::HTML::Element{\"" + i_section.at(index).get_tag() + "\", bygg::HTML::Property{\"" + escape_invalid(properties.at(0).get_key()) + "\", \"" + escape_invalid(properties.at(0).get_value()) + "\"}, \"" + escape_invalid(i_section.at(index).get_data()) + "\"},\n";
                    } else if (!properties.empty()) {
                        pseudocode += "bygg::HTML::Element{\"" + i_section.at(index).get_tag() + "\",";
                        pseudocode += options.use_make_properties ? "bygg::HTML::make_properties(" : "bygg::HTML::Properties{";
                        append_properties(properties);
                        pseudocode += "), \"" + escape_invalid(i_section.at(index).get_data()) + "\", " + type_map.at(i_section.at(index).get_type()) + "},\n";
                    } else if (options.use_empty_properties) {
                        pseudocode += "bygg::HTML::Element{\"" + i_section.at(index).get_tag() + "\", ";
                        pseudocode += options.use_make_properties ? "bygg::HTML::make_properties()" : "bygg::HTML::Properties{}";
                        pseudocode += ", \"" + escape_invalid(i_section.at(index).get_data()) + "\", " + type_map.at(i_section.at(index).get_type()) + "},\n";
                    } else {
                        pseudocode += "bygg::HTML::Element{\"" + i_section.at(index).get_tag() + "\", \"" + escape_invalid(i_section.at(index).get_data()) + "\", " + type_map.at(i_section.at(index).get_type()) + "},\n";
                    }
                }
            }
        }
    };

    append_tabs(tabc);

    try {
        // automatically catch
        if (options.use_tag_enums == false) {
            throw bygg::invalid_argument{"Invalid tag"};
        }

        pseudocode += "bygg::HTML::Section root{" + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(section.get_tag())));
        if (section.get_properties().size() > 1 || options.use_empty_properties) {
            if (options.use_make_properties) {
                pseudocode += ", bygg::HTML::make_properties(";
                append_properties(section.get_properties());
                pseudocode += ")";
            } else {
                pseudocode += ", bygg::HTML::Properties{";
                append_properties(section.get_properties());
                pseudocode += "}";
            }
        } else if (section.get_properties().size() == 1) {
            pseudocode += ", bygg::HTML::Property{\"" + escape_invalid(section.get_properties().at(0).get_key()) + "\", \"" + escape_invalid(section.get_properties().at(0).get_value()) + "\"}";
        } else if (options.use_empty_properties) {
            pseudocode += options.use_make_properties ? ", bygg::HTML::make_properties()" : ", bygg::HTML::Properties{}";
        }
    } catch (bygg::invalid_argument&) {
        pseudocode += "bygg::HTML::Section root{\"" + section.get_tag() + "\"";
        if (section.get_properties().size() > 1 || options.use_empty_properties) {
            if (options.use_make_properties) {
                pseudocode += ", bygg::HTML::make_properties(";
                append_properties(section.get_properties());
                pseudocode += ")";
            } else {
                pseudocode += ", bygg::HTML::Properties{";
                append_properties(section.get_properties());
                pseudocode += "}";
            }
        } else if (section.get_properties().size() == 1) {
            pseudocode += ", bygg::HTML::Property{\"" + escape_invalid(section.get_properties().at(0).get_key()) + "\", \"" + escape_invalid(section.get_properties().at(0).get_value()) + "\"}";
        } else if (options.use_empty_properties) {
            pseudocode += options.use_make_properties ? ", bygg::HTML::make_properties()" : ", bygg::HTML::Properties{}";
        }
    }

    append_properties(section.get_properties());
    pseudocode += options.use_lists ? (get_first_type(section) == SectionType::Section ? ", bygg::HTML::SectionList {\n" : ", bygg::HTML::ElementList {\n") : ",\n";
    handle_section(section, tabc + 1);
    append_tabs(tabc);
    pseudocode += options.use_lists ? "}};\n" : "};\n";
    pseudocode += (options.include_main ? "}\n" : "");

    return pseudocode;
}
