/*
 * bygg - Component-based HTML/CSS builder for C++
 *
 * Copyright 2024 - Jacob Nilsson & contributors
 * SPDX-License-Identifier: MIT
 */
#pragma once

namespace bygg::HTML {
    /**
     * @brief Enum for element types.
     */
    enum class Type {
        Text, /* Text element with tab characters appropriately prepended (my text here). Note that this does *not* append a newline character. */
        Text_No_Formatting, /* Text element with no formatting (my text here). */
        Opening, /* Opening element (<tag>) */
        Closing, /* Closing element (</tag>) */
        Data, /* Data element (<tag>data</tag>) */
        Standalone, /* Standalone element (<tag data="data" />) */

        /* Aliases; for compatibility */
        Non_Self_Closing [[deprecated("Non_Self_Closing is deprecated; use Data instead.")]] = Data,
        Self_Closing [[deprecated("Self_Closing is deprecated; use Standalone instead.")]] = Standalone,
        Non_Closed [[deprecated("Non_Closed is deprecated; use Opening instead.")]] = Opening,
        Non_Opened [[deprecated("Non_Opened is deprecated; use Closing instead.")]] = Closing
    };
} // namespace bygg::HTML