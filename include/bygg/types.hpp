/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <exception>

namespace bygg {
#ifndef BYGG_STRING_TYPE
    using string_type = std::string;
#else
    using string_type = BYGG_STRING_TYPE;
#endif
#ifndef BYGG_SIZE_TYPE
    using size_type = std::size_t;
#else
    using size_type = BYGG_SIZE_TYPE;
#endif
#ifndef BYGG_EXCEPTION_CLASS
    using exception_type = std::exception;
#else
    using exception_type = BYGG_EXCEPTION_CLASS;
#endif
#ifndef BYGG_INTEGER_TYPE
    using integer_type = int;
#else
    using integer_type = BYGG_INTEGER_TYPE;
#endif
} // namespace bygg
