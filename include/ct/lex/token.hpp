// Copyright (c) 2023 Fernando Pelliccioni.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <cassert>
#include <string_view>

namespace ct::lex {

constexpr static std::size_t max_token_length = 256;

/**
 * @brief Represents a token.
 *
 * Structural and valid as a non-type template parameter
 */
struct token {
    char _chars[max_token_length + 1] = {};

    constexpr token() = default;

    constexpr token(char const* s) {
        auto        dst = _chars;
        std::size_t n   = 0;
        while (*s) {
            ++n;
            assert(n <= max_token_length);
            *dst++ = *s++;
        }
    }

    constexpr operator std::string_view() const noexcept { return std::string_view(_chars); }

    constexpr char const* data() const noexcept { return _chars; }
    constexpr std::size_t size() const noexcept { return std::char_traits<char>::length(_chars); }
    constexpr char operator[](std::size_t n) const noexcept {
        assert(n < max_token_length);
        return _chars[n];
    }

    constexpr bool operator==(token const&) const = default;

    constexpr token operator+(token const& other) const {
        token ret = *this;
        auto  dst = ret._chars + size();
        auto  src = other._chars;
        while (*src) {
            *dst++ = *src++;
        }
        return ret;
    }
};

template <token... Tokens>
struct token_list;

}  // namespace ct::lex
