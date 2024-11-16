/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
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

void CSS::test_property() {
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

void CSS::test_element() {
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
        
        Element element3{"my_new_element", Property{"key", "value"}, Property{"key2", "value2"}};

        REQUIRE(element3.get_tag() == "my_new_element");
        REQUIRE(element3.get_properties().at(0).get_pair().first == "key");
        REQUIRE(element3.get_properties().at(0).get_pair().second == "value");
        REQUIRE(element3.get_properties().at(1).get_pair().first == "key2");
        REQUIRE(element3.get_properties().at(1).get_pair().second == "value2");
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

void CSS::test_stylesheet() {
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

void CSS::test_color_conversions() {
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

void CSS::test_color_formatter() {
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