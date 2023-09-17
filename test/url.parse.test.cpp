#include <ct/url/parse/parse.hpp>
#include <ct/url/parse/get.hpp>

#include <type_traits>

namespace {

using ct::url::parse::parse_t;
using ct::url::parse::get;
using ct::url::parse::count;
using namespace ct::url::ast;

using tokens = std::pair<token, token>;

static_assert(std::same_as<
    parse_t<"http://example.com">,
    parts<scheme<"http">, host<"example.com">>
>);

static_assert(std::same_as<
    parse_t<"https://www.example.com:8080">,
    parts<scheme<"https">, host<"www.example.com">, port<8080>>
>);

static_assert(std::same_as<
    parse_t<"https://www.example.com:8080/">,
    parts<
        scheme<"https">, host<"www.example.com">, port<8080>, path<"/">
    >
>);

static_assert(std::same_as<
    parse_t<"https://www.example.com:8080/path">,
    parts<
        scheme<"https">, host<"www.example.com">, port<8080>, path<"/path">
    >
>);

static_assert(std::same_as<
    parse_t<"https://www.example.com:8080/path/to/resource">,
    parts<
        scheme<"https">, host<"www.example.com">, port<8080>, path<"/path/to/resource">
    >
>);

static_assert(std::same_as<
    parse_t<"https://www.example.com:8080/path/to/resource?name=test">,
    parts<
        scheme<"https">, host<"www.example.com">, port<8080>, path<"/path/to/resource">,
        query_param<"name", "test">
    >
>);

static_assert(std::same_as<
    parse_t<"https://www.example.com:8080/path/to/resource?name=test&value=123">,
    parts<
        scheme<"https">, host<"www.example.com">, port<8080>, path<"/path/to/resource">,
        query_param<"name", "test">, query_param<"value", "123">
    >
>);

// Fragment ---------------------------------------------------------------
static_assert(std::same_as<
    parse_t<"https://www.example.com:8080/path/to/resource?name=test&value=123#frag">,
    parts<
        scheme<"https">, host<"www.example.com">, port<8080>, path<"/path/to/resource">,
        query_param<"name", "test">, query_param<"value", "123">, fragment<"frag">
    >
>);

static_assert(std::same_as<
    parse_t<"https://www.example.com:8080/path/to/resource?name=test#frag">,
    parts<
        scheme<"https">, host<"www.example.com">, port<8080>, path<"/path/to/resource">,
        query_param<"name", "test">, fragment<"frag">
    >
>);

static_assert(std::same_as<
    parse_t<"https://www.example.com:8080/path/to/resource#frag">,
    parts<
        scheme<"https">, host<"www.example.com">, port<8080>, path<"/path/to/resource">,
        fragment<"frag">
    >
>);

static_assert(std::same_as<
    parse_t<"https://www.example.com:8080/#frag">,
    parts<
        scheme<"https">, host<"www.example.com">, port<8080>, path<"/">,
        fragment<"frag">
    >
>);

static_assert(std::same_as<
    parse_t<"https://www.example.com:8080#frag">,
    parts<
        scheme<"https">, host<"www.example.com">, port<8080>,
        fragment<"frag">
    >
>);

static_assert(std::same_as<
    parse_t<"https://www.example.com#frag">,
    parts<
        scheme<"https">, host<"www.example.com">,
        fragment<"frag">
    >
>);

//TODO: esto debería ser un error
static_assert(std::same_as<
    parse_t<"https://www.#frag">,
    parts<
        scheme<"https">, host<"www.">,
        fragment<"frag">
    >
>);

using MyParts = parts<scheme<"http">, host<"example.com">>;
static_assert(get<MyParts, scheme>() == "http");
static_assert(count<scheme, MyParts>::value == 1);
static_assert(count<query_param, MyParts>::value == 0);

using MyParts2 = parts<scheme<"http">, host<"example.com">, query_param<"name", "test">, query_param<"value", "123">>;
static_assert(count<query_param, MyParts2>::value == 2);
static_assert(get<MyParts2, query_param, 0>() == tokens("name", "test"));
static_assert(get<MyParts2, query_param, 1>() == tokens("value", "123"));
static_assert(get<MyParts2, query_param, 0>().first == "name");
static_assert(get<MyParts2, query_param, 0>().second == "test");
static_assert(get<MyParts2, query_param, 1>().first == "value");
static_assert(get<MyParts2, query_param, 1>().second == "123");

using full_url = parse_t<"https://www.example.com:8080/path/to/resource?name=test&value=123#frag">;
static_assert(get<full_url, scheme>() == "https");
static_assert(get<full_url, host>() == "www.example.com");
static_assert(get<full_url, port>() == 8080);
static_assert(get<full_url, path>() == "/path/to/resource");
static_assert(get<full_url, query_param, 0>() == tokens("name", "test"));
static_assert(get<full_url, query_param, 1>() == tokens("value", "123"));
static_assert(get<full_url, fragment>() == "frag");

}  // namespace

int main() {}
