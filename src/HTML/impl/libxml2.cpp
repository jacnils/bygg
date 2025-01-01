/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <algorithm>
#include <functional>
#include <cstring>

#include <bygg/except.hpp>
#include <bygg/HTML/tag.hpp>
#include <bygg/HTML/impl/libxml2.hpp>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

bygg::TagList bygg::parse_html_string(const string_type& html) {
    TagList ret{};

    LIBXML_TEST_VERSION

    htmlDocPtr doc = htmlReadMemory(html.c_str(), static_cast<int>(html.size()), nullptr, "UTF-8", HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

    if (doc == nullptr) {
        throw invalid_argument("Failed to parse input string");
    }

    std::function<void(xmlNodePtr)> recursive_process_node = [&](xmlNodePtr node) {
        for (xmlNodePtr current_node = node; current_node; current_node = current_node->next) {
            if (current_node->type == XML_ELEMENT_NODE) {
                string_type tag = reinterpret_cast<const char*>(current_node->name);

                HTML::Properties properties;
                for (xmlAttrPtr attr = current_node->properties; attr; attr = attr->next) {
                    string_type key = reinterpret_cast<const char*>(attr->name);
                    xmlChar* value = xmlNodeGetContent(attr->children);
                    properties.push_back(HTML::Property(key, reinterpret_cast<const char*>(value)));
                    xmlFree(value);
                }

                string_type data{};
                for (xmlNodePtr child = current_node->children; child; child = child->next) {
                    if (child->type == XML_TEXT_NODE) {
                        xmlChar* content = xmlNodeGetContent(child);

                        if (content && *content && !std::all_of(reinterpret_cast<const char*>(content), reinterpret_cast<const char*>(content) + std::strlen(reinterpret_cast<const char*>(content)), isspace)) {
                            data += reinterpret_cast<const char*>(content);
                        }

                        xmlFree(content);
                    } else if (child->type == XML_ELEMENT_NODE) {
                        data += "<__bygg_placeholder_tag>";
                    }
                }

                int depth{};
                for (xmlNodePtr parent = current_node->parent; parent; parent = parent->parent) {
                    if (parent->type == XML_ELEMENT_NODE) {
                        ++depth;
                    }
                }

                const auto get_type_from_tag = [&tag](bygg::HTML::Type& t) -> bool {
                    try {
                        const auto resolved = bygg::HTML::resolve_tag(tag);
                        t = resolve_tag(resolved).second;
                        return true;
                    } catch (invalid_argument&) {
                        return false;
                    }
                };

                bygg::HTML::Type type{};
                get_type_from_tag(type);

                ret.push_back({tag, type, data, properties, depth});

                recursive_process_node(current_node->children);
            }
        }
    };

    recursive_process_node(xmlDocGetRootElement(doc));

    xmlFreeDoc(doc);
    xmlCleanupParser();

    return ret;
}