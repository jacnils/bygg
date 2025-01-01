/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

#include <bygg/version.hpp>

std::tuple<bygg::integer_type, bygg::integer_type, bygg::integer_type> bygg::version() {
#ifdef BYGG_VERSION
    bygg::string_type version{BYGG_VERSION};

    if (version.find('.') != bygg::string_type::npos) {
        bygg::string_type major = version.substr(0, version.find('.'));
        version = version.substr(version.find('.') + 1);

        if (version.find('.') != bygg::string_type::npos) {
            bygg::string_type minor = version.substr(0, version.find('.'));
            version = version.substr(version.find('.') + 1);

            if (version.find('.') != bygg::string_type::npos) {
                bygg::string_type patch = version.substr(0, version.find('.'));
                return {std::stoi(major), std::stoi(minor), std::stoi(patch)};
            }
        }
    }
#endif
    return {};
}
