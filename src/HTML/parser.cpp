/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <bygg/HTML/parser.hpp>
#include <bygg/HTML/impl/libxml2.hpp>
#include <stack>

bygg::HTML::Section bygg::HTML::Parser::parse_html_string(const string_type& html, const Options& options) {
    Section container{Tag::Empty, make_properties()};
    TagList list = bygg::parse_html_string(html);

    std::stack<Section*> section_stack;
    section_stack.push(&container);

    for (size_t i = 0; i < list.size(); ++i) {
        const auto& it = list[i];
        bool is_container = bygg::HTML::is_container(it.tag);

        while (section_stack.size() > it.depth + 1) {
            section_stack.pop();
        }

        Section* current_section = section_stack.top();

        if (is_container) {
            current_section->push_back(Section(it.tag, it.properties));
            section_stack.push(&current_section->back_section());
        } else {
            if (i > 0 && list[i].depth > list[i - 1].depth && list[i].data.empty() && list[i-1].data.empty() && options.consider_whitespace) {
                Section new_sect = Section(it.tag, it.properties);
                current_section->push_back(new_sect);
                section_stack.push(&current_section->back_section());
            } else {
                // Utilize the <__bygg_placeholder_tag> marker to replace the inner tag with the actual tag
                if (it.data.empty() == false && options.handle_inner_tags && it.data.find("<__bygg_placeholder_tag>") != string_type::npos) {
                    string_type data = it.data;

                    size_t inner = i + 1;
                    while (inner < list.size() && list[inner].depth > it.depth) {
                        Element inner_elem{};
                        try {
                            const auto tag_t = resolve_tag(resolve_tag(list[inner].tag)).second;
                            inner_elem = Element(list[inner].tag,
                                list[inner].properties,
                                list[inner].data,
                                options.assume_inner_tag_is_non_self_closing ?
                                Type::Non_Self_Closing : tag_t);
                        } catch (std::exception&) {
                            inner_elem = Element(list[inner].tag,
                                list[inner].properties,
                                list[inner].data,
                                options.assume_inner_tag_is_non_self_closing ?
                                Type::Non_Self_Closing : list[inner].type);
                        }

                        size_t pos = data.find("<__bygg_placeholder_tag>");

                        if (pos == string_type::npos) {
                            break;
                        }

                        data.replace(pos, 24, inner_elem.get());

                        ++inner;
                    }

                    i = inner;

                    if (data.length() - 1 != string_type::npos && data.at(data.length() - 1) == ' ') {
                        data.pop_back();
                    }

                    if (data.find('\n') != string_type::npos && options.replace_newlines) {
                        size_t pos = data.find('\n');

                        while (pos != string_type::npos) {
                            data.replace(pos, 1, " ");
                            pos = data.find('\n');
                        }
                    }

                    current_section->push_back(Element(it.tag, it.properties, data, it.type));
                } else {
                    string_type data = it.data;

                    if (data.find('\n') != string_type::npos && options.replace_newlines) {
                        size_t pos = data.find('\n');

                        while (pos != string_type::npos) {
                            data.replace(pos, 1, " ");
                            pos = data.find('\n');
                        }
                    }

                    if (data.find("<__bygg_placeholder_tag>") != string_type::npos) {
                        size_t pos = data.find("<__bygg_placeholder_tag>");

                        while (pos != string_type::npos) {
                            data.replace(pos, 24, "");
                            pos = data.find("<__bygg_placeholder_tag>");
                        }
                    }

                    current_section->push_back(Element(it.tag, it.properties, data, it.type));
                }
            }
        }
    }

    return container;
}