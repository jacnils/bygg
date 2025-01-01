/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <iostream>
#include <fstream>
#include <string_view>
#include <unistd.h>
#include <filesystem>
#include <sstream>
#include "IniManager.hpp"
#include <bygg/HTML/HTML.hpp>
#include <bygg/HTML/parser.hpp>

#ifndef BYGG_VERSION
#define BYGG_VERSION "0.0.0"
#endif

#ifdef _WIN32
        const std::string temp_file{"temp_data"};
        std::string config_file{};
#else
        const std::string temp_file{"/tmp/temp_data"};
        std::string config_file{};
#endif

enum class InputType {
    HTML,
    Markdown,
};

struct Config {
    bygg::HTML::Formatting formatting{bygg::HTML::Formatting::Pretty};
    bool pseudocode{false};
    InputType input_type{InputType::HTML};
    bygg::HTML::GeneratorOptions options{.include_main = false};
};

bool write_default_config(const std::string& file) {
    std::ofstream out{file};

    if (!out.is_open()) {
        return false;
    }

    out << "; bygg-cli configuration file\n";
    out << "; This file is used to store configuration options for bygg-cli\n";
    out << "; Feel free to edit this file, but be careful not to break the syntax\n";
    out << "\n";
    out << "; formatting options\n";
    out << "[formatting]\n";
    out << "type = pretty ; the type of formatting to use as output by default (none, pretty, newline, pseudo)\n";
    out << "input = html ; the input data type (html, markdown)\n";
    out << "\n";
    out << "; pseudocode options\n";
    out << "[pseudo]\n";
    out << "namespace_level = 0 ; using level (0, 1, 2)\n";
    out << "include_header = true ; include the necessary headers in the pseudocode\n";
    out << "header_path = bygg/bygg.hpp ; the path to the header file to include\n";
    out << "main = false ; include a main() function\n";
    out << "main_function = main ; the name of the main function\n";
    out << "main_return_type = int ; the return type of the main function\n";
    out << "main_args = 0 ; the arguments to pass to the main function (0, 1)\n";
    out << "sequence_mode = 1 ; how to handle sequences in strings (0, 1, 2)\n";
    out << "root_name = root ; the name of the root section\n";
    out << "use_tag_enums = true ; use tag enums instead of strings, if available\n";
    out << "use_empty_properties = false ; pass in an empty properties object if no properties are present\n";
    out << "use_empty_data = false ; pass in an empty data string if no data is present\n";
    out << "use_implicit_property = true ; utilize operator Property() to convert to Properties\n";
    out << "use_make_properties = true ; use make_properties instead of Properties constructor\n";
    out << "use_lists = false ; use SectionList/ElementList instead of variadic arguments\n";

    out.close();

    return true;
}

void handle_config_file(IniManager& ini, Config& config) {
    const auto get_value = [&](const std::string& section, const std::string& value) {
        try {
            return ini[section][value];
        } catch (std::exception&) {
            return std::string{};
        }
    };

    const auto& formatting = get_value("formatting", "type");
    config.pseudocode = false;
    if (formatting == "none") {
        config.formatting = bygg::HTML::Formatting::None;
    } else if (formatting == "newline") {
        config.formatting = bygg::HTML::Formatting::Newline;
    } else if (formatting == "pseudo") {
        config.formatting = bygg::HTML::Formatting::Pretty;
        config.pseudocode = true;
    } else {
        config.formatting = bygg::HTML::Formatting::Pretty;
    }

    const auto& input = get_value("formatting", "input");
    config.input_type = input == "markdown" ? InputType::Markdown : InputType::HTML;

    const auto& namespace_level = get_value("pseudo", "namespace_level");
    if (namespace_level == "1") {
        config.options.namespace_level = bygg::HTML::NamespaceLevel::ByggNamespace;
    } else if (namespace_level == "2") {
        config.options.namespace_level = bygg::HTML::NamespaceLevel::ByggHTMLNamespace;
    } else {
        config.options.namespace_level = bygg::HTML::NamespaceLevel::None;
    }

    const auto& include_header = get_value("pseudo", "include_header");
    config.options.include_header = include_header == "false";

    const auto& main = get_value("pseudo", "main");
    config.options.include_main = main == "true" ? true : false;

    const auto& header_path = get_value("pseudo", "header_path");
    if (!header_path.empty()) config.options.header_path = header_path;

    const auto& main_function = get_value("pseudo", "main_function");
    if (!main_function.empty()) config.options.main_function = main_function;

    const auto& main_return_type = get_value("pseudo", "main_return_type");
    if (!main_return_type.empty()) config.options.main_return_type = main_return_type;

    const auto& main_args = get_value("pseudo", "main_args");
    if (main_args == "1") {
        config.options.main_args = bygg::HTML::MainArgs::ArgcArgv;
    } else {
        config.options.main_args = bygg::HTML::MainArgs::None;
    }

    const auto& sequence_mode = get_value("pseudo", "sequence_mode");
    if (sequence_mode == "0") {
        config.options.sequence_mode = bygg::HTML::SequenceMode::Replace;
    } else if (sequence_mode == "2") {
        config.options.sequence_mode = bygg::HTML::SequenceMode::Leave;
    } else {
        config.options.sequence_mode = bygg::HTML::SequenceMode::Remove;
    }

    const auto& root_name = get_value("pseudo", "root_name");
    if (!root_name.empty()) config.options.header_path = root_name;

    const auto& use_tag_enums = get_value("pseudo", "use_tag_enums");
    config.options.use_tag_enums = use_tag_enums != "false";

    const auto& use_empty_properties = get_value("pseudo", "use_empty_properties");
    config.options.use_empty_properties = use_empty_properties != "false";

    const auto& use_empty_data = get_value("pseudo", "use_empty_data");
    config.options.use_empty_data = use_empty_data != "false";

    const auto& use_implicit_property = get_value("pseudo", "use_implicit_property");
    config.options.use_implicit_property = use_implicit_property != "false";

    const auto& use_make_properties = get_value("pseudo", "use_make_properties");
    config.options.use_make_properties = use_make_properties != "false";

    const auto& use_lists = get_value("pseudo", "use_lists");
    config.options.use_lists = use_lists != "false";
}

int main(int argc, char** argv) {
    Config config{};
    std::string input_file{};

#ifndef _WIN32
    config_file = std::string{std::getenv("HOME")} + "/";
    if (std::getenv("XDG_CONFIG_HOME")) {
        config_file = std::string{std::getenv("XDG_CONFIG_HOME")} + "/bygg-cli/bygg-cli.ini";
    } else {
        config_file += ".config/bygg-cli/bygg-cli.ini";
    }
#else
    config_file = std::string{std::getenv("APPDATA")} + "/bygg-cli/bygg-cli.ini";
#endif

    static const std::vector<std::string_view> args{argv, argv + argc};

    // first check for help, version, copyright and config file
    for (int i{1}; i < args.size(); ++i) {
        if (args.at(i) == "-h" || args.at(i) == "--help" || args.at(i) == "/h") {
            std::cout << "bygg-cli - a command-line interface for bygg\n";
            std::cout << "usage: bygg-cli [options] [file]\n";
            std::cout << "  -h, --help, /h: show this help message\n";
            std::cout << "  -v, --version, /v: show the version of bygg-cli\n";
            std::cout << "  -c, --copyright, /c: show the copyright information\n";
            std::cout << "  -f, -f=formatting, --formatting, --formatting=formatting, /f, /f=formatting: set the formatting type (none, pretty, newline, pseudo)\n";
            std::cout << "  -m, --main, /m: include a main function in the pseudocode\n";
            std::cout << "  -nm, --no-main, /nm: do not include a main function in the pseudocode\n";
            std::cout << "  -cf, -cf=config, --config, --config=config, /cf, /cf=config: set the configuration file\n";
            std::cout << "  -i, -i=input, --input, --input=input, /i, /i=input: set the input type (html, markdown)\n";
            std::cout << "  file: the file to read from\n";
            std::cout << "If no file is specified, input will be read from stdin\n";
            std::cout << "If no formatting type is specified, the default is what is specified in the configuration file, or pretty\n";
            std::cout << "If no input type is specified, the default is what is specified in the configuration file, or html\n";
            return 0;
        } else if (args.at(i) == "-v" || args.at(i) == "--version" || args.at(i) == "/v") {
            std::cout << "bygg-cli version " << BYGG_VERSION << "\n";
            return 0;
        } else if (args.at(i) == "-c" || args.at(i) == "--copyright" || args.at(i) == "/c") {
            std::cout << "bygg-cli - a command-line interface for bygg\n";
            std::cout << "Copyright(c) 2024-2025 - Jacob Nilsson & contributors\n";
            std::cout << "SPDX-License-Identifier: MIT\n";
            std::cout << "See https://jacobnilsson.com/bygg and the license document for copyright and licensing details.\n";
            return 0;
        } else if (args.at(i) == "-cf" || args.at(i) == "--config" || args.at(i) == "/cf") {
            if (argc >= i + 1) {
                config_file = std::string{args.at(i+1)};
            } else {
                std::cout << "no config file specified." << "\n";
                return 1;
            }

            ++i;
        } else if (args.at(i) == "-cf=" || args.at(i) == "--config=" || args.at(i) == "/cf=") {
            std::size_t pos{args.at(i).find('=')};

            if (pos + 1 == std::string::npos) {
                std::cerr << "invalid argument: " << args.at(i) << "\n";
                return 1;
            }

            config_file = std::string{args.at(i).substr(pos + 1)};
        }
    }

    // handle config file
    if (!std::filesystem::create_directories(std::filesystem::path{config_file}.parent_path()) &&
        !std::filesystem::exists(std::filesystem::path{config_file}.parent_path())) {
        throw std::runtime_error{"failed to create directory: " + std::string{config_file}};
    }
    if (!std::filesystem::exists(config_file)) {
        if (!write_default_config(config_file)) {
            std::cerr << "failed to write default configuration file: " << config_file << "\n";
            return 1;
        }

        if (!std::filesystem::exists(config_file)) {
            throw std::runtime_error{"failed to create configuration file: " + std::string{config_file}};
        }
    }

    try {
        IniManager ini;
        ini.load(config_file, true);
        handle_config_file(ini, config);
    } catch (std::exception&) {
        std::cerr << "failed to load configuration file: " << config_file << "\n";
        return 1;
    }

    // handle the rest of the arguments
    for (int i{1}; i < args.size(); ++i) {
        if (args.at(i) == "-h" || args.at(i) == "--help" || args.at(i) == "/h" ||
            args.at(i) == "-v" || args.at(i) == "--version" || args.at(i) == "/v" ||
            args.at(i) == "-c" || args.at(i) == "--copyright" || args.at(i) == "/c" ||
            args.at(i) == "-cf" || args.at(i) == "--config" || args.at(i) == "/cf") {
            continue;
        } else if (args.at(i) == "-f" || args.at(i) == "--formatting" || args.at(i) == "/f") {
            if (argc >= i + 1) {
                config.pseudocode = false;

                if (args.at(i+1) == "none") {
                    config.formatting = bygg::HTML::Formatting::None;
                } else if (args.at(i+1) == "pretty") {
                    config.formatting = bygg::HTML::Formatting::Pretty;
                } else if (args.at(i+1) == "newline") {
                    config.formatting = bygg::HTML::Formatting::Newline;
                } else if (args.at(i+1) == "pseudo") {
                    config.pseudocode = true;
                } else {
                    std::cerr << "invalid formatting type: " << args.at(i+1) << "\n";
                    std::cerr << "valid types: none, pretty, newline, pseudo" << "\n";
                    return 1;
                }
            } else {
                std::cout << "no formatting type specified." << "\n";
                return 1;
            }

            ++i;
        } else if (args.at(i) == "-f=none" || args.at(i) == "--formatting=none" || args.at(i) == "/f=none") {
            config.formatting = bygg::HTML::Formatting::None;
            config.pseudocode = false;
        } else if (args.at(i) == "-f=pretty" || args.at(i) == "--formatting=pretty" || args.at(i) == "/f=pretty") {
            config.formatting = bygg::HTML::Formatting::Pretty;
            config.pseudocode = false;
        } else if (args.at(i) == "-f=newline" || args.at(i) == "--formatting=newline" || args.at(i) == "/f=newline") {
            config.formatting = bygg::HTML::Formatting::Newline;
            config.pseudocode = false;
        } else if (args.at(i) == "-f=pseudo" || args.at(i) == "--formatting=pseudo" || args.at(i) == "/f=pseudo") {
            config.pseudocode = true;
        } else if (args.at(i) == "-m" || args.at(i) == "--main" || args.at(i) == "/m") {
            config.options.include_main = true;
        } else if (args.at(i) == "-nm" || args.at(i) == "--no-main" || args.at(i) == "/nm") {
            config.options.include_main = false;
        } else if (args.at(i) == "-i" || args.at(i) == "--input" || args.at(i) == "/i") {
            if (argc >= i + 1) {
                if (args.at(i+1) == "html") {
                    config.input_type = InputType::HTML;
                } else if (args.at(i+1) == "markdown") {
                    config.input_type = InputType::Markdown;
                } else {
                    std::cerr << "invalid input type: " << args.at(i+1) << "\n";
                    std::cerr << "valid types: html, markdown" << "\n";
                    return 1;
                }
            } else {
                std::cout << "no input type specified." << "\n";
                return 1;
            }

            ++i;
        } else if (args.at(i) == "-i=html" || args.at(i) == "--input=html" || args.at(i) == "/i=html") {
            config.input_type = InputType::HTML;
        } else if (args.at(i) == "-i=markdown" || args.at(i) == "--input=markdown" || args.at(i) == "/i=markdown") {
            config.input_type = InputType::Markdown;
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
    if (config.input_type == InputType::Markdown) {
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

    std::cout << (config.pseudocode ? bygg::HTML::generate_pseudocode(section, config.options) : section.get(config.formatting)) << "\n";

    return 0;
}