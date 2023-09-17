#include <ct/url/lex/lex.hpp>

#include <concepts>

using ct::url::lex::tokenize_t;
using ct::lex::token;
using ct::lex::token_list;

namespace {

// Invalid port number
static_assert(std::same_as<tokenize_t<"www.example.com:-8080">, token_list<...>>);

}  // namespace

int main() {}
