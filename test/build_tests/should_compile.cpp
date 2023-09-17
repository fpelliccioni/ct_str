#include <ct/url/lex/lex.hpp>
#include <ct/url/parse/parse.hpp>

#include <concepts>

using ct::url::lex::tokenize_t;
using ct::lex::token;
using ct::lex::token_list;
using ct::url::parse::parse_t;

namespace {

using toks = tokenize_t<"foobar">;

}  // namespace

int main() {}
