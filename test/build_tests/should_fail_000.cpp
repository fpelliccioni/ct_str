#include <ct/url/lex.hpp>

#include <concepts>

namespace lex = ct::url::lex;
using lex::token;
using lex::token_list;

namespace {

// Unencoded spaces are not valid in URIs.
using toks = lex::tokenize_t<"foo bar">

}  // namespace

int main() {}
