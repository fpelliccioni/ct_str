// Copyright (c) 2023 Fernando Pelliccioni.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include "./meta.hpp"
#include "../str.hpp"

#include <array>
#include <cassert>
#include <string_view>
#include <utility>

namespace ct::url::lex {

constexpr static std::size_t max_token_length = 256;

/**
 * @brief Represents a URL token.
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
    constexpr char        operator[](std::size_t n) const noexcept {
        assert(n < max_token_length);
        return _chars[n];
    }

    bool operator==(token const&) const = default;
};

template <token... Tokens>
struct token_list;

constexpr bool is_digit(char c) { return c <= '9' and c >= '0'; }
constexpr bool is_alpha(char c) { return (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z'); }

constexpr bool is_unreserved(char c) {
    return is_alpha(c) || is_digit(c) || c == '-' || c == '.' || c == '_' || c == '~';
}

constexpr bool is_reserved(char c) {
    switch (c) {
        // gen-delims
        case ':':
        case '/':
        case '?':
        case '#':
        case '[':
        case ']':
        case '@':
        // sub-delims
        case '!':
        case '$':
        case '&':
        case '\'':
        case '(':
        case ')':
        case '*':
        case '+':
        case ',':
        case ';':
        case '=':
            return true;
        default:
            return false;
    }
}

constexpr bool is_hex_digit(char c) {
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

namespace detail {

// using meta::list;

constexpr auto fin_token(char const* s, std::size_t len) {
    token ret;
    for (auto i = 0u; i < len; ++i) {
        ret._chars[i] = s[i];
    }
    return ret;
}

/**
 * @brief Represents the position of a token within a larger string
 *
 */
struct token_range {
    /// The beginning offset of the token
    uint32_t pos;
    /// The length of the token (in char)
    // uint8_t len;
    uint16_t len;
};

/**
 * @brief Obtain the next token_range from the given string at the specified offset
 *
 * @param begin A pointer to the very beginning of the source string
 * @param begin_pos The offset within the string to begin searching for another token
 * @return constexpr token_range
 */
constexpr token_range next_token(char const* const begin, uint32_t start_pos) {
    auto it = begin + start_pos;
    // The beginning index of the token within the string view:
    uint32_t pos = start_pos;
    // // Skip leading whitespace
    // while (*it == ' ' or *it == '\n') {
    //     ++it;
    //     ++pos;
    // }

    char const first = *it;

    // Checking for spaces and treating them as invalid
    if (first == ' ') {
        throw "Space is an invalid character within a URL.";
    }

    // URI reserved characters
    if (is_reserved(first)) {
        return {pos, 1};
    }

    if (first == '%') {
        if ( ! is_hex_digit(*(it + 1)) || ! is_hex_digit(*(it + 2))) {
            throw "Invalid percent encoding.";
        }
        return {pos, 3};
    }

    if (is_unreserved(first)) {
        ++it;
        uint8_t len = 1;
        while (is_unreserved(*it)) {
            ++it;
            ++len;
        }
        return {pos, len};
    } else if (is_digit(first)) {
        auto num_begin = it;
        ++it;
        while (is_digit(*it)) {
            ++it;
        }
        auto len = static_cast<uint8_t>(it - num_begin);
        return {pos, len};
    }
    if ( ! *it) {
        return {pos, 0};  // End case: Return an EOF
    }

    // Treat any other character as invalid.
    throw "Invalid character encountered in URL.";

    return {pos, 1};  // Other characters
}

template <std::size_t N>
struct tokenize_result {
    std::array<token_range, N> tokens;
    int num_tokens;
};

template <std::size_t N>
constexpr tokenize_result<N> tokenize(char const* _chars) {
    tokenize_result<N> ret = {};
    ret.num_tokens = tokenize(ret.tokens.data(), _chars);
    return ret;
}

constexpr int tokenize(token_range* into, char const* const cptr) {
    int n_tokens = 0;
    uint32_t off = 0;
    while (true) {
        auto& tr = *into++ = detail::next_token(cptr, off);
        if (tr.len == 0) {
            break;
        }
        ++n_tokens;
        off = tr.pos + tr.len;
    }
    return n_tokens;
}

constexpr token take_token(char const* str_begin, token_range const& r) {
    return detail::fin_token(str_begin + r.pos, r.len);
}

template <ct::str String, tokenize_result Result, std::size_t... I>
auto prune_f(std::index_sequence<I...>*)
    -> token_list<take_token(String.data(), Result.tokens[I])...>;

template <ct::str String>
auto tokenize() {
    constexpr auto res = tokenize<String.size() + 1>(String.data());
    return meta::ptr<decltype(detail::prune_f<String, res>(
        (std::make_index_sequence<res.num_tokens>*)(nullptr)))>{};
}

}  // namespace detail

template <ct::str String>
using tokenize_t = std::remove_pointer_t<decltype(detail::tokenize<String>())>;

}  // namespace ct::url::lex
