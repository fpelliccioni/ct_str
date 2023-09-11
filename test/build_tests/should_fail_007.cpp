#include <ct/url/lex.hpp>

#include <concepts>

namespace lex = ct::url::lex;
using lex::token;
using lex::token_list;

namespace {

// Backslashes are not typically valid in URIs unless percent-encoded.
static_assert(std::same_as<lex::tokenize_t<"www.example\\com">, token_list<...>>);

}  // namespace

int main() {}
