#include <ct/url/lex/lex.hpp>

#include <concepts>

using ct::url::lex::tokenize_t;
using ct::lex::token;
using ct::lex::token_list;

namespace {

// G is not a valid hexadecimal character.
static_assert(std::same_as<tokenize_t<"foo%2Gbar">, token_list<...>>);

}  // namespace

int main() {}
