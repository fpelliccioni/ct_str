#include <ct/url/lex/lex.hpp>
#include <ct/url/parse/parse.hpp>

#include <concepts>

using ct::url::lex::tokenize_t;
using ct::lex::token;
using ct::lex::token_list;
using ct::url::parse::parse_t;

namespace {

// Invalid host
static_assert(std::same_as<parse_t<"http://">, parts<scheme<"http">>);

}  // namespace

int main() {}
