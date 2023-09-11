#include <ct/url/lex.hpp>

#include <concepts>

namespace lex = ct::url::lex;
using lex::token;
using lex::token_list;

namespace {

// Underscores are not valid in domain names.
static_assert(std::same_as<lex::tokenize_t<"www.exa_mple.com">, token_list<...>>);

}  // namespace

int main() {}
