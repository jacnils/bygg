/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <bygg/except.hpp>
#include <bygg/HTML/pseudocode_generator.hpp>

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

    static const std::unordered_map<bygg::HTML::Type, string_type> type_map {
        {Type::Non_Closed, "bygg::HTML::Type::Non_Closed"},
        {Type::Non_Opened, "bygg::HTML::Type::Non_Opened"},
        {Type::Non_Self_Closing, "bygg::HTML::Type::Non_Self_Closing"},
        {Type::Self_Closing, "bygg::HTML::Type::Self_Closing"},
        {Type::Text, "bygg::HTML::Type::Text"},
        {Type::Text_No_Formatting, "bygg::HTML::Type::Text_No_Formatting"},
    };

    const auto get_front_element_type = [](const Section& section, const size_type index = 0) -> SectionType {
        if (section.empty()) {
            return SectionType::Empty;
        }
        try {
            static_cast<void>(section.at_section(index)); // Intentionally discard the result
            return SectionType::Section;
        } catch (std::exception&) {
            return SectionType::Element;
        }
    };

    string_type pseudocode = "#include <bygg/bygg.hpp>\n\nint main() {\n";
    int tabc = 1;

    const auto append_tabs = [&pseudocode](const int tabc) {
        for (int i{0}; i < tabc; i++) pseudocode += "\t";
    };

    const auto append_properties = [&pseudocode](const auto& properties) {
        bool first_passed{false};
        for (const auto& it : properties) {
            if (first_passed) {
                pseudocode += ", ";
            }
            pseudocode += "bygg::HTML::Property{\"" + it.get_key() + "\", \"" + it.get_value() + "\"}";
            first_passed = true;
        }
    };

    const std::function<void(const Section&, int)> handle_section = [&](const Section& i_section, const int i_tabc) -> void {
        for (size_type index = 0; index < i_section.size(); index++) {
            append_tabs(i_tabc);
            if (get_front_element_type(i_section, index) == SectionType::Section) {
                try {
                    if (options.use_tag_enums == false) {
                        throw bygg::invalid_argument{"Invalid tag"};
                    }
                    pseudocode += "bygg::HTML::Section{" + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(i_section.at_section(index).get_tag()))) + ", bygg::HTML::make_properties(";
                } catch (bygg::invalid_argument&) {
                    pseudocode += "bygg::HTML::Section{\"" + i_section.at_section(index).get_tag() + "\", bygg::HTML::make_properties(";
                }
                append_properties(i_section.at_section(index).get_properties());

                const auto front_type = get_front_element_type(i_section.at_section(index));

                if (front_type == SectionType::Section) {
                    pseudocode += "), bygg::HTML::SectionList {\n";
                } else {
                    pseudocode += "), bygg::HTML::ElementList {\n";
                }

                handle_section(i_section.at_section(index), i_tabc + 1);
                append_tabs(i_tabc);
                pseudocode += "}},\n";
            } else {
                try {
                    if (options.use_tag_enums == false) {
                        throw bygg::invalid_argument{"Invalid tag"};
                    }

                    pseudocode += "bygg::HTML::Element{" + HTML::resolve_tag_enum_name(HTML::resolve_tag(tolower(i_section.at(index).get_tag()))) + ", bygg::HTML::make_properties(";
                } catch (bygg::invalid_argument&) {
                    pseudocode += "bygg::HTML::Element{\"" + i_section.at(index).get_tag() + "\", bygg::HTML::make_properties(";
                }

                append_properties(i_section.at(index).get_properties());
                pseudocode += "), \"" + i_section.at(index).get_data() + "\", " + type_map.at(i_section.at(index).get_type()) + "},\n";
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
    pseudocode += "), bygg::HTML::SectionList {\n";
    handle_section(section, tabc + 1);
    append_tabs(tabc);
    pseudocode += "}};\n}\n";

    return pseudocode;
}
