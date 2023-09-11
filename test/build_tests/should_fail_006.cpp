#include <ct/url/lex.hpp>

#include <concepts>

namespace lex = ct::url::lex;
using lex::token;
using lex::token_list;

namespace {

// URIs cannot begin with a number.
static_assert(std::same_as<lex::tokenize_t<"123example.com">, token_list<...>>);

}  // namespace

int main() {}
