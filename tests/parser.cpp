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

#ifdef BYGG_USE_LIBXML2
void Parser::test_html_parser() {
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
#endif