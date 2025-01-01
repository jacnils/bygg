/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024-2025 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */

// NOLINTBEGIN
#include <src/version.cpp>
#include <src/CSS/properties.cpp>
#include <src/CSS/property.cpp>
#include <src/CSS/element.cpp>
#include <src/CSS/stylesheet.cpp>
#include <src/CSS/color.cpp>
#include <src/CSS/impl/color_conversions.cpp>
#include <src/HTML/document.cpp>
#include <src/HTML/element.cpp>
#include <src/HTML/properties.cpp>
#include <src/HTML/property.cpp>
#include <src/HTML/section.cpp>
#include <src/HTML/tag.cpp>
#include <src/HTML/pseudocode_generator.cpp>
#include <src/HTML/content_formatter.cpp>
#ifdef BYGG_USE_LIBXML2
#include <src/HTML/parser.cpp>
#include <src/HTML/impl/libxml2.cpp>
#endif
// NOLINTEND