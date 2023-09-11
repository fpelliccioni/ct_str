#include <ct/url/lex.hpp>

#include <concepts>

namespace lex = ct::url::lex;
using lex::token;
using lex::token_list;

namespace {

// G is not a valid hexadecimal character.
static_assert(std::same_as<lex::tokenize_t<"foo%2Gbar">, token_list<...>>);

}  // namespace

int main() {}
