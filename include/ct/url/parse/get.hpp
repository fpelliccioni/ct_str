#pragma once

#include <array>
#include <string_view>

#include "../../basics.hpp"
#include "../../list.hpp"
#include "../../str.hpp"

#include "./ast.hpp"
#include "../lex/lex.hpp"

// namespace lex = ct::url::lex;
// using ct::lex::token;
using ct::list;
using ct::head;
using u64 = std::uint64_t;
using u32 = std::uint32_t;

namespace ct::url::parse {

// -----------------------------------------------------------------------------
template <typename T>
struct get_value;

template <token N>
struct get_value<ct::url::ast::name<N>> {
    static constexpr auto value = N;
};

template <token N>
struct get_value<ct::url::ast::scheme<N>> {
    static constexpr auto value = N;
};

template <token N>
struct get_value<ct::url::ast::host<N>> {
    static constexpr auto value = N;
};

template <uint64_t N>
struct get_value<ct::url::ast::port<N>> {
    static constexpr auto value = N;
};

template <token N>
struct get_value<ct::url::ast::path<N>> {
    static constexpr auto value = N;
};

template <token N>
struct get_value<ct::url::ast::fragment<N>> {
    static constexpr auto value = N;
};

template <token Key, token Value>
struct get_value<ct::url::ast::query_param<Key, Value>> {
    static constexpr auto value = std::pair{Key, Value};
};

// -----------------------------------------------------------------------------
template <template <auto...> typename Target, typename T>
struct is_instance_of : std::false_type {};

template <template <auto...> typename Target, auto... Args>
struct is_instance_of<Target, Target<Args...>> : std::true_type {};

// -----------------------------------------------------------------------------

namespace detail {

// -----------------------------------------------------------------------------
template <size_t Index, bool... Bools>
struct find_index_if_impl;

template <size_t Index>
struct find_index_if_impl<Index> {
    static constexpr size_t value = -1;
};

template <size_t Index, bool... Rest>
struct find_index_if_impl<Index, true, Rest...> {
    static constexpr size_t value = Index;
};

template <size_t Index, bool... Rest>
struct find_index_if_impl<Index, false, Rest...> : find_index_if_impl<Index + 1, Rest...> {};


// -----------------------------------------------------------------------------
template <size_t Index, size_t Current, bool... Bools> struct find_nth_type_if_impl;

template <size_t Index, size_t Current>
struct find_nth_type_if_impl<Index, Current> {
    static_assert(Current < 0, "Index out of bounds");
};

template <size_t Index, size_t Current, bool First, bool... Rest>
struct find_nth_type_if_impl<Index, Current, First, Rest...> :
    std::conditional_t<
        First && Current == 0,
        std::integral_constant<size_t, Index>,
        find_nth_type_if_impl<Index + 1, Current - First, Rest...>
    > {};

// -----------------------------------------------------------------------------
template <bool... Bools>
struct count_if_impl;

template <>
struct count_if_impl<> {
    static constexpr size_t value = 0;
};

template <bool First, bool... Rest>
struct count_if_impl<First, Rest...> {
    static constexpr size_t value = First + count_if_impl<Rest...>::value;
};

template <bool... Bools>
using count_if = count_if_impl<Bools...>;

}  // namespace detail


// -----------------------------------------------------------------------------
template <bool... Bools>
using find_index_if = detail::find_index_if_impl<0, Bools...>;


// -----------------------------------------------------------------------------
template <template <auto...> typename T, typename Parts>
struct count;

template <template <auto...> typename T, typename... Types>
struct count<T, ct::url::ast::parts<Types...>> {
    static constexpr size_t value = detail::count_if<is_instance_of<T, Types>::value...>::value;
};


// -----------------------------------------------------------------------------
namespace detail {

template <template <auto...> typename T, typename Parts>
struct get_impl;

template <template <auto...> typename T, typename... Types>
struct get_impl<T, ct::url::ast::parts<Types...>> {
    static constexpr size_t idx = find_index_if<is_instance_of<T, Types>::value...>::value;

    static_assert(idx != static_cast<size_t>(-1), "Type template not found in the URL parts!");

    using type = typename std::type_identity<
        std::tuple_element_t<idx, std::tuple<Types...>>
    >::type;

    static constexpr auto value = get_value<type>::value;
};

// -----------------------------------------------------------------------------
template <typename Parts, template <auto...> typename T, size_t Index>
struct get_query_param_impl;

template <template <auto...> typename T, typename... Types, size_t Index>
struct get_query_param_impl<ct::url::ast::parts<Types...>, T, Index> {
    static constexpr size_t idx = detail::find_nth_type_if_impl<0, Index, is_instance_of<T, Types>::value...>::value;

    using type = typename std::type_identity<
        std::tuple_element_t<idx, std::tuple<Types...>>
    >::type;

    static constexpr auto value = get_value<type>::value;
};

} // namespace detail

template <typename Parts, template <auto...> typename T>
constexpr auto get() {
    return detail::get_impl<T, Parts>::value;
}

template <typename Parts, template <auto...> typename T, size_t Index>
constexpr auto get() {
    return detail::get_query_param_impl<Parts, T, Index>::value;
}

}  // namespace ct::url::parse


