#include <ct/url/lex.hpp>

#include <concepts>

namespace lex = ct::url::lex;
using lex::token;
using lex::token_list;

namespace {

using toks = lex::tokenize_t<"foobar">;

}  // namespace

int main() {}
