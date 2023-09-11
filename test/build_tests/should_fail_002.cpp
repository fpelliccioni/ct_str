#include <ct/url/lex.hpp>

#include <concepts>

namespace lex = ct::url::lex;
using lex::token;
using lex::token_list;

namespace {

// A percent-encoded character should always have two hexadecimal digits.
using toks = lex::tokenize_t<"foo%2">;

}  // namespace

int main() {}
