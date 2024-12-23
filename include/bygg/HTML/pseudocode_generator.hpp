/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>

#include <bygg/types.hpp>
#include <bygg/HTML/section.hpp>

namespace bygg::HTML {
    /**
    * @brief The method used to handle sequences found in strings
    */
    enum class SequenceMode {
        Replace,
        Remove,
        Leave,
    };
    /**
     * @brief Options for the pseudocode generator.
     */
    struct GeneratorOptions {
        bool use_tag_enums{true}; /* Use tag enums instead of strings, if available */
        bool use_empty_properties{false}; /* Pass in an empty properties object if no properties are present */
        bool use_implicit_property{true}; /* Utilize operator Property() to convert to Properties */
        bool use_make_properties{true}; /* Use make_properties instead of Properties constructor */
        bool use_lists{false}; /* Use SectionList/ElementList instead of variadic arguments */
        bool include_main{true}; /* Include a main function and include the necessary headers in the pseudocode */
        SequenceMode sequence_mode{SequenceMode::Remove}; /* How to handle sequences in strings */
    };
    /**
    * @brief Generate C++ bygg pseudocode from an HTML section. Useful if you want to read in a file once and make changes to it in C++.
    * @param section The section to generate pseudocode from
    * @param options The options to use when generating pseudocode
    * @return string_type The pseudocode
    */
    string_type generate_pseudocode(const Section& section, const GeneratorOptions& options = GeneratorOptions{});
    /**
     * @brief Generate C++ bygg pseudocode from an HTML section. Useful if you want to read in a file once and make changes to it in C++.
     * @param section The section to generate pseudocode from
     * @param options The options to use when generating pseudocode
     * @return T The pseudocode in the form of a specific type
     */
    template <typename T> T generate_pseudocode(const Section& section, const GeneratorOptions& options = GeneratorOptions{}) {
        return T(generate_pseudocode(section, options));
    }
} // namespace bygg