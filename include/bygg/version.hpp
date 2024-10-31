/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <tuple>
#include <bygg/types.hpp>

namespace bygg {
    /**
     * @brief Get the version of the library
     * @return std::tuple<int, int, int> The version of the library
     */
    std::tuple<integer_type, integer_type, integer_type> version();
} // namespace bygg
