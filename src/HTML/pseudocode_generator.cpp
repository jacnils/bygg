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
            switch (c) {
                case '"': ret += "\\\""; break;
                case '\t': if (options.sequence_mode == SequenceMode::Replace) ret += "\\t"; break;
                case '\n': if (options.sequence_mode == SequenceMode::Replace) ret += "\\n"; break;
                case '\r': if (options.sequence_mode == SequenceMode::Replace) ret += "\\r"; break;
                default: ret += c; break;
            }
        }
        return ret;
    };

    static const std::unordered_map<bygg::HTML::Type, string_type> type_map {
        {Type::Non_Closed, "Type::Non_Closed"},
        {Type::Non_Opened, "Type::Non_Opened"},
        {Type::Non_Self_Closing, "Type::Non_Self_Closing"},
        {Type::Self_Closing, "Type::Self_Closing"},
        {Type::Text, "Type::Text"},
        {Type::Text_No_Formatting, "Type::Text_No_Formatting"},
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
                    ret[i] = sub_section.empty() ? SectionType::Empty : SectionType::Section;
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

    std::string prefix{"bygg::HTML::"};
    string_type pseudocode{};
    int tabc{};

    if (options.include_main && !options.main_function.empty()) {
        if (!options.header_path.empty()) pseudocode += "#include <" + options.header_path + ">\n\n";
        if (options.main_args == MainArgs::ArgcArgv) {
            pseudocode += (options.main_return_type.empty() ? "int" : options.main_return_type) + " " + options.main_function + "(int argc, char** argv) {\n";
        } else {
            pseudocode += (options.main_return_type.empty() ? "int" : options.main_return_type) + " " + options.main_function + "() {\n";
        }

        if (options.namespace_level == NamespaceLevel::ByggNamespace) {
            pseudocode += "\tusing namespace bygg;\n";
            prefix = "HTML::";
        } else if (options.namespace_level == NamespaceLevel::ByggHTMLNamespace) {
            pseudocode += "\tusing namespace bygg::HTML;\n";
            prefix = "";
        }

        tabc = 1;
    } else {
        if (options.namespace_level == NamespaceLevel::ByggNamespace) {
            pseudocode += "using namespace bygg;\n";
            prefix = "HTML::";
        } else if (options.namespace_level == NamespaceLevel::ByggHTMLNamespace) {
            pseudocode += "using namespace bygg::HTML;\n";
            prefix = "";
        }
    }

    const auto append_tabs = [&pseudocode](const int tabc) {
        for (int i{0}; i < tabc; i++) pseudocode += "\t";
    };

    const auto append_properties = [&pseudocode, &escape_invalid, &prefix](const auto& properties) {
        bool first_passed{false};
        for (const auto& it : properties) {
            if (first_passed) {
                pseudocode += ", ";
            }
            pseudocode += prefix + "Property{\"" + escape_invalid(it.get_key()) + "\", \"" + escape_invalid(it.get_value()) + "\"}";
            first_passed = true;
        }
    };

    const auto append_properties_block = [&pseudocode, &append_properties, &options, &prefix](const auto& properties) {
        if (options.use_make_properties) {
            pseudocode += prefix + "make_properties(";
            append_properties(properties);
            pseudocode += ")";
        } else {
            pseudocode += prefix + "Properties{";
            append_properties(properties);
            pseudocode += "}";
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
                    if (!options.use_tag_enums) throw bygg::invalid_argument{"Invalid tag"};
                    pseudocode += prefix + "Section{";
                    pseudocode += prefix + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(i_section.at_section(index).get_tag())));
                } catch (bygg::invalid_argument&) {
                    pseudocode += prefix + "Section{\"" + escape_invalid(i_section.at_section(index).get_tag()) + "\"";
                }

                const auto& _section{ i_section.at_section(index) };
                const auto& _properties{ _section.get_properties() };
                if (_properties.size() > 1 || !options.use_implicit_property) {
                    pseudocode += ", ";
                    append_properties_block(_properties);
                } else if (_properties.size() == 1) {
                    pseudocode += ", " + prefix + "Property{\"" + escape_invalid(_properties.at(0).get_key()) + "\", \"" + escape_invalid(_properties.at(0).get_value()) + "\"}";
                } else if (options.use_empty_properties) {
                    pseudocode += options.use_make_properties ? (", " + prefix + "make_properties()") : (", " + prefix + "Properties{}");
                }

                const auto& _map = get_element_type(_section);
                SectionType front_type = _map.find(0) != _map.end() ? _map.at(0) : SectionType::Empty;

                pseudocode += options.use_lists ? (front_type == SectionType::Section ? (prefix + "SectionList {\n") : (prefix + "ElementList {\n")) : ",\n";
                handle_section(i_section.at_section(index), i_tabc + 1);
                append_tabs(i_tabc);
                pseudocode += options.use_lists ? "}},\n" : "},\n";
            } else {
                try {
                    if (!options.use_tag_enums) throw bygg::invalid_argument{"Invalid tag"};
                    const auto& properties{i_section.at(index).get_properties()};
                    pseudocode += prefix + "Element{";
                    pseudocode += prefix + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(i_section.at(index).get_tag())));
                    if (properties.size() == 1 && options.use_implicit_property) {
                        pseudocode += ", " + prefix + "Property{\"" + escape_invalid(properties.at(0).get_key()) + "\", \"" + escape_invalid(properties.at(0).get_value()) + "\"}";
                    } else if (!properties.empty()) {
                        pseudocode += ", ";
                        append_properties_block(properties);
                    } else if (options.use_empty_properties) {
                        pseudocode += options.use_make_properties ? (", " + prefix + "make_properties()") : (", " + prefix + "Properties{}");
                    }
                    pseudocode += ", \"" + escape_invalid(i_section.at(index).get_data()) + "\"},\n";
                } catch (bygg::invalid_argument&) {
                    const auto& properties{i_section.at(index).get_properties()};
                    pseudocode += prefix + "Element{\"" + i_section.at(index).get_tag() + "\",";
                    if (properties.size() == 1 && options.use_implicit_property) {
                        pseudocode += " " + prefix + "Property{\"" + escape_invalid(properties.at(0).get_key()) + "\", \"" + escape_invalid(properties.at(0).get_value()) + "\"}, \"" + escape_invalid(i_section.at(index).get_data()) + "\"},\n";
                    } else if (!properties.empty()) {
                        pseudocode += " ";
                        append_properties_block(properties);
                        pseudocode += ", \"" + escape_invalid(i_section.at(index).get_data()) + "\", " + prefix + type_map.at(i_section.at(index).get_type()) + "},\n";
                    } else if (options.use_empty_properties) {
                        pseudocode += options.use_make_properties ? (" " + prefix + "make_properties()") : (" " + prefix + "Properties{}");
                        pseudocode += ", \"" + escape_invalid(i_section.at(index).get_data()) + "\", " + prefix + type_map.at(i_section.at(index).get_type()) + "},\n";
                    } else {
                        pseudocode += "\"" + escape_invalid(i_section.at(index).get_data()) + "\", " + prefix + type_map.at(i_section.at(index).get_type()) + "},\n";
                    }
                }
            }
        }
    };

    append_tabs(tabc);

    try {
        if (!options.use_tag_enums) throw bygg::invalid_argument{"Invalid tag"};
        pseudocode += options.root_name.empty() ? (prefix + "Section{" + prefix + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(section.get_tag())))) : (prefix + "Section " + options.root_name + "{" + prefix + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(section.get_tag()))));
        if (section.get_properties().size() > 1 || options.use_empty_properties) {
            pseudocode += ", ";
            append_properties_block(section.get_properties());
        } else if (section.get_properties().size() == 1) {
            pseudocode += ", " + prefix + "Property{\"" + escape_invalid(section.get_properties().at(0).get_key()) + "\", \"" + escape_invalid(section.get_properties().at(0).get_value()) + "\"}";
        } else if (options.use_empty_properties) {
            pseudocode += options.use_make_properties ? (", " + prefix + "make_properties()") : (", " + prefix + "Properties{}");
        }
    } catch (bygg::invalid_argument&) {
        pseudocode += options.root_name.empty() ? (prefix + "Section{\"") : (prefix + "Section " + options.root_name + "{\"" + section.get_tag() + "\"");
        if (section.get_properties().size() > 1 || options.use_empty_properties) {
            pseudocode += ", ";
            append_properties_block(section.get_properties());
        } else if (section.get_properties().size() == 1) {
            pseudocode += ", " + prefix + "Property{\"" + escape_invalid(section.get_properties().at(0).get_key()) + "\", \"" + escape_invalid(section.get_properties().at(0).get_value()) + "\"}";
        } else if (options.use_empty_properties) {
            pseudocode += options.use_make_properties ? (", " + prefix + "make_properties()") : (", " + prefix + "Properties{}");
        }
    }

    pseudocode += options.use_lists ? (get_first_type(section) == SectionType::Section ? (", " + prefix + "SectionList {\n") : (", " + prefix + "ElementList {\n")) : ",\n";
    handle_section(section, tabc + 1);
    append_tabs(tabc);
    pseudocode += options.use_lists ? "}};\n" : "};\n";
    pseudocode += options.include_main ? "}\n" : "";

    return pseudocode;
}
