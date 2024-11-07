#include <iostream>
#include <fstream>
#include <bygg/HTML/HTML.hpp>
#include <bygg/HTML/parser.hpp>

int main(int argc, char** argv) {
    bygg::HTML::Formatting formatting = bygg::HTML::Formatting::Pretty;
    bool pseudocode{false};

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <file.html> <formatting type>" << "\n";
        return 1;
    }

    if (argc > 2) {
        if (std::string{argv[2]} == "none") {
            formatting = bygg::HTML::Formatting::None;
        } else if (std::string{argv[2]} == "pretty") {
            formatting = bygg::HTML::Formatting::Pretty;
        } else if (std::string{argv[2]} == "newline") {
            formatting = bygg::HTML::Formatting::Newline;
        } else if (std::string{argv[2]} == "pseudo") {
            pseudocode = true;
        } else {
            std::cerr << "Invalid formatting type: " << argv[2] << "\n";
            std::cerr << "Valid types are: none, pretty, newline, pseudo" << "\n";
            return 1;
        }
    }

    std::ifstream file{argv[1]};
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << "\n";
        return 1;
    }

    std::string html{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};

    if (html.empty()) {
        std::cerr << "File is empty: " << argv[1] << "\n";
        return 1;
    }

    bygg::HTML::Section section = bygg::HTML::Parser::parse_html_string(html);

    std::cout << (pseudocode ? bygg::HTML::generate_pseudocode(section) : section.get(formatting)) << "\n";

    return 0;
}
