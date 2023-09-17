#include <ct/url/lex/lex.hpp>

#include <concepts>

using ct::url::lex::tokenize_t;
using ct::lex::token;
using ct::lex::token_list;

namespace {

using toks0 = tokenize_t<"foo^bar">;

}  // namespace

int main() {}
