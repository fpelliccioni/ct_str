#include <ct/url/lex/lex.hpp>

#include <concepts>

using ct::url::lex::tokenize_t;
using ct::lex::token;
using ct::lex::token_list;

namespace {

// Backslashes are not typically valid in URIs unless percent-encoded.
static_assert(std::same_as<tokenize_t<"www.example\\com">, token_list<...>>);

}  // namespace

int main() {}
