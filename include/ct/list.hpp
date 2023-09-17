// Copyright (c) 2023 Fernando Pelliccioni.
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <utility>

#include "./basics.hpp"

namespace ct {

template <typename... Ts>
struct list;

// -----------------------------------------------------------------------------

template <typename L>
struct head_tail;

template <ct_ttparam L, typename H, typename... Ts>
struct head_tail<L<H, Ts...>> {
    using head = H;
    using tail = L<Ts...>;
};

template <ct_ttparam L, typename H, typename S, typename... Ts>
struct head_tail<L<H, S, Ts...>> {
    using head   = H;
    using second = S;
    using tail   = L<S, Ts...>;
};


/**
 * @brief Obtain the first type in the given typelist
 */
template <typename L>
using head = typename head_tail<L>::head;

/**
 * @brief Obtain the remainder of a typelist following the head element
 */
template <typename L>
using tail = typename head_tail<L>::tail;


// -----------------------------------------------------------------------------

template <ct_ttparam L, typename... Ts, typename V>
auto push_front_fn(L<Ts...> const&, V const&) -> L<V, Ts...>;

template <typename L, typename V>
using push_front = decltype(push_front_fn(std::declval<L const&>(), std::declval<V const&>()));



// -----------------------------------------------------------------------------

namespace tacit {

struct clear {
    template <typename L>
    struct impl;

    template <ct_ttparam L, typename... Ts>
    struct impl<L<Ts...>> {
        using type = L<>;
    };

    template <typename L>
    using f = typename impl<L>::type;
};

}  // namespace tacit

template <typename L>
using clear = tacit::clear::f<L>;


namespace detail {

constexpr uint64_t sa_log2fn(uint64_t n) {
    uint64_t r = 0;
    for (; n > 1; n >>= 1) {
        ++r;
    }
    return r;
}

template <uint64_t N>
struct sa_log2 {
    constexpr static uint64_t value = sa_log2fn(N);
};

template <uint64_t N, typename Acc, typename More, uint64_t Log2 = sa_log2<N>::value>
struct split_at_;

// Final:
template <ct_ttparam L, typename... Acc, typename... Ts>
struct split_at_<0, L<Acc...>, L<Ts...>, 0> {
    using type = L<L<Acc...>, L<Ts...>>;
};

// One more:
template <ct_ttparam L, typename... Acc, typename Head, typename... Tail>
struct split_at_<1, L<Acc...>, L<Head, Tail...>, 0> {
    using type = L<L<Acc..., Head>, L<Tail...>>;
};

// At least two more:
template <uint64_t N, ct_ttparam L, typename... Acc, typename A, typename B, typename... Tail>
struct split_at_<N, L<Acc...>, L<A, B, Tail...>, 1>
    : split_at_<N - 2, L<Acc..., A, B>, L<Tail...>> {};

// At least four more:
template <uint64_t N, ct_ttparam L, typename... Acc, typename A, typename B, typename C, typename D, typename... Tail>
struct split_at_<N, L<Acc...>, L<A, B, C, D, Tail...>, 2>
    : split_at_<N - 4, L<Acc..., A, B, C, D>, L<Tail...>> {};

// At least eight more:
template <uint64_t         N,
          ct_ttparam L,
          typename... Acc,
          typename A,
          typename B,
          typename C,
          typename D,
          typename E,
          typename F,
          typename G,
          typename H,
          typename... Tail>
struct split_at_<N, L<Acc...>, L<A, B, C, D, E, F, G, H, Tail...>, 3>
    : split_at_<N - 8, L<Acc..., A, B, C, D, E, F, G, H>, L<Tail...>> {};

template <uint64_t         N,
          uint64_t         Log2,
          ct_ttparam L,
          typename... Acc,
          typename T0,
          typename T1,
          typename T2,
          typename T3,
          typename T4,
          typename T5,
          typename T6,
          typename T7,
          typename T8,
          typename T9,
          typename Ta,
          typename Tb,
          typename Tc,
          typename Td,
          typename Te,
          typename Tf,
          typename... Tail>
    requires(Log2 > 3)  //
struct split_at_<N,
                 L<Acc...>,
                 L<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, Ta, Tb, Tc, Td, Te, Tf, Tail...>,
                 Log2>
    : split_at_<N - 16,
                L<Acc..., T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, Ta, Tb, Tc, Td, Te, Tf>,
                L<Tail...>> {};

}  // namespace detail

template <typename L, uint64_t N>
using split_at = typename detail::split_at_<N, clear<L>, L>::type;


// -----------------------------------------------------------------------------


namespace tacit {

template <typename L>
constexpr auto len_v = nullptr;

template <ct_ttparam L, typename... Ts>
constexpr std::size_t len_v<L<Ts...>> = sizeof...(Ts);

template <ct_ttparam L, typename... Ts>
constexpr std::size_t len_v<const L<Ts...>> = sizeof...(Ts);

template <ct_vltparam L, auto... Vs>
constexpr std::size_t len_v<L<Vs...>> = sizeof...(Vs);

template <ct_vltparam L, auto... Vs>
constexpr std::size_t len_v<const L<Vs...>> = sizeof...(Vs);

struct len {
    template <typename L>
    using f = ct::val<len_v<L>>;
};

}  // namespace tacit

/**
 * Get the length of in a list-like template specialization.
 *
 * Exposes a nested member ::valuee specifying the list of the list-list type
 */
template <typename L>
using len = tacit::len::f<L>;

template <typename L>
constexpr std::size_t len_v = tacit::len_v<L>;


// -----------------------------------------------------------------------------

namespace detail {

template <typename T, std::size_t N>
struct type_reverser_base {};

template <typename L, typename Seq = std::make_index_sequence<ct::len_v<L>>>
struct reverser_derived;

template <template <class...> class L, typename... Ts, std::size_t... Ns>
struct reverser_derived<L<Ts...>, std::index_sequence<Ns...>> : type_reverser_base<Ts, Ns>... {};

template <std::size_t N>
struct reverser_ns {
    template <typename T>
    static auto apply(type_reverser_base<T, N>*...) -> T;
};

}  // namespace detail

template <typename L,
          std::size_t Len = ct::len_v<L>,
          typename Seq    = std::make_index_sequence<Len>,
          typename D      = detail::reverser_derived<L, Seq>>
struct reverser;

template <template <class...> class L,
          typename... Ts,
          std::size_t Len,
          std::size_t... Ns,
          typename D>
struct reverser<L<Ts...>, Len, std::index_sequence<Ns...>, D> {
    using type = L<decltype(detail::reverser_ns<sizeof...(Ns) - Ns - 1>  //
                            ::apply(static_cast<D*>(nullptr)))...>;
};

/**
 * @brief Reverse the elements of a type-list
 */
template <typename L>
using reverse = typename reverser<L>::type;

// -----------------------------------------------------------------------------

namespace tacit {

template <ct_ttparam NewList>
struct rebind {
    template <typename L>
    struct impl;

    template <ct_ttparam OldList, typename... Ts>
    struct impl<OldList<Ts...>> {
        using type = NewList<Ts...>;
    };

    template <typename L>
    using f = typename impl<L>::type;
};

}  // namespace tacit

/**
 * @brief Copy the template arguments from L an apply them to the class template NewL
 *
 * @tparam L An input list or other class template
 * @tparam NewL The class template to specialize with L's arguments.
 */
template <typename L, ct_ttparam NewL>
using rebind = typename tacit::rebind<NewL>::template f<L>;

}  // namespace ct
