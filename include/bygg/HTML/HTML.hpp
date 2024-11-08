/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <bygg/types.hpp>
#include <bygg/except.hpp>

#include <bygg/HTML/tag.hpp>
#include <bygg/HTML/type_enum.hpp>
#include <bygg/HTML/formatting_enum.hpp>
#include <bygg/HTML/property.hpp>
#include <bygg/HTML/properties.hpp>
#include <bygg/HTML/element.hpp>
#include <bygg/HTML/section.hpp>
#include <bygg/HTML/document.hpp>
#include <bygg/HTML/pseudocode_generator.hpp>
#include <bygg/HTML/content_formatter.hpp>
#ifdef BYGG_USE_LIBXML2
#include <bygg/HTML/parser.hpp>
#endif
