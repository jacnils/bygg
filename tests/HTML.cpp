/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <bygg/bygg.hpp>
#include <catch2/catch_test_macros.hpp>
#include "test.hpp"

// NOLINTBEGIN
// Disable linting. This is a test program and the code is intentionally bad in some places.
// As such, I think it's best to turn linting off rather than to try and resolve any such warnings.

void HTML::test_tag() {
    const auto expected_values = bygg::HTML::get_tag_map();

    for (const auto& it : expected_values) {
        REQUIRE((bygg::HTML::resolve_tag(it.first).first == it.second.first && bygg::HTML::resolve_tag(it.first).second == it.second.second));
    }
}

void HTML::test_property() {
    const auto test_get_and_set = []() {
        using namespace bygg::HTML;

        Property property;
        REQUIRE((property.get_key().empty() && property.get_value().empty() && property.get_key().empty() && property.get_value().empty()));

        property.set({"key", "value"});

        REQUIRE(property.get_key() == "key");
        REQUIRE(property.get_value() == "value");
        REQUIRE(property.get_key() == "key");
        REQUIRE(property.get_value() == "value");

        property.set_key("new_key");
        property.set_value("new_value");

        REQUIRE(property.get_key() == "new_key");
        REQUIRE(property.get_value() == "new_value");
        REQUIRE(property.get_key() == "new_key");
        REQUIRE(property.get_value() == "new_value");

        property.set_key("newer_key");

        REQUIRE(property.get_key() == "newer_key");
        REQUIRE(property.get_value() == "new_value");
        REQUIRE(property.get_key() == "newer_key");
        REQUIRE(property.get_value() == "new_value");

        property.set_value("newer_value");

        REQUIRE(property.get_key() == "newer_key");
        REQUIRE(property.get_value() == "newer_value");
        REQUIRE(property.get_key() == "newer_key");
        REQUIRE(property.get_value() == "newer_value");

        REQUIRE(property.empty() == false);

        property.clear();

        REQUIRE(property.empty() == true);

        property.set_key("new_key");
        property.set_value("new_value");

        REQUIRE(property.get() == "new_key=\"new_value\"");
    };

    const auto test_copy_property = []() {
        using namespace bygg::HTML;

        bygg::HTML::Property property;

        property.set({"key", "value"});
    };

    test_get_and_set();
    test_copy_property();
}

void HTML::test_properties() {
    const auto test_get_and_set = []() {
        using namespace bygg::HTML;

        Properties properties;

        REQUIRE(properties.empty());

        properties.push_back({"key1", "value1"});
        properties.push_back({"key2", "value2"});
        properties.push_back({"key3", "value3"});

        REQUIRE(properties.size() == 3);

        std::size_t index = 0;
        for (const auto& it : properties) {
            if (index == 0) {
                REQUIRE(it.get_key() == "key1");
                REQUIRE(it.get_value() == "value1");
            } else if (index == 1) {
                REQUIRE(it.get_key() == "key2");
                REQUIRE(it.get_value() == "value2");
            } else if (index == 2) {
                REQUIRE(it.get_key() == "key3");
                REQUIRE(it.get_value() == "value3");
            }

            ++index;
        }

        REQUIRE(properties.at(0).get_key() == "key1");
        REQUIRE(properties.at(0).get_value() == "value1");
        REQUIRE(properties.at(1).get_key() == "key2");
        REQUIRE(properties.at(1).get_value() == "value2");
        REQUIRE(properties.at(2).get_key() == "key3");
        REQUIRE(properties.at(2).get_value() == "value3");

        // should fail, out of range
        try {
            static_cast<void>(properties.at(3));
        } catch (const bygg::out_of_range& e) {
            REQUIRE(std::string(e.what()) == "Index out of range");
        }

        REQUIRE(properties[0] == properties.at(0));
        REQUIRE(properties[1] == properties.at(1));
        REQUIRE(properties[2] == properties.at(2));
    };

    const auto test_copy_properties = []() {
        using namespace bygg::HTML;

        Properties properties;

        properties += {"key1", "value1"};
        properties += {"key2", "value2"};
        properties += {"key3", "value3"};

        Properties new_properties = properties;

        REQUIRE(properties == new_properties);

        properties.clear();
    };

    const auto test_iterators = []() {
        using namespace bygg::HTML;

        Properties properties = make_properties(Property{"key1", "value1"}, Property{"key2", "value2"}, Property{"key3", "value3"});

        std::size_t index{0};
        for (auto it = properties.begin(); it != properties.end(); ++it) {
            if (index == 0) {
                REQUIRE(it->get_key() == "key1");
                REQUIRE(it->get_value() == "value1");
            } else if (index == 1) {
                REQUIRE(it->get_key() == "key2");
                REQUIRE(it->get_value() == "value2");
            } else if (index == 2) {
                REQUIRE(it->get_key() == "key3");
                REQUIRE(it->get_value() == "value3");
            }

            ++index;
        }

        index = 0;
        for (auto it = properties.cbegin(); it != properties.cend(); ++it) {
            if (index == 0) {
                REQUIRE(it->get_key() == "key1");
                REQUIRE(it->get_value() == "value1");
            } else if (index == 1) {
                REQUIRE(it->get_key() == "key2");
                REQUIRE(it->get_value() == "value2");
            } else if (index == 2) {
                REQUIRE(it->get_key() == "key3");
                REQUIRE(it->get_value() == "value3");
            }

            ++index;
        }

        index = 0;
        for (auto it = properties.rbegin(); it != properties.rend(); ++it) {
            if (index == 0) {
                REQUIRE(it->get_key() == "key3");
                REQUIRE(it->get_value() == "value3");
            } else if (index == 1) {
                REQUIRE(it->get_key() == "key2");
                REQUIRE(it->get_value() == "value2");
            } else if (index == 2) {
                REQUIRE(it->get_key() == "key1");
                REQUIRE(it->get_value() == "value1");
            }

            ++index;
        }

        index = 0;
        for (auto it = properties.crbegin(); it != properties.crend(); ++it) {
            if (index == 0) {
                REQUIRE(it->get_key() == "key3");
                REQUIRE(it->get_value() == "value3");
            } else if (index == 1) {
                REQUIRE(it->get_key() == "key2");
                REQUIRE(it->get_value() == "value2");
            } else if (index == 2) {
                REQUIRE(it->get_key() == "key1");
                REQUIRE(it->get_value() == "value1");
            }

            ++index;
        }

        index = 0;
        for (const auto& it : properties) {
            if (index == 0) {
                REQUIRE(it.get_key() == "key1");
                REQUIRE(it.get_value() == "value1");
            } else if (index == 1) {
                REQUIRE(it.get_key() == "key2");
                REQUIRE(it.get_value() == "value2");
            } else if (index == 2) {
                REQUIRE(it.get_key() == "key3");
                REQUIRE(it.get_value() == "value3");
            }

            ++index;
        }
    };

    const auto test_find = []() {
        using namespace bygg::HTML;

        Properties properties = make_properties(Property{"key1", "value1"}, Property{"key2", "value2"}, Property{"key3", "value3"});

        REQUIRE(properties.find(Property("key1", "value1")) == 0);
        REQUIRE(properties.find(Property("key2", "value2")) == 1);
        REQUIRE(properties.find(Property("key3", "value3")) == 2);
        REQUIRE(properties.find(Property("key4", "value4")) == Properties::npos);

        REQUIRE(properties.find("key1") == 0);
        REQUIRE(properties.find("key2") == 1);
        REQUIRE(properties.find("key3") == 2);
        REQUIRE(properties.find("key4") == Properties::npos);

        REQUIRE(properties.find("value1") == 0);
        REQUIRE(properties.find("value2") == 1);
        REQUIRE(properties.find("value3") == 2);
        REQUIRE(properties.find("value4") == Properties::npos);

        std::size_t pos = properties.find("key1");

        REQUIRE(properties[pos].get_key() == "key1");
        REQUIRE(properties.at(pos).get_key() == "key1");
        REQUIRE(properties[pos].get_value() == "value1");
        REQUIRE(properties.at(pos).get_value() == "value1");

        pos = properties.find("key2");

        REQUIRE(properties[pos].get_key() == "key2");
        REQUIRE(properties.at(pos).get_key() == "key2");
        REQUIRE(properties[pos].get_value() == "value2");
        REQUIRE(properties.at(pos).get_value() == "value2");

        pos = properties.find("key3");

        REQUIRE(properties[pos].get_key() == "key3");
        REQUIRE(properties.at(pos).get_key() == "key3");
        REQUIRE(properties[pos].get_value() == "value3");
        REQUIRE(properties.at(pos).get_value() == "value3");

        pos = properties.find("key4");

        REQUIRE(pos == Properties::npos);

        try {
            static_cast<void>(properties.at(pos));
        } catch (const bygg::out_of_range& e) {
            REQUIRE(std::string(e.what()) == "Index out of range");
        }
    };

    const auto test_insert = []() {
        using namespace bygg::HTML;

        Properties properties = make_properties(Property{"key1", "value1"}, Property{"key2", "value2"}, Property{"key3", "value3"});

        std::size_t pos = properties.find("key1");
        Property found_property = properties[pos];

        std::size_t pos2 = properties.find("value2");
        properties.insert(pos2, found_property);

        REQUIRE(properties.at(pos2) == found_property);
        REQUIRE(properties.get_properties().at(pos2) == found_property);
        REQUIRE(properties.get_properties().at(pos2).get_key() == "key1");
    };

    const auto test_swap = []() {
        using namespace bygg::HTML;

        Properties properties = make_properties(Property{"key1", "value1"}, Property{"key2", "value2"}, Property{"key3", "value3"});

        std::size_t pos1 = properties.find("key1");
        Property property1 = properties[pos1];

        std::size_t pos2 = properties.find("key2");
        Property property2 = properties[pos2];

        properties.swap(pos1, pos2);

        REQUIRE(properties[pos1] == property2);
        REQUIRE(properties[pos2] == property1);

        properties.swap(property1, property2);

        REQUIRE(properties[pos1] == property1);
        REQUIRE(properties[pos2] == property2);
    };

    const auto test_front_and_back = []() {
        using namespace bygg::HTML;

        Properties properties = make_properties(Property{"key1", "value1"}, Property{"key2", "value2"}, Property{"key3", "value3"});

        REQUIRE(properties.front().get_key() == "key1");
        REQUIRE(properties.front().get_value() == "value1");

        REQUIRE(properties.back().get_key() == "key3");
        REQUIRE(properties.back().get_value() == "value3");
    };

    const auto test_size_empty_and_clear = []() {
        using namespace bygg::HTML;

        Properties properties = make_properties(Property{"key1", "value1"}, Property{"key2", "value2"}, Property{"key3", "value3"});

        REQUIRE(properties.size() == 3);
        REQUIRE(properties.empty() == false);

        properties.clear();

        REQUIRE(properties.size() == 0);
        REQUIRE(properties.empty() == true);
    };

    const auto test_push_front_and_back = []() {
        using namespace bygg::HTML;

        Properties properties;

        properties.push_front({"key1", "value1"});
        properties.push_back({"key2", "value2"});

        REQUIRE(properties.front().get_key() == "key1");
        REQUIRE(properties.front().get_value() == "value1");

        REQUIRE(properties.back().get_key() == "key2");
        REQUIRE(properties.back().get_value() == "value2");
    };

    const auto test_constructors = []() {
        using namespace bygg::HTML;

        Properties properties = make_properties(Property{"key1", "value1"}, Property{"key2", "value2"}, Property{"key3", "value3"});
        Properties new_properties = properties;

        REQUIRE(properties == new_properties);

        Properties new_properties2(properties.get_properties());

        REQUIRE(properties == new_properties2);

        Property property1{"key1", "value1"};
        Properties new_properties3 = make_properties(property1);

        REQUIRE(new_properties3.size() == 1);
    };

    test_get_and_set();
    test_copy_properties();
    test_iterators();
    test_find();
    test_insert();
    test_swap();
    test_front_and_back();
    test_size_empty_and_clear();
    test_push_front_and_back();
    test_constructors();
}

void HTML::test_element() {
    const auto test_get_and_set = []() {
        using namespace bygg::HTML;

        Element element;

        element.set("my_element", {}, {}, bygg::HTML::Type::Data);

        REQUIRE(element.get_tag() == "my_element");
        REQUIRE(element.get_data() == "");
        REQUIRE(element.get_properties() == Properties{});
        REQUIRE(element.get_type() == bygg::HTML::Type::Data);

        element.set_tag("new_element");
        element.set_data("new_data");
        element.set_properties(Properties(std::vector<Property>{{"key", "value"}, {"key2", "value2"}}));
        element.set_type(bygg::HTML::Type::Standalone);

        REQUIRE(element.get_tag() == "new_element");
        REQUIRE(element.get_data() == "new_data");
        REQUIRE(element.get_properties().at(0).get_key() == "key");
        REQUIRE(element.get_properties().at(0).get_value() == "value");
        REQUIRE(element.get_properties().at(1).get_key() == "key2");
        REQUIRE(element.get_properties().at(1).get_value() == "value2");
        REQUIRE(element.get_type() == bygg::HTML::Type::Standalone);
    };

    const auto test_copy_element = []() {
        using namespace bygg::HTML;

        Element element;

        element.set("my_element", {}, {}, bygg::HTML::Type::Data);

        Element new_element = element;

        REQUIRE(element == new_element);
        REQUIRE(element.empty() == false);

        element.clear();

        REQUIRE(element.empty() == true);

        element = std::move(new_element);

        REQUIRE(element.empty() == false);
    };

    const auto test_operators = []() {
        using namespace bygg::HTML;

        Element element1;
        Element element2;

        element1.set("my_element", {}, {}, bygg::HTML::Type::Data);
        element2.set("my_element", {}, {}, bygg::HTML::Type::Data);

        REQUIRE(element1 == element2);

        element2.set("new_element", {}, {}, bygg::HTML::Type::Data);

        REQUIRE(element1 != element2);

        element2 = element1;

        REQUIRE(element1 == element2);
    };

    const auto test_constructors = []() {
        using namespace bygg::HTML;

        Element elem{};

        REQUIRE(elem.get_tag() == "");
        REQUIRE(elem.get_data() == "");
        REQUIRE(elem.get_properties() == Properties{});
        REQUIRE(elem.get_type() == bygg::HTML::Type::Data);

        Element elem2("my_element", "data", bygg::HTML::Type::Standalone);

        REQUIRE(elem2.get_tag() == "my_element");
        REQUIRE(elem2.get_data() == "data");
        REQUIRE(elem2.get_properties() == Properties{});
        REQUIRE(elem2.get_type() == bygg::HTML::Type::Standalone);

        Element elem3(bygg::HTML::Tag::H1, "data");

        REQUIRE(elem3.get_tag() == "h1");
        REQUIRE(elem3.get_data() == "data");
        REQUIRE(elem3.get_properties() == Properties{});
        REQUIRE(elem3.get_type() == bygg::HTML::Type::Data);
    };

    const auto test_string_get = []() {
        using namespace bygg::HTML;

        Element element(bygg::HTML::Tag::H1, "data");

        REQUIRE(element.get() == "<h1>data</h1>");
        REQUIRE(element.get<std::string>() == "<h1>data</h1>");
        REQUIRE(element.get<std::string>(bygg::HTML::Formatting::None) == "<h1>data</h1>");
        REQUIRE(element.get<std::string>(bygg::HTML::Formatting::Pretty) == "<h1>data</h1>\n");
        REQUIRE(element.get<std::string>(bygg::HTML::Formatting::Newline) == "<h1>data</h1>\n");

        element.set_type(bygg::HTML::Type::Closing);
        REQUIRE(element.get<std::string>() == "</h1>");
    };

    const auto test_builder = []() {
        using namespace bygg::HTML;

        Element element = Element{bygg::HTML::Tag::H1, ContentFormatter("This header contains @STRONG(bold) text.").get()};
        REQUIRE(element.get() == "<h1>This header contains <strong>bold</strong> text.</h1>");

        Element element2 = Element{Tag::H1, ContentFormatter("This header contains %s %s %s", "an", "extra", "string").get()};
        REQUIRE(element2.get() == "<h1>This header contains an extra string</h1>");
    };

    test_get_and_set();
    test_copy_element();
    test_operators();
    test_constructors();
    test_string_get();
    test_builder();
}

void HTML::test_section() {
    const auto test_get_and_set = []() {
        using namespace bygg::HTML;

        Section section;

        section.set_tag("my_section");

        REQUIRE(section.get_tag() == "my_section");
        REQUIRE(section.get_sections().empty());
        REQUIRE(section.get_elements().empty());
        REQUIRE(section.get_properties().empty());

        section.set_tag("new_section");
        section.set_properties(make_properties(Property("key", "value"), Property("key2", "value2")));

        REQUIRE(section.get_tag() == "new_section");
        REQUIRE(section.get_properties().at(0).get_key() == "key");
        REQUIRE(section.get_properties().at(0).get_value() == "value");
        REQUIRE(section.get_properties().at(1).get_key() == "key2");
        REQUIRE(section.get_properties().at(1).get_value() == "value2");

        section.set("new_section", Properties(Property("key", "value"), Property("key2", "value2")));

        REQUIRE(section.get_tag() == "new_section");
        REQUIRE(section.get_properties().at(0).get_key() == "key");
        REQUIRE(section.get_properties().at(0).get_value() == "value");
        REQUIRE(section.get_properties().at(1).get_key() == "key2");
        REQUIRE(section.get_properties().at(1).get_value() == "value2");

        section.set_properties(make_properties(Property("key3", "value3"), Property("key4", "value4")));

        REQUIRE(section.get_properties().at(0).get_key() == "key3");
        REQUIRE(section.get_properties().at(0).get_value() == "value3");
        REQUIRE(section.get_properties().at(1).get_key() == "key4");
        REQUIRE(section.get_properties().at(1).get_value() == "value4");

        Section completely_empty_section;

        static_cast<void>(completely_empty_section.get(bygg::HTML::Formatting::None));
        static_cast<void>(completely_empty_section.get(bygg::HTML::Formatting::Newline));
        static_cast<void>(completely_empty_section.get(bygg::HTML::Formatting::Pretty));
    };

    const auto test_copy_section = []() {
        using namespace bygg::HTML;

        Section section;
        section.set("my_section", make_properties(Property("key", "value"), Property("key2", "value2")));

        Section new_section = section;

        REQUIRE(section == new_section);
        REQUIRE(section.empty() == true); // should still be empty, because the
                                          // section itself is not an element, it's more of a container element
                                          // in the same vein as a <div> element

        section.push_back(Element{});

        REQUIRE(section.empty() == false);
        REQUIRE(section != new_section);

        section.clear();

        REQUIRE(section.empty() == true);

        section = std::move(new_section);

        REQUIRE(section.empty() == true);
    };

    const auto test_operators = []() {
        using namespace bygg::HTML;

        Section section1;
        Section section2;

        section1.set("my_section", make_properties(Property("key", "value"), Property("key2", "value2")));
        section2.set("my_section", make_properties(Property("key", "value"), Property("key2", "value2")));

        REQUIRE(section1 == section2);

        section2.set("new_section", make_properties(Property("key", "value"), Property("key2", "value2")));

        REQUIRE(section1 != section2);

        section2 = section1;

        REQUIRE(section1 == section2);
    };

    const auto test_constructors = []() {
        using namespace bygg::HTML;

        Section section{};

        REQUIRE(section.get_tag() == "");
        REQUIRE(section.get_sections().empty());
        REQUIRE(section.get_elements().empty());
        REQUIRE(section.get_properties().empty());

        Section section2("my_section", make_properties(Property("key", "value"), Property("key2", "value2")));

        REQUIRE(section2.get_tag() == "my_section");
        REQUIRE(section2.get_properties().at(0).get_key() == "key");
        REQUIRE(section2.get_properties().at(0).get_value() == "value");
        REQUIRE(section2.get_properties().at(1).get_key() == "key2");
        REQUIRE(section2.get_properties().at(1).get_value() == "value2");

        Section section3(bygg::HTML::Tag::H1, make_properties(Property{"key", "value"}, Property{"key2", "value2"}), {Element{}});

        REQUIRE(section3.get_tag() == "h1");
        REQUIRE(section3.get_properties().at(0).get_key() == "key");
        REQUIRE(section3.get_properties().at(0).get_value() == "value");
        REQUIRE(section3.get_properties().at(1).get_key() == "key2");
        REQUIRE(section3.get_properties().at(1).get_value() == "value2");
    };

    const auto test_iterators = []() {
        using namespace bygg::HTML;

        Section section{bygg::HTML::Tag::Html};

        section.push_back(Element{bygg::HTML::Tag::H2});
        section.push_back(Element{bygg::HTML::Tag::H3});
        section.push_back(Element{bygg::HTML::Tag::H4});
        section.push_back(Element{bygg::HTML::Tag::H5});
        section.push_back(Element{bygg::HTML::Tag::H6});

        Element element;

        std::size_t index{0};
        for (auto it = section.element_begin(); it != section.element_end(); ++it) {
            Element element = *it;

            if (index == 0) {
                REQUIRE(element.get_tag() == "h2");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h2></h2>");
            } else if (index == 1) {
                REQUIRE(element.get_tag() == "h3");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h3></h3>");
            } else if (index == 2) {
                REQUIRE(element.get_tag() == "h4");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h4></h4>");
            } else if (index == 3) {
                REQUIRE(element.get_tag() == "h5");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h5></h5>");
            } else if (index == 4) {
                REQUIRE(element.get_tag() == "h6");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h6></h6>");
            }

            ++index;
        }

        index = 0;
        for (Section::element_const_iterator it = section.element_cbegin(); it != section.element_cend(); ++it) {
            Element element = *it;

            if (index == 0) {
                REQUIRE(element.get_tag() == "h2");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h2></h2>");
            } else if (index == 1) {
                REQUIRE(element.get_tag() == "h3");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h3></h3>");
            } else if (index == 2) {
                REQUIRE(element.get_tag() == "h4");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h4></h4>");
            } else if (index == 3) {
                REQUIRE(element.get_tag() == "h5");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h5></h5>");
            } else if (index == 4) {
                REQUIRE(element.get_tag() == "h6");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h6></h6>");
            }

            ++index;
        }

        index = 0;
        for (Section::element_reverse_iterator it = section.element_rbegin(); it != section.element_rend(); ++it) {
            Element element = *it;

            if (index == 0) {
                REQUIRE(element.get_tag() == "h6");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h6></h6>");
            } else if (index == 1) {
                REQUIRE(element.get_tag() == "h5");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h5></h5>");
            } else if (index == 2) {
                REQUIRE(element.get_tag() == "h4");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h4></h4>");
            } else if (index == 3) {
                REQUIRE(element.get_tag() == "h3");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h3></h3>");
            } else if (index == 4) {
                REQUIRE(element.get_tag() == "h2");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h2></h2>");
            }

            ++index;
        }

        index = 0;
        for (Section::element_const_reverse_iterator it = section.element_crbegin(); it != section.element_crend(); ++it) {
            Element element = *it;

            if (index == 0) {
                REQUIRE(element.get_tag() == "h6");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h6></h6>");
            } else if (index == 1) {
                REQUIRE(element.get_tag() == "h5");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h5></h5>");
            } else if (index == 2) {
                REQUIRE(element.get_tag() == "h4");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h4></h4>");
            } else if (index == 3) {
                REQUIRE(element.get_tag() == "h3");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h3></h3>");
            } else if (index == 4) {
                REQUIRE(element.get_tag() == "h2");
                REQUIRE(element.get_type() == bygg::HTML::Type::Data);
                REQUIRE(element.get() == "<h2></h2>");
            }

            ++index;
        }
    };

    const auto test_section_iterators = []() {
        using namespace bygg::HTML;

        Section section{Tag::Html,
            Section{Tag::Head,
                Element{Tag::Title, "Title"},
                Element{Tag::Meta},
            },
            Section{Tag::Body,
                Element{Tag::H1, "Header"},
                Element{Tag::H2, "Subheader"},
                Section{Tag::Div,
                    Element{Tag::P, "Paragraph"},
                    Element{Tag::P, "Paragraph 2"},
                },
            },
            Section{Tag::Footer,
                Element{Tag::P, "Footer"},
            },
        };

        for (Section::section_iterator it = section.section_begin(); it != section.section_end(); ++it) {
            Section _section = *it;

            if (_section.get_tag() == "head") {
                REQUIRE(_section.get_elements().size() == 2);
                REQUIRE(_section.get_elements().at(0).get_tag() == "title");
                REQUIRE(_section.get_elements().at(0).get_data() == "Title");
                REQUIRE(_section.get_elements().at(1).get_tag() == "meta");
            } else if (_section.get_tag() == "body") {
                REQUIRE(_section.size() == 3);
                REQUIRE(_section.get_elements().at(0).get_tag() == "h1");
                REQUIRE(_section.get_elements().at(0).get_data() == "Header");
                REQUIRE(_section.get_elements().at(1).get_tag() == "h2");
                REQUIRE(_section.get_elements().at(1).get_data() == "Subheader");

                Section _section2 = _section.get_sections().at(0);
                REQUIRE(_section2.get_tag() == "div");
                REQUIRE(_section2.get_elements().size() == 2);

                REQUIRE(_section2.get_elements().at(0).get_tag() == "p");
                REQUIRE(_section2.get_elements().at(0).get_data() == "Paragraph");
                REQUIRE(_section2.get_elements().at(1).get_tag() == "p");
                REQUIRE(_section2.get_elements().at(1).get_data() == "Paragraph 2");
            } else if (_section.get_tag() == "footer") {
                REQUIRE(_section.get_elements().size() == 1);
                REQUIRE(_section.get_elements().at(0).get_tag() == "p");
                REQUIRE(_section.get_elements().at(0).get_data() == "Footer");
            }
        }
    };

    const auto test_variant_iterators = []() {
        using namespace bygg::HTML;

        Section section{Tag::Html,
            Element{Tag::H1, "Header"},
            Element{Tag::H2, "Subheader"},
            Section{Tag::Div,
                Element{Tag::P, "Paragraph"},
                Element{Tag::P, "Paragraph 2"},
            },
        };

        std::size_t index{};
        for (auto it : section) {
            std::visit([&index](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, Element>) {
                    if (index == 0) {
                        REQUIRE(arg.get_tag() == "h1");
                        REQUIRE(arg.get_data() == "Header");
                    } else if (index == 1) {
                        REQUIRE(arg.get_tag() == "h2");
                        REQUIRE(arg.get_data() == "Subheader");
                    }
                } else if constexpr (std::is_same_v<T, Section>) {
                    if (index == 2) {
                        REQUIRE(arg.get_tag() == "div");
                        REQUIRE(arg.get_elements().size() == 2);

                        std::size_t _index{};
                        for (bygg::HTML::Section::element_iterator _it = arg.element_begin(); _it != arg.element_end(); ++_it) {
                            Element _element = *_it;

                            if (_index == 0) {
                                REQUIRE(_element.get_tag() == "p");
                                REQUIRE(_element.get_data() == "Paragraph");
                            } else if (_index == 1) {
                                REQUIRE(_element.get_tag() == "p");
                                REQUIRE(_element.get_data() == "Paragraph 2");
                            }
                            ++_index;
                        }
                    }
                }

                ++index;
            }, it);
        }
    };

    const auto test_find = []() {
        using namespace bygg::HTML;

        Section section{bygg::HTML::Tag::Html};

        section.push_back(Element{bygg::HTML::Tag::H2, "data"});
        section.push_back(Element{bygg::HTML::Tag::H3, "data"});
        section.push_back(Element{bygg::HTML::Tag::H4, "data"});
        section.push_back(Element{bygg::HTML::Tag::H5, "data"});
        section.push_back(Element{bygg::HTML::Tag::H6, "data"});

        REQUIRE(section.find(Element{bygg::HTML::Tag::H2, "data"}) == 0);
        REQUIRE(section.find(Element{bygg::HTML::Tag::H3, "data"}) == 1);
        REQUIRE(section.find(Element{bygg::HTML::Tag::H4, "data"}) == 2);
        REQUIRE(section.find(Element{bygg::HTML::Tag::H5, "data"}) == 3);
        REQUIRE(section.find(Element{bygg::HTML::Tag::H6, "data"}) == 4);
        REQUIRE(section.find(Element{bygg::HTML::Tag::H1, "data"}) == Section::npos);

        REQUIRE(section.find("h2") == 0);
        REQUIRE(section.find("h3") == 1);
        REQUIRE(section.find("h4") == 2);
        REQUIRE(section.find("h5") == 3);
        REQUIRE(section.find("h6") == 4);

        REQUIRE(section.find("<h2>data</h2>", 0, FindParameters::Search_Deserialized) == 0);
        REQUIRE(section.find("<h3>data</h3>", 0, FindParameters::Search_Deserialized) == 1);
        REQUIRE(section.find("<h4>data</h4>", 0, FindParameters::Search_Deserialized) == 2);
        REQUIRE(section.find("<h5>data</h5>", 0, FindParameters::Search_Deserialized) == 3);
        REQUIRE(section.find("<h6>data</h6>", 0, FindParameters::Search_Deserialized) == 4);
        REQUIRE(section.find("data") == 0);
        REQUIRE(section.find("h1") == Section::npos);
        REQUIRE(section.find("bygg sucks") == Section::npos);
    };

    const auto test_insert = []() {
        using namespace bygg::HTML;

        Section section{bygg::HTML::Tag::Html};

        section.push_back(Element{bygg::HTML::Tag::H2, "data"});
        section.push_back(Element{bygg::HTML::Tag::H3, "data"});
        section.push_back(Element{bygg::HTML::Tag::H4, "data"});
        section.push_back(Element{bygg::HTML::Tag::H5, "data"});
        section.push_back(Element{bygg::HTML::Tag::H6, "data"});

        std::size_t pos = section.find("h2");

        Element element = std::get<Element>(section[pos]);

        std::size_t pos2 = section.find("h4");

        section.insert(pos2, element);

        REQUIRE(section.at(pos2) != element);
        REQUIRE(section.get_elements().at(pos2) != element);
        REQUIRE(section.get_elements().at(pos2).get_tag() == "h4");
    };

    const auto test_swap = []() {
        using namespace bygg::HTML;

        Section section{bygg::HTML::Tag::Html};

        section.push_back(Element{bygg::HTML::Tag::H2, "data"});
        section.push_back(Element{bygg::HTML::Tag::H3, "data"});
        section.push_back(Element{bygg::HTML::Tag::H4, "data"});
        section.push_back(Element{bygg::HTML::Tag::H5, "data"});
        section.push_back(Element{bygg::HTML::Tag::H6, "data"});

        std::size_t pos1 = section.find("h2");
        Element element1 = std::get<Element>(section[pos1]);

        std::size_t pos2 = section.find("h4");
        Element element2 = std::get<Element>(section[pos2]);

        section.swap(pos1, pos2);

        REQUIRE(std::get<Element>(section[pos1]) == element2);
        REQUIRE(std::get<Element>(section[pos2]) == element1);

        section.swap(element1, element2);

        REQUIRE(std::get<Element>(section[pos1]) == element1);
        REQUIRE(std::get<Element>(section[pos2]) == element2);
    };

    const auto test_front_and_back = []() {
        using namespace bygg::HTML;

        Section section{bygg::HTML::Tag::Html};

        section.push_back(Element{bygg::HTML::Tag::H2, "data"});
        section.push_back(Element{bygg::HTML::Tag::H3, "data"});
        section.push_back(Element{bygg::HTML::Tag::H4, "data"});
        section.push_back(Element{bygg::HTML::Tag::H5, "data"});
        section.push_back(Element{bygg::HTML::Tag::H6, "data"});

        REQUIRE(section.front().get_tag() == "h2");
        REQUIRE(section.front().get_data() == "data");

        REQUIRE(section.back().get_tag() == "h6");
        REQUIRE(section.back().get_data() == "data");
    };

    const auto test_size_empty_and_clear = []() {
        using namespace bygg::HTML;

        Section section{bygg::HTML::Tag::Html};

        section.push_back(Element{bygg::HTML::Tag::H2, "data"});
        section.push_back(Element{bygg::HTML::Tag::H3, "data"});
        section.push_back(Element{bygg::HTML::Tag::H4, "data"});
        section.push_back(Element{bygg::HTML::Tag::H5, "data"});
        section.push_back(Element{bygg::HTML::Tag::H6, "data"});

        REQUIRE(section.size() == 5);
        REQUIRE(section.empty() == false);

        section.clear();

        REQUIRE(section.size() == 0);
        REQUIRE(section.empty() == true);
    };

    const auto test_push_front_and_back = []() {
        using namespace bygg::HTML;

        Section section{bygg::HTML::Tag::Html};

        section.push_front(Element{bygg::HTML::Tag::H2, "data"});
        section.push_back(Element{bygg::HTML::Tag::H3, "data"});

        REQUIRE(section.front().get_tag() == "h2");
        REQUIRE(section.front().get_data() == "data");

        REQUIRE(section.back().get_tag() == "h3");
        REQUIRE(section.back().get_data() == "data");
    };

    const auto test_string_get = []() {
        using namespace bygg::HTML;

        Section section{bygg::HTML::Tag::Html};

        section.push_back(Element{bygg::HTML::Tag::H2, "data"});
        section.push_back(Element{bygg::HTML::Tag::H3, "data"});
        section.push_back(Element{bygg::HTML::Tag::H4, "data"});
        section.push_back(Element{bygg::HTML::Tag::H5, "data"});

        const std::string expected_1 = "<html><h2>data</h2><h3>data</h3><h4>data</h4><h5>data</h5></html>";
        const std::string expected_2 = "<html>\n<h2>data</h2>\n<h3>data</h3>\n<h4>data</h4>\n<h5>data</h5>\n</html>";
        const std::string expected_3 = "<html>\n\t<h2>data</h2>\n\t<h3>data</h3>\n\t<h4>data</h4>\n\t<h5>data</h5>\n</html>";

        REQUIRE(section.get() == expected_1);
        REQUIRE(section.get<std::string>() == expected_1);
        REQUIRE(section.get<std::string>(bygg::HTML::Formatting::None) == expected_1);
        REQUIRE(section.get<std::string>(bygg::HTML::Formatting::Newline) == expected_2);
        REQUIRE(section.get<std::string>(bygg::HTML::Formatting::Pretty) == expected_3);
    };

    const auto test_handle_elements = []() {
        using namespace bygg::HTML;

        Section section{bygg::HTML::Tag::Html};

        section.push_back(Element{bygg::HTML::Tag::H2, "data"});
        section.push_back(Element{bygg::HTML::Tag::H3, "data"});
        section.push_back(Element{bygg::HTML::Tag::H4, "data"});
        section.push_back(Element{bygg::HTML::Tag::H5, "data"});

        Element retrieved_elem1 = section.get_elements().at(0);
        Element retrieved_elem2 = section.get_elements().at(1);
        Element retrieved_elem3 = section.get_elements().at(2);
        Element retrieved_elem4 = section.get_elements().at(3);

        REQUIRE(retrieved_elem1.get_tag() == "h2");
        REQUIRE(retrieved_elem1.get_data() == "data");
        REQUIRE(retrieved_elem2.get_tag() == "h3");
        REQUIRE(retrieved_elem2.get_data() == "data");
        REQUIRE(retrieved_elem3.get_tag() == "h4");
        REQUIRE(retrieved_elem3.get_data() == "data");
        REQUIRE(retrieved_elem4.get_tag() == "h5");
        REQUIRE(retrieved_elem4.get_data() == "data");
    };

    const auto test_handle_sections = []() {
        using namespace bygg::HTML;

        Section section{bygg::HTML::Tag::Html};

        section.push_back(Element{bygg::HTML::Tag::H2, "data"});
        section.push_back(Element{bygg::HTML::Tag::H3, "data"});
        section.push_back(Element{bygg::HTML::Tag::H4, "data"});

        Section new_section = section;

        new_section.set_tag(bygg::HTML::Tag::Div);
        section.push_back(new_section);

        REQUIRE(section.get() == "<html><h2>data</h2><h3>data</h3><h4>data</h4><div><h2>data</h2><h3>data</h3><h4>data</h4></div></html>");

        Section retrieved_section = section.get_sections().at(0);

        REQUIRE(retrieved_section.get_tag() == "div");

        Element retrieved_elem1 = retrieved_section.get_elements().at(0);
        Element retrieved_elem2 = retrieved_section.get_elements().at(1);
        Element retrieved_elem3 = retrieved_section.get_elements().at(2);

        REQUIRE(retrieved_elem1.get_tag() == "h2");
        REQUIRE(retrieved_elem1.get_data() == "data");
        REQUIRE(retrieved_elem2.get_tag() == "h3");
        REQUIRE(retrieved_elem2.get_data() == "data");
        REQUIRE(retrieved_elem3.get_tag() == "h4");
        REQUIRE(retrieved_elem3.get_data() == "data");
    };

    const auto the_test_to_end_all_tests = []() {
        using namespace bygg::HTML;

        Section base_section{bygg::HTML::Tag::Html};

        base_section.push_back(Element{bygg::HTML::Tag::H2, "data"});
        base_section.push_back(Element{bygg::HTML::Tag::H3, "data"});
        base_section.push_back(Element{bygg::HTML::Tag::H4, "data"});
        base_section.push_back(Element{bygg::HTML::Tag::H5, "data"});

        REQUIRE(base_section.get() == "<html><h2>data</h2><h3>data</h3><h4>data</h4><h5>data</h5></html>");

        Section section{bygg::HTML::Tag::Html};
        Section section_plus_plus{bygg::HTML::Tag::Html};

        // just... why? because i can, that's why
        for (std::size_t i = 0; i < 10; ++i) {
            Section _section{bygg::HTML::Tag::Div};

            _section.push_back(Element{bygg::HTML::Tag::P, "Data " + std::to_string(i)});

            section_plus_plus += _section;
            section_plus_plus += section_plus_plus;

            REQUIRE(section_plus_plus.size() == (i + 1) * 2);
        }

        Section new_section{bygg::HTML::Tag::Empty, std::vector<bygg::HTML::Element> {
            bygg::HTML::Element{bygg::HTML::Tag::H1, "Hello world!"},
            bygg::HTML::Element{bygg::HTML::Tag::P, "Hello world paragraph!"}
        }};

        REQUIRE(new_section.get<std::string>() == "<h1>Hello world!</h1><p>Hello world paragraph!</p>");

        Section section_container = bygg::HTML::make_section_container(
            bygg::HTML::Element{bygg::HTML::Tag::H1, "Hello world!"},
            bygg::HTML::Element{bygg::HTML::Tag::P, "Hello world paragraph!"}
        );

        REQUIRE(section_container.get<std::string>() == "<h1>Hello world!</h1><p>Hello world paragraph!</p>");
        REQUIRE(section_container.get<std::string>(bygg::HTML::Formatting::Newline) == "<h1>Hello world!</h1>\n<p>Hello world paragraph!</p>");
        REQUIRE(section_container.get<std::string>(bygg::HTML::Formatting::Pretty) == "<h1>Hello world!</h1>\n<p>Hello world paragraph!</p>");
        REQUIRE(Section().get<std::string>() == "");
    };

    test_get_and_set();
    test_copy_section();
    test_operators();
    test_constructors();
    test_iterators();
    test_variant_iterators();
    test_section_iterators();
    test_find();
    test_insert();
    test_swap();
    test_front_and_back();
    test_size_empty_and_clear();
    test_push_front_and_back();
    test_string_get();
    test_constructors();
    test_handle_elements();
    test_handle_sections();
    the_test_to_end_all_tests();
}

void HTML::test_document() {
    const auto test_get_and_set = []() {
        using namespace bygg::HTML;

        Document document;

        document.set(Section {
            bygg::HTML::Tag::Html, {},
            {
                Element{bygg::HTML::Tag::H1, "data1"},
                Element{bygg::HTML::Tag::H2, "data2"},
                Element{bygg::HTML::Tag::H3, "data3"},
                Element{bygg::HTML::Tag::H4, "data4"},
                Element{bygg::HTML::Tag::H5, "data5"},
                Element{bygg::HTML::Tag::H6, "data6"}
            }
        });

        REQUIRE(document.get() == "<!DOCTYPE html><html><h1>data1</h1><h2>data2</h2><h3>data3</h3><h4>data4</h4><h5>data5</h5><h6>data6</h6></html>");
        REQUIRE(document.get<std::string>() == "<!DOCTYPE html><html><h1>data1</h1><h2>data2</h2><h3>data3</h3><h4>data4</h4><h5>data5</h5><h6>data6</h6></html>");
        REQUIRE(document.get<std::string>(bygg::HTML::Formatting::None) == "<!DOCTYPE html><html><h1>data1</h1><h2>data2</h2><h3>data3</h3><h4>data4</h4><h5>data5</h5><h6>data6</h6></html>");
        REQUIRE(document.get<std::string>(bygg::HTML::Formatting::Pretty) == "<!DOCTYPE html>\n<html>\n\t<h1>data1</h1>\n\t<h2>data2</h2>\n\t<h3>data3</h3>\n\t<h4>data4</h4>\n\t<h5>data5</h5>\n\t<h6>data6</h6>\n</html>");
        REQUIRE(document.get<std::string>(bygg::HTML::Formatting::Newline) == "<!DOCTYPE html>\n<html>\n<h1>data1</h1>\n<h2>data2</h2>\n<h3>data3</h3>\n<h4>data4</h4>\n<h5>data5</h5>\n<h6>data6</h6>\n</html>");

        REQUIRE(document.get_doctype() == "<!DOCTYPE html>");
        REQUIRE(document.get_section() == Section {
            bygg::HTML::Tag::Html, {},
            {
                Element{bygg::HTML::Tag::H1, "data1"},
                Element{bygg::HTML::Tag::H2, "data2"},
                Element{bygg::HTML::Tag::H3, "data3"},
                Element{bygg::HTML::Tag::H4, "data4"},
                Element{bygg::HTML::Tag::H5, "data5"},
                Element{bygg::HTML::Tag::H6, "data6"}
            }
        });

        REQUIRE(document.get_section().get() == "<html><h1>data1</h1><h2>data2</h2><h3>data3</h3><h4>data4</h4><h5>data5</h5><h6>data6</h6></html>");

        document.set_doctype("<!NEW_DOCTYPE html>");

        REQUIRE(document.get_doctype() == "<!NEW_DOCTYPE html>");
        REQUIRE(document.get() == "<!NEW_DOCTYPE html><html><h1>data1</h1><h2>data2</h2><h3>data3</h3><h4>data4</h4><h5>data5</h5><h6>data6</h6></html>");
    };

    const auto test_copy_document = []() {
        using namespace bygg::HTML;

        Document document;

        document.set(Section {
            bygg::HTML::Tag::Html, {},
            {
                Element{bygg::HTML::Tag::H1, "data1"},
                Element{bygg::HTML::Tag::H2, "data2"},
                Element{bygg::HTML::Tag::H3, "data3"},
                Element{bygg::HTML::Tag::H4, "data4"},
                Element{bygg::HTML::Tag::H5, "data5"},
                Element{bygg::HTML::Tag::H6, "data6"}
            }
        });

        Document new_document = document;

        REQUIRE(document == new_document);
        REQUIRE(document.empty() == false);

        document.clear();

        REQUIRE(document.empty() == true);

        document = std::move(new_document);

        REQUIRE(document.empty() == false);
    };

    const auto test_operators = []() {
        using namespace bygg::HTML;

        Document document1;
        Document document2;

        document1.set(Section {
            bygg::HTML::Tag::Html, {},
            {
                Element{bygg::HTML::Tag::H1, "data1"},
                Element{bygg::HTML::Tag::H2, "data2"},
                Element{bygg::HTML::Tag::H3, "data3"},
                Element{bygg::HTML::Tag::H4, "data4"},
                Element{bygg::HTML::Tag::H5, "data5"},
                Element{bygg::HTML::Tag::H6, "data6"}
            }
        });

        document2.set(Section {
            bygg::HTML::Tag::Html, {},
            {
                Element{bygg::HTML::Tag::H1, "data1"},
                Element{bygg::HTML::Tag::H2, "data2"},
                Element{bygg::HTML::Tag::H3, "data3"},
                Element{bygg::HTML::Tag::H4, "data4"},
                Element{bygg::HTML::Tag::H5, "data5"},
                Element{bygg::HTML::Tag::H6, "data6"}
            }
        });

        REQUIRE(document1 == document2);

        document2.set(Section {
            bygg::HTML::Tag::Html, {},
            {
                Element{bygg::HTML::Tag::H6, "data1"},
                Element{bygg::HTML::Tag::H5, "data2"},
                Element{bygg::HTML::Tag::H4, "data3"},
                Element{bygg::HTML::Tag::H3, "data4"},
                Element{bygg::HTML::Tag::H2, "data5"},
                Element{bygg::HTML::Tag::H1, "data6"}
            }
        });

        REQUIRE(document1 != document2);
    };

    const auto test_constructors = []() {
        using namespace bygg::HTML;

        Document document{};

        REQUIRE(document.get_doctype() == "<!DOCTYPE html>");
        REQUIRE(document.get_section().get_tag() == "");
        REQUIRE(document.get_section().get_sections().empty());
        REQUIRE(document.get_section().get_elements().empty());
        REQUIRE(document.get_section().get_properties().empty());

        Document document2(Section {
            bygg::HTML::Tag::Html, {},
            {
                Element{bygg::HTML::Tag::H1, "data1"},
                Element{bygg::HTML::Tag::H2, "data2"},
                Element{bygg::HTML::Tag::H3, "data3"},
                Element{bygg::HTML::Tag::H4, "data4"},
                Element{bygg::HTML::Tag::H5, "data5"},
                Element{bygg::HTML::Tag::H6, "data6"}
            }
        });

        REQUIRE(document2.get_doctype() == "<!DOCTYPE html>");
        REQUIRE(document2.get_section().get_tag() == "html");
        REQUIRE(document2.get_section().get_sections().empty());
        REQUIRE(document2.get_section().get_elements().size() == 6);
        REQUIRE(document2.get_section().get_properties().empty());

        Document document3(Section(bygg::HTML::Tag::Html, {
            Section(bygg::HTML::Tag::Head, {
                Element(bygg::HTML::Tag::Title, "Title")
            }),
            Section(bygg::HTML::Tag::Body, {
                Element(bygg::HTML::Tag::H1, "Hello, World!")
            }),
        }), "<!NEW_DOCTYPE html>");

        REQUIRE(document3.get_doctype() == "<!NEW_DOCTYPE html>");
        REQUIRE(document3.get_section().get_tag() == "html");
        REQUIRE(document3.get_section().get_sections().size() == 2);
        REQUIRE(document3.get_section().get_elements().empty());
        REQUIRE(document3.get_section().get_properties().empty());
    };

    test_get_and_set();
    test_copy_document();
    test_operators();
    test_constructors();
}

void HTML::test_pseudocode_generator() {
    using namespace bygg::HTML;

    Section mysection = Section{bygg::HTML::Tag::Html, {},
        {
            Element{bygg::HTML::Tag::H1, "data1"},
            Element{bygg::HTML::Tag::H2, "data2"},
            Element{bygg::HTML::Tag::H3, "data3"},
            Element{bygg::HTML::Tag::H4, "data4"},
            Element{bygg::HTML::Tag::H5, "data5"},
            Element{bygg::HTML::Tag::H6, "data6"}
        }
    };

    std::string expected_1 = "#include <bygg/bygg.hpp>\n\nint main() {\n\tbygg::HTML::Section root{\"html\", bygg::HTML::make_properties(), bygg::HTML::ElementList {\n\t\tbygg::HTML::Element{\"h1\", bygg::HTML::make_properties(), \"data1\", bygg::HTML::Type::Non_Self_Closing},\n\t\tbygg::HTML::Element{\"h2\", bygg::HTML::make_properties(), \"data2\", bygg::HTML::Type::Non_Self_Closing},\n\t\tbygg::HTML::Element{\"h3\", bygg::HTML::make_properties(), \"data3\", bygg::HTML::Type::Non_Self_Closing},\n\t\tbygg::HTML::Element{\"h4\", bygg::HTML::make_properties(), \"data4\", bygg::HTML::Type::Non_Self_Closing},\n\t\tbygg::HTML::Element{\"h5\", bygg::HTML::make_properties(), \"data5\", bygg::HTML::Type::Non_Self_Closing},\n\t\tbygg::HTML::Element{\"h6\", bygg::HTML::make_properties(), \"data6\", bygg::HTML::Type::Non_Self_Closing},\n\t}};\n}\n";
    REQUIRE(generate_pseudocode(mysection, {.use_tag_enums = false, .use_lists = true, .use_empty_properties = true}) == expected_1);

    std::string expected_2 = "#include <bygg/bygg.hpp>\n\nint main() {\n\tbygg::HTML::Section root{bygg::HTML::Tag::Html, bygg::HTML::make_properties(), bygg::HTML::ElementList {\n\t\tbygg::HTML::Element{bygg::HTML::Tag::H1, bygg::HTML::make_properties(), \"data1\"},\n\t\tbygg::HTML::Element{bygg::HTML::Tag::H2, bygg::HTML::make_properties(), \"data2\"},\n\t\tbygg::HTML::Element{bygg::HTML::Tag::H3, bygg::HTML::make_properties(), \"data3\"},\n\t\tbygg::HTML::Element{bygg::HTML::Tag::H4, bygg::HTML::make_properties(), \"data4\"},\n\t\tbygg::HTML::Element{bygg::HTML::Tag::H5, bygg::HTML::make_properties(), \"data5\"},\n\t\tbygg::HTML::Element{bygg::HTML::Tag::H6, bygg::HTML::make_properties(), \"data6\"},\n\t}};\n}\n";
    REQUIRE(generate_pseudocode(mysection, {.use_tag_enums = true, .use_lists = true, .use_empty_properties = true}) == expected_2);

    Section mysection2 = Section{bygg::HTML::Tag::Html,
        Section{Tag::Head,
            Element{Tag::Title, "Title"}
        },
        Section{Tag::Body,
            Element{Tag::H1, "Hello, World!"}
        },
        Section{Tag::Footer,
            Element{Tag::P, "Footer"}
        },
    };

    // TODO: More advanced tests
}
// NOLINTEND