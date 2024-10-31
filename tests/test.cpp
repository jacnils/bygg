/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <src/bygg.cpp>
#include <catch2/catch_test_macros.hpp>

// NOLINTBEGIN
// Disable linting. This is a test program and the code is intentionally bad in some places.
// As such, I think it's best to turn linting off rather than to try and resolve any such warnings.

inline namespace HTML {
    void test_tag() {
        const auto expected_values = bygg::HTML::get_tag_map();

        for (const auto& it : expected_values) {
            REQUIRE((bygg::HTML::resolve_tag(it.first).first == it.second.first && bygg::HTML::resolve_tag(it.first).second == it.second.second));
        }
    }

    void test_property() {
        const auto test_get_and_set = []() {
            using namespace bygg::HTML;

            Property property;
            REQUIRE((property.get_pair().first.empty() && property.get_pair().second.empty() && property.get_key().empty() && property.get_value().empty()));

            property.set({"key", "value"});

            REQUIRE(property.get_pair().first == "key");
            REQUIRE(property.get_pair().second == "value");
            REQUIRE(property.get_key() == "key");
            REQUIRE(property.get_value() == "value");

            property.set_key("new_key");
            property.set_value("new_value");

            REQUIRE(property.get_pair().first == "new_key");
            REQUIRE(property.get_pair().second == "new_value");
            REQUIRE(property.get_key() == "new_key");
            REQUIRE(property.get_value() == "new_value");

            property.set_key("newer_key");

            REQUIRE(property.get_pair().first == "newer_key");
            REQUIRE(property.get_pair().second == "new_value");
            REQUIRE(property.get_key() == "newer_key");
            REQUIRE(property.get_value() == "new_value");

            property.set_value("newer_value");

            REQUIRE(property.get_pair().first == "newer_key");
            REQUIRE(property.get_pair().second == "newer_value");
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

    void test_properties() {
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
                    REQUIRE(it.get_pair().first == "key1");
                    REQUIRE(it.get_pair().second == "value1");
                } else if (index == 1) {
                    REQUIRE(it.get_pair().first == "key2");
                    REQUIRE(it.get_pair().second == "value2");
                } else if (index == 2) {
                    REQUIRE(it.get_pair().first == "key3");
                    REQUIRE(it.get_pair().second == "value3");
                }

                ++index;
            }

            REQUIRE(properties.at(0).get_pair().first == "key1");
            REQUIRE(properties.at(0).get_pair().second == "value1");
            REQUIRE(properties.at(1).get_pair().first == "key2");
            REQUIRE(properties.at(1).get_pair().second == "value2");
            REQUIRE(properties.at(2).get_pair().first == "key3");
            REQUIRE(properties.at(2).get_pair().second == "value3");

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
                    REQUIRE(it->get_pair().first == "key1");
                    REQUIRE(it->get_pair().second == "value1");
                } else if (index == 1) {
                    REQUIRE(it->get_pair().first == "key2");
                    REQUIRE(it->get_pair().second == "value2");
                } else if (index == 2) {
                    REQUIRE(it->get_pair().first == "key3");
                    REQUIRE(it->get_pair().second == "value3");
                }

                ++index;
            }

            index = 0;
            for (auto it = properties.cbegin(); it != properties.cend(); ++it) {
                if (index == 0) {
                    REQUIRE(it->get_pair().first == "key1");
                    REQUIRE(it->get_pair().second == "value1");
                } else if (index == 1) {
                    REQUIRE(it->get_pair().first == "key2");
                    REQUIRE(it->get_pair().second == "value2");
                } else if (index == 2) {
                    REQUIRE(it->get_pair().first == "key3");
                    REQUIRE(it->get_pair().second == "value3");
                }

                ++index;
            }

            index = 0;
            for (auto it = properties.rbegin(); it != properties.rend(); ++it) {
                if (index == 0) {
                    REQUIRE(it->get_pair().first == "key3");
                    REQUIRE(it->get_pair().second == "value3");
                } else if (index == 1) {
                    REQUIRE(it->get_pair().first == "key2");
                    REQUIRE(it->get_pair().second == "value2");
                } else if (index == 2) {
                    REQUIRE(it->get_pair().first == "key1");
                    REQUIRE(it->get_pair().second == "value1");
                }

                ++index;
            }

            index = 0;
            for (auto it = properties.crbegin(); it != properties.crend(); ++it) {
                if (index == 0) {
                    REQUIRE(it->get_pair().first == "key3");
                    REQUIRE(it->get_pair().second == "value3");
                } else if (index == 1) {
                    REQUIRE(it->get_pair().first == "key2");
                    REQUIRE(it->get_pair().second == "value2");
                } else if (index == 2) {
                    REQUIRE(it->get_pair().first == "key1");
                    REQUIRE(it->get_pair().second == "value1");
                }

                ++index;
            }

            index = 0;
            for (const auto& it : properties) {
                if (index == 0) {
                    REQUIRE(it.get_pair().first == "key1");
                    REQUIRE(it.get_pair().second == "value1");
                } else if (index == 1) {
                    REQUIRE(it.get_pair().first == "key2");
                    REQUIRE(it.get_pair().second == "value2");
                } else if (index == 2) {
                    REQUIRE(it.get_pair().first == "key3");
                    REQUIRE(it.get_pair().second == "value3");
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

            REQUIRE(properties[pos].get_pair().first == "key1");
            REQUIRE(properties.at(pos).get_pair().first == "key1");
            REQUIRE(properties[pos].get_pair().second == "value1");
            REQUIRE(properties.at(pos).get_pair().second == "value1");

            pos = properties.find("key2");

            REQUIRE(properties[pos].get_pair().first == "key2");
            REQUIRE(properties.at(pos).get_pair().first == "key2");
            REQUIRE(properties[pos].get_pair().second == "value2");
            REQUIRE(properties.at(pos).get_pair().second == "value2");

            pos = properties.find("key3");

            REQUIRE(properties[pos].get_pair().first == "key3");
            REQUIRE(properties.at(pos).get_pair().first == "key3");
            REQUIRE(properties[pos].get_pair().second == "value3");
            REQUIRE(properties.at(pos).get_pair().second == "value3");

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
            REQUIRE(properties.get_properties().at(pos2).get_pair().first == "key1");
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

            REQUIRE(properties.front().get_pair().first == "key1");
            REQUIRE(properties.front().get_pair().second == "value1");

            REQUIRE(properties.back().get_pair().first == "key3");
            REQUIRE(properties.back().get_pair().second == "value3");
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

            REQUIRE(properties.front().get_pair().first == "key1");
            REQUIRE(properties.front().get_pair().second == "value1");

            REQUIRE(properties.back().get_pair().first == "key2");
            REQUIRE(properties.back().get_pair().second == "value2");
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

    void test_element() {
        const auto test_get_and_set = []() {
            using namespace bygg::HTML;

            Element element;

            element.set("my_element", {}, {}, bygg::HTML::Type::Non_Self_Closing);

            REQUIRE(element.get_tag() == "my_element");
            REQUIRE(element.get_data() == "");
            REQUIRE(element.get_properties() == Properties{});
            REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);

            element.set_tag("new_element");
            element.set_data("new_data");
            element.set_properties(Properties(std::vector<Property>{{"key", "value"}, {"key2", "value2"}}));
            element.set_type(bygg::HTML::Type::Self_Closing);

            REQUIRE(element.get_tag() == "new_element");
            REQUIRE(element.get_data() == "new_data");
            REQUIRE(element.get_properties().at(0).get_pair().first == "key");
            REQUIRE(element.get_properties().at(0).get_pair().second == "value");
            REQUIRE(element.get_properties().at(1).get_pair().first == "key2");
            REQUIRE(element.get_properties().at(1).get_pair().second == "value2");
            REQUIRE(element.get_type() == bygg::HTML::Type::Self_Closing);
        };

        const auto test_copy_element = []() {
            using namespace bygg::HTML;

            Element element;

            element.set("my_element", {}, {}, bygg::HTML::Type::Non_Self_Closing);

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

            element1.set("my_element", {}, {}, bygg::HTML::Type::Non_Self_Closing);
            element2.set("my_element", {}, {}, bygg::HTML::Type::Non_Self_Closing);

            REQUIRE(element1 == element2);

            element2.set("new_element", {}, {}, bygg::HTML::Type::Non_Self_Closing);

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
            REQUIRE(elem.get_type() == bygg::HTML::Type::Non_Self_Closing);

            Element elem2("my_element", "data", bygg::HTML::Type::Self_Closing);

            REQUIRE(elem2.get_tag() == "my_element");
            REQUIRE(elem2.get_data() == "data");
            REQUIRE(elem2.get_properties() == Properties{});
            REQUIRE(elem2.get_type() == bygg::HTML::Type::Self_Closing);

            Element elem3(bygg::HTML::Tag::H1, "data");

            REQUIRE(elem3.get_tag() == "h1");
            REQUIRE(elem3.get_data() == "data");
            REQUIRE(elem3.get_properties() == Properties{});
            REQUIRE(elem3.get_type() == bygg::HTML::Type::Non_Self_Closing);
        };

        const auto test_string_get = []() {
            using namespace bygg::HTML;

            Element element(bygg::HTML::Tag::H1, "data");

            REQUIRE(element.get() == "<h1>data</h1>");
            REQUIRE(element.get<std::string>() == "<h1>data</h1>");
            REQUIRE(element.get<std::string>(bygg::HTML::Formatting::None) == "<h1>data</h1>");
            REQUIRE(element.get<std::string>(bygg::HTML::Formatting::Pretty) == "<h1>data</h1>\n");
            REQUIRE(element.get<std::string>(bygg::HTML::Formatting::Newline) == "<h1>data</h1>\n");

            element.set_type(bygg::HTML::Type::Non_Opened);
            REQUIRE(element.get<std::string>() == "</h1>");
        };

        test_get_and_set();
        test_copy_element();
        test_operators();
        test_constructors();
        test_string_get();
    }

    void test_section() {
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
            REQUIRE(section.get_properties().at(0).get_pair().first == "key");
            REQUIRE(section.get_properties().at(0).get_pair().second == "value");
            REQUIRE(section.get_properties().at(1).get_pair().first == "key2");
            REQUIRE(section.get_properties().at(1).get_pair().second == "value2");

            section.set("new_section", Properties(Property("key", "value"), Property("key2", "value2")));

            REQUIRE(section.get_tag() == "new_section");
            REQUIRE(section.get_properties().at(0).get_pair().first == "key");
            REQUIRE(section.get_properties().at(0).get_pair().second == "value");
            REQUIRE(section.get_properties().at(1).get_pair().first == "key2");
            REQUIRE(section.get_properties().at(1).get_pair().second == "value2");

            section.set_properties(make_properties(Property("key3", "value3"), Property("key4", "value4")));

            REQUIRE(section.get_properties().at(0).get_pair().first == "key3");
            REQUIRE(section.get_properties().at(0).get_pair().second == "value3");
            REQUIRE(section.get_properties().at(1).get_pair().first == "key4");
            REQUIRE(section.get_properties().at(1).get_pair().second == "value4");

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

            Section section3;

            section3 += Element{bygg::HTML::Tag::H1, "This is some data."};
            section3 += Element{bygg::HTML::Tag::H1, "This is some data two."};
            section3 += Element{bygg::HTML::Tag::H2, "This is some more data."};

            REQUIRE(section3["h1"]["This is some data."] == section3.get_elements().at(0));
            REQUIRE(section3["h1"]["This is some data two."] == section3.get_elements().at(1));
            REQUIRE(section3["h2"]["This is some more data."] == section3.get_elements().at(2));
            REQUIRE(section3["h3"]["This is some more data."] == Element{});
            REQUIRE(section3["h4"]["This is some more data."] == Element{});
            REQUIRE(section3[Tag::H1]["This is some data."] == section3.get_elements().at(0));
            REQUIRE(section3[Tag::H1]["This is some data two."] == section3.get_elements().at(1));
            REQUIRE(section3[Tag::H2]["This is some more data."] == section3.get_elements().at(2));
            REQUIRE(section3[Tag::H3]["This is some more data."] == Element{});
            REQUIRE(section3[Tag::H4]["This is some more data."] == Element{});
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
            REQUIRE(section2.get_properties().at(0).get_pair().first == "key");
            REQUIRE(section2.get_properties().at(0).get_pair().second == "value");
            REQUIRE(section2.get_properties().at(1).get_pair().first == "key2");
            REQUIRE(section2.get_properties().at(1).get_pair().second == "value2");

            Section section3(bygg::HTML::Tag::H1, make_properties(Property{"key", "value"}, Property{"key2", "value2"}), {Element{}});

            REQUIRE(section3.get_tag() == "h1");
            REQUIRE(section3.get_properties().at(0).get_pair().first == "key");
            REQUIRE(section3.get_properties().at(0).get_pair().second == "value");
            REQUIRE(section3.get_properties().at(1).get_pair().first == "key2");
            REQUIRE(section3.get_properties().at(1).get_pair().second == "value2");
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
            for (auto it = section.begin(); it != section.end(); ++it) {
                Element element = *it;

                if (index == 0) {
                    REQUIRE(element.get_tag() == "h2");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h2></h2>");
                } else if (index == 1) {
                    REQUIRE(element.get_tag() == "h3");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h3></h3>");
                } else if (index == 2) {
                    REQUIRE(element.get_tag() == "h4");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h4></h4>");
                } else if (index == 3) {
                    REQUIRE(element.get_tag() == "h5");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h5></h5>");
                } else if (index == 4) {
                    REQUIRE(element.get_tag() == "h6");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h6></h6>");
                }

                ++index;
            }

            index = 0;
            for (Section::const_iterator it = section.cbegin(); it != section.cend(); ++it) {
                Element element = *it;

                if (index == 0) {
                    REQUIRE(element.get_tag() == "h2");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h2></h2>");
                } else if (index == 1) {
                    REQUIRE(element.get_tag() == "h3");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h3></h3>");
                } else if (index == 2) {
                    REQUIRE(element.get_tag() == "h4");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h4></h4>");
                } else if (index == 3) {
                    REQUIRE(element.get_tag() == "h5");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h5></h5>");
                } else if (index == 4) {
                    REQUIRE(element.get_tag() == "h6");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h6></h6>");
                }

                ++index;
            }

            index = 0;
            for (Section::reverse_iterator it = section.rbegin(); it != section.rend(); ++it) {
                Element element = *it;

                if (index == 0) {
                    REQUIRE(element.get_tag() == "h6");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h6></h6>");
                } else if (index == 1) {
                    REQUIRE(element.get_tag() == "h5");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h5></h5>");
                } else if (index == 2) {
                    REQUIRE(element.get_tag() == "h4");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h4></h4>");
                } else if (index == 3) {
                    REQUIRE(element.get_tag() == "h3");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h3></h3>");
                } else if (index == 4) {
                    REQUIRE(element.get_tag() == "h2");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h2></h2>");
                }

                ++index;
            }

            index = 0;
            for (Section::const_reverse_iterator it = section.crbegin(); it != section.crend(); ++it) {
                Element element = *it;

                if (index == 0) {
                    REQUIRE(element.get_tag() == "h6");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h6></h6>");
                } else if (index == 1) {
                    REQUIRE(element.get_tag() == "h5");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h5></h5>");
                } else if (index == 2) {
                    REQUIRE(element.get_tag() == "h4");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h4></h4>");
                } else if (index == 3) {
                    REQUIRE(element.get_tag() == "h3");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h3></h3>");
                } else if (index == 4) {
                    REQUIRE(element.get_tag() == "h2");
                    REQUIRE(element.get_type() == bygg::HTML::Type::Non_Self_Closing);
                    REQUIRE(element.get() == "<h2></h2>");
                }

                ++index;
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

            REQUIRE(section.find("<h2>data</h2>") == 0);
            REQUIRE(section.find("<h3>data</h3>") == 1);
            REQUIRE(section.find("<h4>data</h4>") == 2);
            REQUIRE(section.find("<h5>data</h5>") == 3);
            REQUIRE(section.find("<h6>data</h6>") == 4);
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

            Element element = section[pos];

            std::size_t pos2 = section.find("h4");

            section.insert(pos2, element);

            REQUIRE(section.at(pos2) == element);
            REQUIRE(section.get_elements().at(pos2) == element);
            REQUIRE(section.get_elements().at(pos2).get_tag() == "h2");
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
            Element element1 = section[pos1];

            std::size_t pos2 = section.find("h4");
            Element element2 = section[pos2];

            section.swap(pos1, pos2);

            REQUIRE(section[pos1] == element2);
            REQUIRE(section[pos2] == element1);

            section.swap(element1, element2);

            REQUIRE(section[pos1] == element1);
            REQUIRE(section[pos2] == element2);
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

    void test_document() {
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

    void test_html() {
        test_tag();
        test_property();
        test_properties();
        test_element();
        test_section();
        test_document();
    }
} // namespace HTML

inline namespace CSS {
    void test_property() {
        const auto test_get_and_set = []() {
            using namespace bygg::CSS;

            Property property;

            property.set("key", "value");

            REQUIRE(property.get_pair().first == "key");
            REQUIRE(property.get_pair().second == "value");
            REQUIRE(property.get_key() == "key");
            REQUIRE(property.get_value() == "value");

            property.set_key("new_key");

            REQUIRE(property.get_pair().first == "new_key");
            REQUIRE(property.get_pair().second == "value");
            REQUIRE(property.get_key() == "new_key");
            REQUIRE(property.get_value() == "value");

            property.set_value("new_value");

            REQUIRE(property.get_pair().first == "new_key");
            REQUIRE(property.get_pair().second == "new_value");
            REQUIRE(property.get_key() == "new_key");
            REQUIRE(property.get_value() == "new_value");

            REQUIRE(property.get_key<std::string>() == "new_key");
            REQUIRE(property.get_value<std::string>() == "new_value");

            REQUIRE(property.get<std::string>() == "new_key: new_value;");
        };

        const auto test_operators = []() {
            using namespace bygg::CSS;

            Property property1;
            Property property2;

            property1.set("key", "value");
            property2.set("key", "value2");

            REQUIRE(property1 == property1);
            REQUIRE(property1 != property2);

            property2 = property1;

            REQUIRE(property1 == property2);

            property2.set("key2", "value2");

            REQUIRE(property1 != property2);
        };

        test_get_and_set();
        test_operators();
    }

    void test_element() {
        const auto test_get_and_set = []() {
            using namespace bygg::CSS;

            Element element;

            element.set_tag("my_element");

            REQUIRE(element.get_tag() == "my_element");
            REQUIRE(element.get_properties().empty());

            element.set_properties(make_properties(Property{"key", "value"}, Property{"key2", "value2"}));

            REQUIRE(element.get_properties().at(0).get_pair().first == "key");
            REQUIRE(element.get_properties().at(0).get_pair().second == "value");
            REQUIRE(element.get_properties().at(1).get_pair().first == "key2");
            REQUIRE(element.get_properties().at(1).get_pair().second == "value2");

            element.set("new_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"}));

            REQUIRE(element.get_tag() == "new_element");
            REQUIRE(element.get_properties().at(0).get_pair().first == "key");
            REQUIRE(element.get_properties().at(0).get_pair().second == "value");
            REQUIRE(element.get_properties().at(1).get_pair().first == "key2");
            REQUIRE(element.get_properties().at(1).get_pair().second == "value2");

            element.set_properties(make_properties(Property{"key3", "value3"}, Property{"key4", "value4"}));

            REQUIRE(element.get_properties().at(0).get_pair().first == "key3");
            REQUIRE(element.get_properties().at(0).get_pair().second == "value3");
            REQUIRE(element.get_properties().at(1).get_pair().first == "key4");
            REQUIRE(element.get_properties().at(1).get_pair().second == "value4");
        };

        const auto test_operators = []() {
            using namespace bygg::CSS;

            Element element1;
            Element element2;

            element1.set("my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"}));
            element2.set("my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"}));

            REQUIRE(element1 == element2);

            element2.set("new_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"}));

            REQUIRE(element1 != element2);

            element2 = element1;

            REQUIRE(element1 == element2);
        };

        const auto test_constructors = []() {
            using namespace bygg::CSS;

            Element element{};

            REQUIRE(element.get_tag() == "");
            REQUIRE(element.get_properties().empty());
            REQUIRE(element.get().empty());

            Element element2{"my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"})};

            REQUIRE(element2.get_tag() == "my_element");
            REQUIRE(element2.get_properties().at(0).get_pair().first == "key");
            REQUIRE(element2.get_properties().at(0).get_pair().second == "value");
            REQUIRE(element2.get_properties().at(1).get_pair().first == "key2");
            REQUIRE(element2.get_properties().at(1).get_pair().second == "value2");
        };

        const auto test_copy_section = []() {
            using namespace bygg::CSS;

            Element element;

            element.set("my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"}));

            Element element2 = element;

            REQUIRE(element == element2);

            element2.set("new_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"}));

            REQUIRE(element != element2);

            element2 = std::move(element);

            REQUIRE(element2.get_tag() == "my_element");
        };

        const auto test_iterators = []() {
            using namespace bygg::CSS;

            Element element;

            element.set("my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"}));

            std::size_t index{0};
            for (Element::iterator it = element.begin(); it != element.end(); ++it) {
                Property property = *it;

                if (index == 0) {
                    REQUIRE(property.get_pair().first == "key");
                    REQUIRE(property.get_pair().second == "value");
                } else if (index == 1) {
                    REQUIRE(property.get_pair().first == "key2");
                    REQUIRE(property.get_pair().second == "value2");
                }

                ++index;
            }

            index = 0;
            for (Element::const_iterator it = element.cbegin(); it != element.cend(); ++it) {
                Property property = *it;

                if (index == 0) {
                    REQUIRE(property.get_pair().first == "key");
                    REQUIRE(property.get_pair().second == "value");
                } else if (index == 1) {
                    REQUIRE(property.get_pair().first == "key2");
                    REQUIRE(property.get_pair().second == "value2");
                }

                ++index;
            }

            index = 0;
            for (Element::reverse_iterator it = element.rbegin(); it != element.rend(); ++it) {
                Property property = *it;

                if (index == 0) {
                    REQUIRE(property.get_pair().first == "key2");
                    REQUIRE(property.get_pair().second == "value2");
                } else if (index == 1) {
                    REQUIRE(property.get_pair().first == "key");
                    REQUIRE(property.get_pair().second == "value");
                }

                ++index;
            }

            index = 0;
            for (Element::const_reverse_iterator it = element.crbegin(); it != element.crend(); ++it) {
                Property property = *it;

                if (index == 0) {
                    REQUIRE(property.get_pair().first == "key2");
                    REQUIRE(property.get_pair().second == "value2");
                } else if (index == 1) {
                    REQUIRE(property.get_pair().first == "key");
                    REQUIRE(property.get_pair().second == "value");
                }

                ++index;
            }
        };

        const auto test_find = []() {
            using namespace bygg::CSS;

            Element element;

            element.set("my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"}));

            REQUIRE(element.find(Property{"key", "value"}) == 0);
            REQUIRE(element.find("key") == 0);
            REQUIRE(element.find(Property{"key2", "value2"}) == 1);
            REQUIRE(element.find("key2") == 1);
            REQUIRE(element.find("key3") == Element::npos);
            REQUIRE(element.find(Property{"key3", "value3"}) == Element::npos);

            Property property = element.at(element.find("key2"));

            REQUIRE(property.get_pair().first == "key2");
        };

        const auto test_insert = []() {
            using namespace bygg::CSS;

            Element element;

            element.set("my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"}));

            std::size_t pos = element.find("key");

            Property property = element.at(pos);

            std::size_t pos2 = element.find("key2");

            element.insert(pos2, property);

            REQUIRE(element.at(pos2) == property);
            REQUIRE(element.get_properties().at(pos2) == property);
            REQUIRE(element.get_properties().at(pos2).get_pair().first == "key");
        };

        const auto test_swap = []() {
            using namespace bygg::CSS;

            Element element;

            element.set("my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"}));

            std::size_t pos1 = element.find("key");
            Property property1 = element.at(pos1);

            std::size_t pos2 = element.find("key2");
            Property property2 = element.at(pos2);

            element.swap(pos1, pos2);

            REQUIRE(element[pos1] == property2);
            REQUIRE(element[pos2] == property1);
        };

        const auto test_front_and_back = []() {
            using namespace bygg::CSS;

            Element element;

            element.set("my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"}));

            REQUIRE(element.front().get_pair().first == "key");
            REQUIRE(element.back().get_pair().first == "key2");
        };

        const auto test_size_empty_and_clear = []() {
            using namespace bygg::CSS;

            Element element;

            element.set("my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"}));

            REQUIRE(element.size() == 2);
            REQUIRE(element.empty() == false);

            element.clear();

            REQUIRE(element.size() == 0);
            REQUIRE(element.empty() == true);
        };

        const auto test_push_front_and_back = []() {
            using namespace bygg::CSS;

            Element element;

            element.push_front(Property{"key", "value"});
            element.push_back(Property{"key2", "value2"});

            REQUIRE(element.front().get_pair().first == "key");
            REQUIRE(element.back().get_pair().first == "key2");
        };

        const auto test_string_get = []() {
            using namespace bygg::CSS;

            Element element;

            element.set("my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"}));

            const std::string expected_1 = "my_element {key: value;key2: value2;}";
            const std::string expected_2 = "my_element {\nkey: value;\nkey2: value2;\n}\n";
            const std::string expected_3 = "my_element {\n\tkey: value;\n\tkey2: value2;\n}\n";

            REQUIRE(element.get() == expected_1);
            REQUIRE(element.get<std::string>() == expected_1);
            REQUIRE(element.get<std::string>(bygg::CSS::Formatting::None) == expected_1);
            REQUIRE(element.get<std::string>(bygg::CSS::Formatting::Newline) == expected_2);
            REQUIRE(element.get<std::string>(bygg::CSS::Formatting::Pretty) == expected_3);
        };

        const auto test_handle_elements = []() {
            using namespace bygg::CSS;

            Element element;

            element.set("my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"}));

            Property retrieved_prop1 = element.get_properties().at(0);
            Property retrieved_prop2 = element.get_properties().at(1);

            REQUIRE(retrieved_prop1.get_pair().first == "key");
            REQUIRE(retrieved_prop1.get_pair().second == "value");
            REQUIRE(retrieved_prop2.get_pair().first == "key2");
            REQUIRE(retrieved_prop2.get_pair().second == "value2");
        };

        test_get_and_set();
        test_operators();
        test_get_and_set();
        test_constructors();
        test_copy_section();
        test_iterators();
        test_find();
        test_insert();
        test_swap();
        test_front_and_back();
        test_size_empty_and_clear();
        test_push_front_and_back();
        test_string_get();
        test_handle_elements();
    }

    void test_stylesheet() {
        const auto test_get_and_set = []() {
            using namespace bygg::CSS;

            Stylesheet stylesheet;

            stylesheet.set({Element{"my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"})}});

            REQUIRE(stylesheet.at(0).get_tag() == "my_element");
            REQUIRE(stylesheet.at(0).get_properties().at(0).get_pair().first == "key");
            REQUIRE(stylesheet.at(0).get_properties().at(0).get_pair().second == "value");
            REQUIRE(stylesheet.at(0).get_properties().at(1).get_pair().first == "key2");
            REQUIRE(stylesheet.at(0).get_properties().at(1).get_pair().second == "value2");

            stylesheet.set({Element{"my_element2", make_properties(Property{"key3", "value3"}, Property{"key4", "value4"})}});

            REQUIRE(stylesheet.at(0).get_tag() == "my_element2");
            REQUIRE(stylesheet.at(0).get_properties().at(0).get_pair().first == "key3");
            REQUIRE(stylesheet.at(0).get_properties().at(0).get_pair().second == "value3");
            REQUIRE(stylesheet.at(0).get_properties().at(1).get_pair().first == "key4");
            REQUIRE(stylesheet.at(0).get_properties().at(1).get_pair().second == "value4");

            try {
                static_cast<void>(stylesheet.at(1));
            } catch (const bygg::out_of_range& e) {
                REQUIRE(true);
            }

            try {
                static_cast<void>(stylesheet.at(0).at(2));
            } catch (const bygg::out_of_range& e) {
                REQUIRE(true);
            }
        };

        const auto test_copy_section = []() {
            using namespace bygg::CSS;

            Stylesheet stylesheet;

            stylesheet.set({Element{"my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"})}});

            Stylesheet new_stylesheet = stylesheet;

            REQUIRE(stylesheet == new_stylesheet);
            REQUIRE(stylesheet.empty() == false);

            stylesheet.clear();

            REQUIRE(stylesheet.empty() == true);

            stylesheet = std::move(new_stylesheet);

            REQUIRE(stylesheet.empty() == false);
        };

        const auto test_operators = []() {
            using namespace bygg::CSS;

            Stylesheet stylesheet1;
            Stylesheet stylesheet2;

            Element element{"my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"})};
            Element element2{"my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"})};

            stylesheet1.set({element});
            stylesheet2.set({element2});

            REQUIRE(stylesheet1 == stylesheet2);

            element2.set("my_element", make_properties(Property{"key3", "value3"}, Property{"key4", "value4"}));

            stylesheet2.set({element2});

            REQUIRE(stylesheet1 != stylesheet2);
        };

        const auto test_constructors = []() {
            using namespace bygg::CSS;

            Stylesheet stylesheet{};

            REQUIRE(stylesheet.empty() == true);

            Stylesheet stylesheet2{{Element{"my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"})}}};

            REQUIRE(stylesheet2.size() == 1);

            Stylesheet stylesheet3 = bygg::CSS::make_stylesheet(
                bygg::CSS::make_element(
                    "my_element", bygg::CSS::make_properties(Property{"property1", "data1"}, Property{"property2", "data2"})
                ),
                bygg::CSS::make_element(
                    "my_element_2", bygg::CSS::make_properties(Property{"property3", "data3"}, Property{"property4", "data4"})
                )
            );

            REQUIRE(stylesheet3.size() == 2);
            REQUIRE(stylesheet3.get<std::string>() == "my_element {property1: data1;property2: data2;}my_element_2 {property3: data3;property4: data4;}");
            REQUIRE(stylesheet3.at(1).at(1).get_pair().first == "property4");
        };

        const auto test_push_front_and_back = []() {
            using namespace bygg::CSS;

            Stylesheet stylesheet;

            stylesheet.push_front(Element{"my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"})});
            stylesheet.push_back(Element{"my_element2", make_properties(Property{"key3", "value3"}, Property{"key4", "value4"})});

            REQUIRE(stylesheet.front().get_tag() == "my_element");
            REQUIRE(stylesheet.back().get_tag() == "my_element2");
        };

        const auto test_swap = []() {
            using namespace bygg::CSS;

            Stylesheet stylesheet;

            stylesheet.push_back(Element{"my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"})});
            stylesheet.push_back(Element{"my_element2", make_properties(Property{"key3", "value3"}, Property{"key4", "value4"})});

            std::size_t pos1 = stylesheet.find("my_element");
            Element element1 = stylesheet.at(pos1);

            std::size_t pos2 = stylesheet.find("my_element2");
            Element element2 = stylesheet.at(pos2);

            stylesheet.swap(pos1, pos2);

            REQUIRE(stylesheet.at(pos1) == element2);
            REQUIRE(stylesheet.at(pos2) == element1);

            stylesheet.swap(element1, element2);

            REQUIRE(stylesheet.at(pos1) == element1);
            REQUIRE(stylesheet.at(pos2) == element2);
        };

        const auto test_size_empty_and_clear = []() {
            using namespace bygg::CSS;

            Stylesheet stylesheet;

            stylesheet.push_back(Element{"my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"})});
            stylesheet.push_back(Element{"my_element2", make_properties(Property{"key3", "value3"}, Property{"key4", "value4"})});

            REQUIRE(stylesheet.size() == 2);
            REQUIRE(stylesheet.empty() == false);

            stylesheet.clear();

            REQUIRE(stylesheet.size() == 0);
            REQUIRE(stylesheet.empty() == true);
        };

        const auto test_insert = []() {
            using namespace bygg::CSS;

            Stylesheet stylesheet;

            stylesheet.push_back(Element{"my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"})});
            stylesheet.push_back(Element{"my_element2", make_properties(Property{"key3", "value3"}, Property{"key4", "value4"})});

            std::size_t pos = stylesheet.find("my_element");

            Element element = stylesheet.at(pos);

            std::size_t pos2 = stylesheet.find("my_element2");

            stylesheet.insert(pos2, element);

            REQUIRE(stylesheet.at(pos2) == element);
            REQUIRE(stylesheet.at(pos2).get_tag() == "my_element");
        };

        const auto test_iterators = []() {
            using namespace bygg::CSS;

            Stylesheet stylesheet;

            stylesheet.push_back(Element{"my_element", make_properties(Property{"key", "value"}, Property{"key2", "value2"})});
            stylesheet.push_back(Element{"my_element2", make_properties(Property{"key3", "value3"}, Property{"key4", "value4"})});
            stylesheet.push_back(Element{"my_element3", make_properties(Property{"key5", "value5"}, Property{"key6", "value6"})});
            stylesheet.push_back(Element{"my_element4", make_properties(Property{"key7", "value7"}, Property{"key8", "value8"})});

            std::size_t index{0};
            for (Stylesheet::iterator it = stylesheet.begin(); it != stylesheet.end(); ++it) {
                Element element = *it;

                if (index == 0) {
                    REQUIRE(element.get_tag() == "my_element");
                } else if (index == 1) {
                    REQUIRE(element.get_tag() == "my_element2");
                } else if (index == 2) {
                    REQUIRE(element.get_tag() == "my_element3");
                } else if (index == 3) {
                    REQUIRE(element.get_tag() == "my_element4");
                }

                ++index;
            }

            index = 0;
            for (Stylesheet::const_iterator it = stylesheet.cbegin(); it != stylesheet.cend(); ++it) {
                Element element = *it;

                if (index == 0) {
                    REQUIRE(element.get_tag() == "my_element");
                } else if (index == 1) {
                    REQUIRE(element.get_tag() == "my_element2");
                } else if (index == 2) {
                    REQUIRE(element.get_tag() == "my_element3");
                } else if (index == 3) {
                    REQUIRE(element.get_tag() == "my_element4");
                }

                ++index;
            }

            index = 0;
            for (Stylesheet::reverse_iterator it = stylesheet.rbegin(); it != stylesheet.rend(); ++it) {
                Element element = *it;

                if (index == 0) {
                    REQUIRE(element.get_tag() == "my_element4");
                } else if (index == 1) {
                    REQUIRE(element.get_tag() == "my_element3");
                } else if (index == 2) {
                    REQUIRE(element.get_tag() == "my_element2");
                } else if (index == 3) {
                    REQUIRE(element.get_tag() == "my_element");
                }

                ++index;
            }

            index = 0;
            for (Stylesheet::const_reverse_iterator it = stylesheet.crbegin(); it != stylesheet.crend(); ++it) {
                Element element = *it;

                if (index == 0) {
                    REQUIRE(element.get_tag() == "my_element4");
                } else if (index == 1) {
                    REQUIRE(element.get_tag() == "my_element3");
                } else if (index == 2) {
                    REQUIRE(element.get_tag() == "my_element2");
                } else if (index == 3) {
                    REQUIRE(element.get_tag() == "my_element");
                }

                ++index;
            }

            index = 0;

            for (const auto& it : stylesheet) {
                Element element = it;

                if (index == 0) {
                    REQUIRE(element.get_tag() == "my_element");
                } else if (index == 1) {
                    REQUIRE(element.get_tag() == "my_element2");
                } else if (index == 2) {
                    REQUIRE(element.get_tag() == "my_element3");
                } else if (index == 3) {
                    REQUIRE(element.get_tag() == "my_element4");
                }

                ++index;
            }
        };

        test_get_and_set();
        test_copy_section();
        test_operators();
        test_constructors();
        test_push_front_and_back();
        test_swap();
        test_size_empty_and_clear();
        test_insert();
        test_iterators();
    }

    void test_color_conversions() {
        bygg::CSS::ColorStruct cstr = bygg::CSS::from_rgba(255, 0, 0, 255); // Should be #FF0000FF

        const auto check_cstr = [&cstr]() {
            REQUIRE(cstr.r == 1.0);
            REQUIRE(cstr.g == 0.0);
            REQUIRE(cstr.b == 0.0);
            REQUIRE(cstr.a == 1.0);
        };

        check_cstr();

        cstr = bygg::CSS::from_hex("#FF0000FF"); // Should be #FF0000FF
        check_cstr();

        cstr = bygg::CSS::from_hex("#FF0000"); // Should still be #FF0000FF
        check_cstr();

        cstr = bygg::CSS::from_float(1.0f, 0.0f, 0.0f, 1.0f); // Should still be #FF0000FF
        check_cstr();

        cstr = bygg::CSS::from_double(1, 0, 0, 1); // Should still be #FF0000FF
        check_cstr();
    }

    void test_color_formatter() {
        bygg::CSS::ColorFormatter formatter{};

        REQUIRE(formatter.get<std::string>() == "#000000");

        formatter.set_color_struct(bygg::CSS::from_hex("#FFFF00"));

        REQUIRE(formatter.get<std::string>() == "#ffff00");
        REQUIRE(formatter.get<std::string>(bygg::CSS::ColorFormatting::Hex) == "#ffff00");
        REQUIRE(formatter.get<std::string>(bygg::CSS::ColorFormatting::Hex_A) == "#ffff00ff");
        REQUIRE(formatter.get<std::string>(bygg::CSS::ColorFormatting::Rgb) == "rgb(255, 255, 0)");
        REQUIRE(formatter.get<std::string>(bygg::CSS::ColorFormatting::Rgb_A) == "rgba(255, 255, 0, 255)");

        formatter.set_formatting(bygg::CSS::ColorFormatting::Hex_A);
        formatter.set_color_struct(bygg::CSS::from_rgba(0, 0, 0, 255));

        REQUIRE(formatter.get<std::string>() == "#000000ff");
    }

    void test_css() {
        test_property();
        test_element();
        test_stylesheet();
        test_color_conversions();
        test_color_formatter();
    }

} // namespace CSS

inline namespace General {
    void test_exceptions() {
        try {
            throw bygg::out_of_range{};
        } catch (const bygg::out_of_range& e) {
            REQUIRE(std::string(e.what()) == "Out of range");
        }

        try {
            throw bygg::out_of_range{"Out of range, very very out of range"};
        } catch (const bygg::out_of_range& e) {
            REQUIRE(std::string(e.what()) == "Out of range, very very out of range");
        }

        try {
            throw bygg::invalid_argument{};
        } catch (const bygg::invalid_argument& e) {
            REQUIRE(std::string(e.what()) == "Invalid argument");
        }

        try {
            throw bygg::invalid_argument{"Invalid, very very invalid argument"};
        } catch (const bygg::invalid_argument& e) {
            REQUIRE(std::string(e.what()) == "Invalid, very very invalid argument");
        }
    }

    void test_npos_values() {
        REQUIRE(bygg::HTML::Property::npos == -1);
        REQUIRE(bygg::HTML::Properties::npos == -1);
        REQUIRE(bygg::HTML::Element::npos == -1);
        REQUIRE(bygg::HTML::Section::npos == -1);
        REQUIRE(bygg::HTML::Document::npos == -1);
        REQUIRE(bygg::CSS::Property::npos == -1);
        REQUIRE(bygg::CSS::Element::npos == -1);
        REQUIRE(bygg::CSS::Stylesheet::npos == -1);
    }

    void test_version() {
        std::tuple<int, int, int> version = bygg::version();

        REQUIRE(std::get<0>(version) >= 0);
        REQUIRE(std::get<1>(version) >= 0);
        REQUIRE(std::get<2>(version) >= 0);
    }
}

#ifdef BYGG_USE_LIBXML2
inline namespace Parser {
    void test_html_parser() {
        /*
        const auto test_struct_output = []() {
            const std::string html = "<html><head><title>Title</title></head><body><p>Paragraph with a <strong>strong</strong> tag in between.</p></body></html>";
            const auto structure = bygg::parse_html_string(html);

            for (const auto& it : structure) {
                std::cout << "Tag: " << it.tag << "\n";
                std::cout << "Data: " << it.data << "\n";
                std::cout << "Depth: " << it.depth << "\n";
                std::cout << "Type: " << static_cast<int>(it.type) << "\n";
                std::cout << "Properties: \n";

                for (const auto& itp : it.properties) {
                    std::cout << "\t" << itp.get_key() << ": " << itp.get_value() << "\n";
                }
            }
        };
        */

        const auto test_basic = []() {
            using namespace bygg::HTML::Parser;

            const std::string html = "<html><head><title>Test</title></head><body><h1>Hello, World!</h1></body></html>";
            const auto document = bygg::HTML::Parser::parse_html_string(html);

            REQUIRE(document.get(bygg::HTML::Formatting::Pretty) == "<html>\n\t<head>\n\t\t<title>Test</title>\n\t</head>\n\t<body>\n\t\t<h1>Hello, World!</h1>\n\t</body>\n</html>");
            REQUIRE(document.get() == html);
        };

        const auto test_slightly_more_advanced = []() {
            using namespace bygg::HTML::Parser;

            const std::string html = "<html><head><title>Test</title></head><body><h1>Hello, World!</h1><h2>Test</h2><h3>Test</h3><h4>Test</h4><h5>Test</h5><h6>Test</h6></body></html>";
            const auto document = bygg::HTML::Parser::parse_html_string(html);

            REQUIRE(document.get(bygg::HTML::Formatting::Pretty) == "<html>\n\t<head>\n\t\t<title>Test</title>\n\t</head>\n\t<body>\n\t\t<h1>Hello, World!</h1>\n\t\t<h2>Test</h2>\n\t\t<h3>Test</h3>\n\t\t<h4>Test</h4>\n\t\t<h5>Test</h5>\n\t\t<h6>Test</h6>\n\t</body>\n</html>");
            REQUIRE(document.get() == html);
        };

        const auto test_divs = []() {
            using namespace bygg::HTML::Parser;

            const std::string html = "<html><head><title>Test</title></head><body><div><h1>Hello, World!</h1></div><div><h2>Test</h2></div><div><h3>Test</h3></div><div><h4>Test</h4></div><div><h5>Test</h5></div><div><h6>Test</h6></div></body></html>";
            const auto document = bygg::HTML::Parser::parse_html_string(html);

            REQUIRE(document.get(bygg::HTML::Formatting::Pretty) == "<html>\n\t<head>\n\t\t<title>Test</title>\n\t</head>\n\t<body>\n\t\t<div>\n\t\t\t<h1>Hello, World!</h1>\n\t\t</div>\n\t\t<div>\n\t\t\t<h2>Test</h2>\n\t\t</div>\n\t\t<div>\n\t\t\t<h3>Test</h3>\n\t\t</div>\n\t\t<div>\n\t\t\t<h4>Test</h4>\n\t\t</div>\n\t\t<div>\n\t\t\t<h5>Test</h5>\n\t\t</div>\n\t\t<div>\n\t\t\t<h6>Test</h6>\n\t\t</div>\n\t</body>\n</html>");
            REQUIRE(document.get() == html);
        };

        const auto test_with_tags_in_between = []() {
            using namespace bygg::HTML::Parser;

            const std::string html = "<html><head><title>Title</title></head><body><p>Paragraph with a <strong>strong</strong> tag in between. And an <em>Italic</em> for good measure.</p></body></html>";
            const auto document = bygg::HTML::Parser::parse_html_string(html);

            REQUIRE(document.get(bygg::HTML::Formatting::Pretty) == "<html>\n\t<head>\n\t\t<title>Title</title>\n\t</head>\n\t<body>\n\t\t<p>Paragraph with a <strong>strong</strong> tag in between. And an <em>Italic</em> for good measure.</p>\n\t</body>\n</html>");
            REQUIRE(document.get() == html);
        };

        /*
        test_struct_output();
        */
        test_basic();
        test_slightly_more_advanced();
        test_divs();
        test_with_tags_in_between();
    }
}
#endif

/**
 * @brief Test cases for the bygg namespace.
 */
SCENARIO("Test general", "[GENERAL]") {
    General::test_exceptions();
    General::test_npos_values();
    General::test_version();
}

/**
 * @brief Test cases for the bygg namespace.
 */
SCENARIO("Test HTML", "[HTML]") {
    HTML::test_html();
}

/**
 * @brief Test cases for the bygg namespace.
 */
SCENARIO("Test CSS", "[CSS]") {
    CSS::test_css();
}

#ifdef BYGG_USE_LIBXML2
/**
 * @brief Test cases for the bygg namespace.
 */
SCENARIO("Test HTML parser", "[libxml2]") {
    Parser::test_html_parser();
}
#endif

// NOLINTEND