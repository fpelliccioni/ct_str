// Copyright (c) 2023 Fernando Pelliccioni.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <ct/str.hpp>

constexpr ct::str<5> hello("Hello");
constexpr ct::str<7> world(", World");
constexpr ct::str<12> combined = hello + world;
constexpr ct::str<5> default_constructed;

TEST_CASE("Compile-time tests") {
    // Test size
    static_assert(hello.size() == 5, "hello.size() failed");
    static_assert(world.size() == 7, "world.size() failed");
    static_assert(combined.size() == 12, "combined.size() failed");
    static_assert(default_constructed.size() == 5, "default_constructed.size() failed");

    // Test concatenation
    static_assert(combined == "Hello, World", "Concatenation test failed");

    // Test equality
    static_assert(hello == "Hello", "Equality test for hello failed");
    static_assert(world == ", World", "Equality test for world failed");
    static_assert(hello != world, "Inequality test failed");
    static_assert(default_constructed != "", "Equality test for default_constructed failed");
    static_assert(default_constructed == std::string_view("\0\0\0\0\0\0", 5), "Equality test for default_constructed failed");

    // Test access operator
    static_assert(hello[0] == 'H', "Access operator test for hello failed");
    static_assert(world[0] == ',', "Access operator test for world failed");

    // Other
    static_assert(ct::str<5>(std::string_view("Hello")) == "Hello", "Failed to create ct::str from string_view at compile-time");
    static_assert(ct::str("Hello") == "Hello", "Failed to create ct::str from string_view at compile-time");
}

TEST_CASE("Default construction of ct::str") {
    CHECK(std::string_view(default_constructed) == std::string_view("\0\0\0\0\0\0", 5));
}

TEST_CASE("Construction from string literal") {
    constexpr ct::str<5> hello("Hello");
    CHECK(std::string_view(hello) == "Hello");
}

TEST_CASE("Concatenation of ct::str") {
    constexpr auto hello = ct::str("Hello, ");
    constexpr auto world = ct::str("World!");
    constexpr auto result = hello + world;

    CHECK(std::string_view(result) == "Hello, World!");
}

TEST_CASE("Size of ct::str") {
    constexpr ct::str<5> hello("Hello");
    CHECK(hello.size() == 5);
}
