#pragma once

// TODO: implement render_v

#include "../../lex/token.hpp"

namespace ct::url::ast {

using ct::lex::token;

template <token N>
struct name {};

template <token N>
struct scheme {};

template <token N>
struct host {};

template <uint64_t N>
struct port {};

template <token N>
struct path {};

template <token Key, token Value>
struct query_param {};

template <token N>
struct fragment {};

template <typename... Parts>
struct parts {};

}  // namespace ct::url::ast

