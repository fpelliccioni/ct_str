# C++ Compile-Time Utilities (ctutils)

A collection of utilities crafted specifically for compile-time usage in C++. These tools are geared to simplify and enhance compile-time programming using the modern features of C++20.

**Note**: ctutils is a header-only library, making it easy to integrate into your projects.

## Requirements

- A C++20 compliant compiler.
- [CMake](https://cmake.org/) (if you intend to build and run tests).

## Included Utilities

### ct::str

A compile-time string representation. `ct::str` is a template class that embodies a fixed-length character string. It's designed to be used in `constexpr` contexts and can be employed as a non-type template parameter.

**Features**:

- **Compile-time construction**: Can be initialized with string literals and other compile-time known data.
- **Interoperability**: Implicitly converts to `std::string_view` for easy interaction with other C++ utilities and libraries.
- **Basic operations**: Supports operations like compile-time concatenation and comparison.

**Example**:

```cpp
constexpr auto s1 = ct::str("Hello, ");
constexpr auto s2 = ct::str("World!");
constexpr auto s3 = s1 + s2;  // Compile-time concatenation
```

## Contributions

Contributions are welcome. Please refer to [CONTRIBUTING.md](CONTRIBUTING.md) for more details on the contribution process and guidelines.

## License

Distributed under the Boost Software License, Version 1.0. See `LICENSE` for more information.

