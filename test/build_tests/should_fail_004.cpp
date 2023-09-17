#include <ct/url/lex/lex.hpp>

#include <concepts>

using ct::url::lex::tokenize_t;
using ct::lex::token;
using ct::lex::token_list;

namespace {

// Underscores are not valid in domain names.
static_assert(std::same_as<tokenize_t<"www.exa_mple.com">, token_list<...>>);

}  // namespace

int main() {}
