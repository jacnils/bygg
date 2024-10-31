/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <bygg/except.hpp>
#include <bygg/HTML/tag.hpp>
#include <bygg/HTML/impl/libxml2.hpp>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

bygg::TagList bygg::parse_html_string(const string_type& html) {
    TagList ret{};

    LIBXML_TEST_VERSION

    htmlDocPtr doc = htmlReadMemory(html.c_str(), static_cast<int>(html.size()), nullptr, nullptr, HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

    if (doc == nullptr) {
        throw invalid_argument("Failed to parse input string");
    }

    std::function<void(xmlNodePtr)> processNode = [&](xmlNodePtr node) {
        for (xmlNodePtr curNode = node; curNode; curNode = curNode->next) {
            if (curNode->type == XML_ELEMENT_NODE) {
                string_type tag = reinterpret_cast<const char*>(curNode->name);

                HTML::Properties properties;
                for (xmlAttrPtr attr = curNode->properties; attr; attr = attr->next) {
                    string_type key = reinterpret_cast<const char*>(attr->name);
                    string_type value = reinterpret_cast<const char*>(xmlNodeGetContent(attr->children));
                    properties.push_back(HTML::Property(key, value));
                }

                string_type data;
                for (xmlNodePtr child = curNode->children; child; child = child->next) {
                    if (child->type == XML_TEXT_NODE) {
                        const char* content = reinterpret_cast<const char*>(xmlNodeGetContent(child));
                        if (content && *content && !std::all_of(content, content + std::strlen(content), isspace)) {
                            data += content;
                        }
                    }
                }

                int depth = 0;
                for (xmlNodePtr parent = curNode->parent; parent; parent = parent->parent) {
                    if (parent->type == XML_ELEMENT_NODE) {
                        ++depth;
                    }
                }

                // TODO: This is a little wasteful, handle it better.
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
                if (get_type_from_tag(type) == false) {
                    // TODO: Handle unknown tags
                    if (tag == "br" || tag == "img" || tag == "input" || tag == "meta" || tag == "link") {
                        type = bygg::HTML::Type::Self_Closing;
                    }
                }

                ret.push_back({tag, type, data, properties, depth});

                processNode(curNode->children);
            }
        }
    };

    processNode(xmlDocGetRootElement(doc));

    xmlFreeDoc(doc);
    xmlCleanupParser();

    return ret;
}