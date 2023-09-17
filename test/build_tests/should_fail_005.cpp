#include <ct/url/lex/lex.hpp>

#include <concepts>

using ct::url::lex::tokenize_t;
using ct::lex::token;
using ct::lex::token_list;

namespace {

// Consecutive dots in domain names are not valid.
static_assert(std::same_as<tokenize_t<"www...example.com">, token_list<...>>);

}  // namespace

int main() {}
