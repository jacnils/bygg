/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <fstream>
#include <string_view>
#include <unistd.h>
#include <filesystem>
#include <bygg/HTML/HTML.hpp>
#include <bygg/HTML/parser.hpp>

#ifndef BYGG_VERSION
#define BYGG_VERSION "0.0.0"
#endif

#ifdef _WIN32
        constexpr std::string_view temp_file{"temp_data"};
#else
        constexpr std::string_view temp_file{"/tmp/temp_data"};
#endif

enum class InputType {
    HTML,
    Markdown,
};

int main(int argc, char** argv) {
    bygg::HTML::Formatting formatting = bygg::HTML::Formatting::Pretty;
    InputType input_type = InputType::HTML;
    bool pseudocode{false};
    std::string input_file{};

    const std::vector<std::string_view> args{argv, argv + argc};

    for (int i{1}; i < args.size(); ++i) {
        if (args.at(i) == "-h" || args.at(i) == "--help" || args.at(i) == "/h") {
            std::cout << "bygg-cli - a command-line interface for bygg\n";
            std::cout << "usage: bygg-cli [options] [file]\n";
            std::cout << "options:\n";
            std::cout << "  -h, --help, /h: show this help message\n";
            std::cout << "  -v, --version, /v: show the version of bygg-cli\n";
            std::cout << "  -c, --copyright, /c: show the copyright information\n";
            std::cout << "  -f, --formatting, /f: set the formatting type (none, pretty, newline, pseudo)\n";
            std::cout << "  -i, --input, /i: set the input type (html, markdown)\n";
            std::cout << "  file: the file to read from\n";
            std::cout << "if no file is specified, input will be read from stdin\n";
            std::cout << "if no formatting type is specified, the default is pretty\n";
            std::cout << "if no input type is specified, the default is html\n";
            return 0;
        } else if (args.at(i) == "-v" || args.at(i) == "--version" || args.at(i) == "/v") {
            std::cout << "bygg-cli version " << BYGG_VERSION << "\n";
            return 0;
        } else if (args.at(i) == "-c" || args.at(i) == "--copyright" || args.at(i) == "/c") {
            std::cout << "bygg-cli - a command-line interface for bygg\n";
            std::cout << "by Jacob Nilsson & contributors\n";
            std::cout << "licensed under the MIT license\n";
            return 0;
        } else if (args.at(i) == "-f" || args.at(i) == "--formatting" || args.at(i) == "/f") {
            if (argc >= i + 1) {
                if (args.at(i+1) == "none") {
                    formatting = bygg::HTML::Formatting::None;
                } else if (args.at(i+1) == "pretty") {
                    formatting = bygg::HTML::Formatting::Pretty;
                } else if (args.at(i+1) == "newline") {
                    formatting = bygg::HTML::Formatting::Newline;
                } else if (args.at(i+1) == "pseudo") {
                    pseudocode = true;
                } else {
                    std::cerr << "invalid formatting type: " << args.at(i+1) << "\n";
                    std::cerr << "valid types: none, pretty, newline, pseudo" << "\n";
                    return 1;
                }
            } else {
                std::cout << "no formatting type specified." << "\n";
            }

            ++i;
        } else if (args.at(i) == "-i" || args.at(i) == "--input" || args.at(i) == "/i") {
            if (argc >= i + 1) {
                if (args.at(i+1) == "html") {
                    input_type = InputType::HTML;
                } else if (args.at(i+1) == "markdown") {
                    input_type = InputType::Markdown;
                } else {
                    std::cerr << "invalid input type: " << args.at(i+1) << "\n";
                    std::cerr << "valid types: html, markdown" << "\n";
                    return 1;
                }
            } else {
                std::cout << "no input type specified." << "\n";
            }

            ++i;
        } else {
            if (input_file.empty()) {
                input_file = std::string{args.at(i)};
            } else {
                std::cerr << "invalid argument: " << args.at(i) << "\n";
                return 1;
            }
        }
    }

    if (input_file.empty()) {
        if (isatty(fileno(stdin))) {
            std::cerr << "no input file specified." << "\n";
            return 1;
        }

        std::string data{};
        std::string line{};
        while (std::getline(std::cin, line)) {
            data += line + "\n";
        }

        std::ofstream file{temp_file};
        if (!file.is_open()) {
            std::cerr << "failed to open temporary file: " << temp_file << "\n";
            return 1;
        }

        file << data;

        file.close();

        input_file = temp_file;
    }

    std::string html{};
    if (input_type == InputType::Markdown) {
        std::string command{"pandoc -f markdown -t html " + input_file};

        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            std::cerr << "pandoc failed to convert Markdown to HTML. is it installed?\n";
            if (input_file == temp_file) {
                std::remove(temp_file.data());
            }
            return 1;
        }

        char buffer[128];
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr) {
                html += buffer;
            }
        }

        pclose(pipe);
    } else {
        std::ifstream file{input_file};
        if (!file.is_open()) {
            std::cerr << "failed to open file: " << input_file << "\n";
            if (input_file == temp_file) {
                std::remove(temp_file.data());
            }
            return 1;
        }

        html = {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};

        file.close();
    }

    if (html.empty()) {
        std::cerr << "buffer is empty: " << input_file << "\n";

        if (input_file == temp_file) {
            std::remove(temp_file.data());
        }

        return 1;
    }

    if (std::filesystem::exists(temp_file)) {
        std::remove(temp_file.data());
    }

    bygg::HTML::Section section = bygg::HTML::Parser::parse_html_string(html);

    std::cout << (pseudocode ? bygg::HTML::generate_pseudocode(section) : section.get(formatting)) << "\n";

    return 0;
}