// Copyright (c) 2023 Fernando Pelliccioni.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <utility>

/// Shorthand for a type-list-template-template parameter
#define ct_ttparam template <class...> class

/// Shorthand for a value-list-template-template parameter
#define ct_vltparam template <auto...> class

namespace ct {

template <typename T>
using ptr = T*;

template <auto V>
struct val {
    constexpr static auto value = V;
};

}  // namespace ct
