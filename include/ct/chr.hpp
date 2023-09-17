// Copyright (c) 2023 Fernando Pelliccioni.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

namespace ct {

constexpr bool is_digit(char c) { return c <= '9' and c >= '0'; }

constexpr bool is_alpha(char c) { return (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z'); }

constexpr bool is_hex_digit(char c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

} // namespace ct