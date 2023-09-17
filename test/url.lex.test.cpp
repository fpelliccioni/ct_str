#include <ct/url/lex/lex.hpp>

#include <concepts>

namespace url_lex = ct::url::lex;
using ct::lex::token;
using ct::lex::token_list;

namespace {

// Testing standard alphanumeric characters:
static_assert(std::same_as<url_lex::tokenize_t<"foobar">, token_list<"foobar">>);

// Testing reserved characters:
static_assert(std::same_as<url_lex::tokenize_t<":">, token_list<":">>);
static_assert(std::same_as<url_lex::tokenize_t<"/">, token_list<"/">>);
static_assert(std::same_as<url_lex::tokenize_t<"http://example.com">, token_list<"http", ":", "/", "/", "example.com">>);
static_assert(std::same_as<url_lex::tokenize_t<":/?#[]@!$&'()*+,;=">, token_list<":", "/", "?", "#", "[", "]", "@", "!", "$", "&", "'", "(", ")", "*", "+", ",", ";", "=">>);

// Testing unreserved characters:
static_assert(std::same_as<url_lex::tokenize_t<"-">, token_list<"-">>);
static_assert(std::same_as<url_lex::tokenize_t<"example.com">, token_list<"example.com">>);
static_assert(std::same_as<url_lex::tokenize_t<"example-com">, token_list<"example-com">>);

// Testing percent-encoded characters:
static_assert(std::same_as<url_lex::tokenize_t<"%20">, token_list<"%20">>);
static_assert(std::same_as<url_lex::tokenize_t<"example%20com">, token_list<"example", "%20", "com">>);

// Basic tokenization
static_assert(std::same_as<url_lex::tokenize_t<"foobar.com">, token_list<"foobar.com">>);
static_assert(std::same_as<url_lex::tokenize_t<"/path/to/resource">, token_list<"/", "path", "/", "to", "/", "resource">>);
static_assert(std::same_as<url_lex::tokenize_t<"www.example.com">, token_list<"www.example.com">>);

// Query strings
static_assert(std::same_as<url_lex::tokenize_t<"?key=value&anotherKey=anotherValue">, token_list<"?", "key", "=", "value", "&", "anotherKey", "=", "anotherValue">>);

// Mixed tokens
static_assert(std::same_as<url_lex::tokenize_t<"example.com/path/to/resource?param=value">,
    token_list<"example.com", "/", "path", "/", "to", "/", "resource", "?", "param", "=", "value">>
);

// Percentage encoding
static_assert(std::same_as<url_lex::tokenize_t<"hello%20world">, token_list<"hello", "%20", "world">>);
static_assert(std::same_as<url_lex::tokenize_t<"%7E">, token_list<"%7E">>);

// Domains with subdomains
static_assert(std::same_as<url_lex::tokenize_t<"subdomain.example.com">, token_list<"subdomain.example.com">>);
static_assert(std::same_as<url_lex::tokenize_t<"a.b.c.domain.com">, token_list<"a.b.c.domain.com">>);


// Domains wirt ports
static_assert(std::same_as<url_lex::tokenize_t<"subdomain.example.com:8080">, token_list<"subdomain.example.com", ":", "8080">>);
static_assert(std::same_as<url_lex::tokenize_t<"a.b.c.domain.com:8080">, token_list<"a.b.c.domain.com", ":", "8080">>);

// Testing mix of various valid characters:
static_assert(std::same_as<url_lex::tokenize_t<"example.com/path?query=value#fragment">,
              token_list<"example.com", "/", "path", "?", "query", "=", "value", "#", "fragment">>);

// Testing more complex URIs:
static_assert(std::same_as<url_lex::tokenize_t<"https://www.example.com/path/to/resource?name=test&value=123#frag">,
              token_list<"https", ":", "/", "/", "www.example.com", "/", "path", "/", "to", "/", "resource", "?", "name", "=", "test", "&", "value", "=", "123", "#", "frag">>);

// IPv4 and IPv6 literals:
static_assert(std::same_as<url_lex::tokenize_t<"[2001:0db8:85a3:0000:0000:8a2e:0370:7334]">,
              token_list<"[", "2001", ":", "0db8", ":", "85a3", ":", "0000", ":", "0000", ":", "8a2e", ":", "0370", ":", "7334", "]">>);

static_assert(std::same_as<url_lex::tokenize_t<"192.168.0.1">, token_list<"192.168.0.1">>);

}  // namespace

int main() {}
