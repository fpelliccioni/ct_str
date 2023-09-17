#include <ct/url/lex/lex.hpp>

#include <concepts>

using ct::url::lex::tokenize_t;
using ct::lex::token;
using ct::lex::token_list;

namespace {

// A percent-encoded character should always have two hexadecimal digits.
using toks = tokenize_t<"foo%2">;

}  // namespace

int main() {}
