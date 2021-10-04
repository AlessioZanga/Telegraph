#pragma once

#include <stdexcept>
#include <string>

namespace std {
//! Overloading to_string for transparent exception handling.
inline string to_string(const string &__val) { return __val; }
}  // namespace std

class NOT_DEFINED : public std::invalid_argument {
   public:
    template <typename T>
    explicit NOT_DEFINED(const T &X) : std::invalid_argument("Vertex " + std::to_string(X) + " is not defined.") {}

    template <typename T, typename U>
    NOT_DEFINED(const T &X, const U &Y)
        : std::invalid_argument("Edge (" + std::to_string(X) + ", " + std::to_string(Y) + ") is not defined.") {}

    NOT_DEFINED(const NOT_DEFINED &) = default;
    NOT_DEFINED &operator=(const NOT_DEFINED &) = default;
    NOT_DEFINED(NOT_DEFINED &&) = default;
    NOT_DEFINED &operator=(NOT_DEFINED &&) = default;
};

class NO_LABEL : public std::invalid_argument {
   public:
    NO_LABEL() : std::invalid_argument("Graph label is not defined.") {}

    template <typename T>
    explicit NO_LABEL(const T &X) : std::invalid_argument("Vertex " + std::to_string(X) + " label is not defined.") {}

    template <typename T, typename U>
    NO_LABEL(const T &X, const U &Y)
        : std::invalid_argument("Edge (" + std::to_string(X) + ", " + std::to_string(Y) + ") label is not defined.") {}

    NO_LABEL(const NO_LABEL &) = default;
    NO_LABEL &operator=(const NO_LABEL &) = default;
    NO_LABEL(NO_LABEL &&) = default;
    NO_LABEL &operator=(NO_LABEL &&) = default;
};

#define INVALID_LABEL std::invalid_argument("Label cannot be empty string.")
#define DUPLICATED_LABEL(X) std::invalid_argument("Label '" + std::to_string(X) + "' already defined.")
#define NO_KEY(X) std::invalid_argument("Key " + std::to_string(X) + " is not defined.")
