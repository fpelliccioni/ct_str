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
using token = ct::lex::token;

namespace ct::url::parse {

namespace detail {

constexpr u64 join_u32s_to_u64(u32 high, u32 low) {
    return (static_cast<u64>(high) << 32) | static_cast<u64>(low);
}

constexpr u32 extract_higher_u32(u64 value) {
    return static_cast<u32>(value >> 32);
}

constexpr u32 extract_lower_u32(u64 value) {
    return static_cast<u32>(value & 0xFFFFFFFF);
}

constexpr u64 encode_path_info(u32 start_index, u32 token_count) {
    return join_u32s_to_u64(start_index, token_count);
}

constexpr u64 decode_start_index(u64 encoded) {
    return extract_higher_u32(encoded);
}

constexpr u64 decode_token_count(u64 encoded) {
    return extract_lower_u32(encoded);
}

enum ukind {
    k_done = 0,
    k_scheme,
    // k_username,      // TODO
    // k_password,
    k_host,
    k_port,
    k_path,
    k_query,
    k_fragment,

    k_parts,
};

// A node in the URL construction
struct node {
    ukind kind;
    u64 n;
};

template <auto Tokens>
struct executor {
    template <ukind K, typename = void>
    struct step;

    template <ukind K, typename = void>
    struct parse;

    template <auto Nodes, u64 Idx = 0, typename Stack = list<>>
    using exec_parse_t = typename parse<Nodes[Idx].kind, void>::template f<Nodes, Idx, Stack>;

    template <typename Void>
    struct parse<k_done, Void> {
        template <auto, auto, typename Stack>
        using f = head<Stack>;
    };

    template <ukind K, typename Void>
    struct parse {
        using MyStep = step<K>;
        // Recursive case:
        template <auto Nodes,
                  auto Idx,
                  // The stack that we receive:
                  typename StackIn,
                  // Apply the step function to the node and receive the transformed stack:
                  typename StackOut = typename MyStep::template f<Nodes[Idx].n, StackIn>>
        // Recurse into the parser, to the next node instruction, with the new stack:
        using f = exec_parse_t<Nodes, Idx + 1, StackOut>;
    };

    template <typename Void>
    struct step<k_scheme, Void> {
        template <u64 N, typename Stack>
        using f = ct::push_front<Stack, ct::url::ast::scheme<Tokens[N]>>;
    };

    template <typename Void>
    struct step<k_host, Void> {
        template <u64 N, typename Stack>
        using f = ct::push_front<Stack, ct::url::ast::host<Tokens[N]>>;
    };

    template <typename Void>
    struct step<k_port, Void> {
        constexpr static uint64_t parse_int(std::string_view sv) {
            uint64_t ret = 0;
            for (auto it = sv.begin(); it != sv.end(); ++it) {
                ret *= 10;
                ret += *it - '0';
            }
            return ret;
        }
        template <u64 N, typename Stack>
        using f = ct::push_front<Stack, ct::url::ast::port<parse_int(std::string_view(Tokens[N]))>>;
    };

    template <typename Void>
    struct step<k_path, Void> {
        template <u64 N, std::size_t... Indices>
        constexpr static auto concat_tokens(std::index_sequence<Indices...>) {
            return (... + Tokens[decode_start_index(N) + Indices]);
        }

        template <u64 N, typename Stack>
        using f = ct::push_front<Stack, ct::url::ast::path<concat_tokens<N>(std::make_index_sequence<static_cast<std::size_t>(decode_token_count(N))>{})>>;
    };

    template <typename Void>
    struct step<k_query, Void> {
        template <u64 N>
        constexpr static auto extract_key() {
            u32 key_start = extract_higher_u32(N);
            return Tokens[key_start];
        }

        template <u64 N>
        constexpr static auto extract_value() {
            u32 value_start = extract_lower_u32(N);
            return Tokens[value_start];
        }

        template <u64 N, typename Stack>
        using f = ct::push_front<Stack, ct::url::ast::query_param<extract_key<N>(), extract_value<N>()>>;
    };

    template <typename Void>
    struct step<k_fragment, Void> {
        template <u64 N, typename Stack>
        using f = ct::push_front<Stack, ct::url::ast::fragment<Tokens[N]>>;
    };

    template <typename Void>
    struct step<k_parts, Void> {
        template <u64 Count,
                  typename Stack,
                  typename Split = ct::split_at<Stack, Count>,
                  typename Head  = ct::head<Split>,
                  typename Tail  = ct::tail<Split>,
                  typename Seq   = ct::rebind<ct::reverse<Head>, ct::url::ast::parts>>
        using f = ct::push_front<Tail, Seq>;
    };
};

struct token_iter {
    token const* _base;
    u64 pos = 0;
    constexpr const token& get() const noexcept { return _base[pos]; }
};

struct url_parser {
    constexpr static void parse_path(node*& into, token_iter& it) {
        // precondition: std::string_view(it.get()) == "/"

        auto tk = std::string_view(it.get());
        ++it.pos;

        if (std::string_view(it.get()).empty() || std::string_view(it.get()) == "?" || std::string_view(it.get()) == "#") {
            *into++ = {k_path, encode_path_info(it.pos - 1, 1)};
            return;
        }

        u32 start_pos = static_cast<u32>(it.pos - 1);
        u32 token_count = 1;

        while (true) {
            tk = std::string_view(it.get());

            if (tk == "/") {
                ++it.pos;
                ++token_count;
                continue;
            }

            if (tk.empty() || tk == "?" || tk == "#") {
                break;
            }

            ++it.pos;
            ++token_count;
        }
        *into++ = {k_path, encode_path_info(start_pos, token_count)};
    }


    constexpr static u64 parse_query(node*& into, token_iter& it) {
        //precondition: std::string_view(it.get()) == "?"
        it.pos++;
        u64 param_count = 0;

        while (true) {
            auto tk = std::string_view(it.get());
            if (tk.empty() || tk == "#") {
                break;
            }
            if ( ! ct::is_alpha(tk[0])) {
                throw "Invalid key in query";
            }
            auto key_pos = static_cast<u64>(it.pos);
            ++it.pos;

            if (std::string_view(it.get()) != "=") {
                throw "Invalid query format";
            }
            ++it.pos;

            if (std::string_view(it.get()).empty()) {
                throw "Invalid value in query";
            }
            *into++ = {k_query, join_u32s_to_u64(key_pos, static_cast<u64>(it.pos))};
            ++it.pos;
            ++param_count;

            if (std::string_view(it.get()) == "&") {
                ++it.pos;
            }
        }
        return param_count;
    }

    constexpr static void parse_parts(node*& into, token_iter& it) {
        auto tk = std::string_view(it.get());

        u64 n_parts = 0;

        // Detect scheme
        if (ct::is_alpha(tk.front())) {
            *into++ = {k_scheme, static_cast<u64>(it.pos)};
            ++it.pos;
            ++n_parts;
        }

        // Detect "://"
        if (std::string_view(it.get()) != ":") {
            throw "Invalid URL: missing scheme";
        }
        ++it.pos;

        if (std::string_view(it.get()) != "/") {
            throw "Invalid URL: missing host";
        }
        ++it.pos;

        if (std::string_view(it.get()) != "/") {
            throw "Invalid URL: missing host";
        }
        ++it.pos;

        // Now detect the host
        if (std::string_view(it.get()).empty()) {
            throw "Invalid URL: empty host";
        }
        *into++ = {k_host, static_cast<u64>(it.pos)};
        ++it.pos;
        ++n_parts;

        if (std::string_view(it.get()) == ":") {
            ++it.pos;
            tk = std::string_view(it.get());
            if (tk.empty()) {
                throw "Invalid URL: empty port";
            }

            *into++ = {k_port, static_cast<u64>(it.pos)};
            ++it.pos;
            ++n_parts;
        }

        if (std::string_view(it.get()) == "/") {
            parse_path(into, it);
            ++n_parts;
        }

        if (std::string_view(it.get()) == "?") {
            n_parts += parse_query(into, it);
        }

        if (std::string_view(it.get()) == "#") {
            ++it.pos;
            if (std::string_view(it.get()).empty()) {
                throw "Invalid URL: empty fragment";
            }
            *into++ = {k_fragment, static_cast<u64>(it.pos)};
            ++n_parts;
        }

        *into++ = {k_parts, n_parts};
    }

    template <auto Arr>
    static constexpr auto make_parser() {
        std::array<node, Arr.size()* 2> ret = {};
        token_iter iter{Arr.data()};
        auto into = ret.data();
        parse_parts(into, iter);
        *into = {k_done, 0};
        return ret;
    }

    template <token... Tokens>
    auto parse(ct::lex::token_list<Tokens...>*) {
        // Create a constexpr array of the tokens:
        constexpr std::array<token, sizeof...(Tokens) + 1> arr = {Tokens...};
        // Calculate an array that will instruct the expression regrouper
        constexpr auto nodes = make_parser<arr>();
        using ast = typename executor<arr>::template exec_parse_t<nodes>;
        return ct::ptr<ast>{};
    }
};

}  // namespace detail

template <typename AST>
using parse_tokens_t
    = std::remove_pointer_t<decltype(detail::url_parser{}.parse(ct::ptr<AST>{}))>;

template <ct::str Code>
using parse_t = parse_tokens_t<lex::tokenize_t<Code>>;

}  // namespace ct::url::parse


