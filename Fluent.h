#pragma once

#include <optional>
#include <type_traits>
#include <utility>

template <typename T>
class Fluent;

template <typename T>
struct is_fluent : std::false_type {};

template <typename T>
struct is_fluent<Fluent<T>> : std::true_type {};

template <typename T>
struct is_optional : std::false_type {};

template <typename T>
struct is_optional<std::optional<T>> : std::true_type {};

template <typename T>
class Fluent {
    T value;

public:
    explicit Fluent(T v) : value(std::move(v)) {}

    static Fluent from(T v) {
        return Fluent(std::move(v));
    }

    template <typename F>
    auto map(F&& func) {
        using OutputType = decltype(func(value));
        return Fluent<OutputType>::from(func(value));
    }

    template <typename F>
    auto flatMap(F&& func) {
        return func(value);
    }

    template <typename P>
    auto filter(P&& pred) {
        if (pred(value)) {
            return Fluent<std::optional<T>>::from(std::optional<T>{value});
        }
        return Fluent<std::optional<T>>::from(std::nullopt);
    }

    template <typename F>
    Fluent& tap(F&& func) {
        func(value);
        return *this;
    }

    template <typename F>
    auto then(F&& func) {
        return map(std::forward<F>(func));
    }

    template <typename U = T>
    auto flatten() -> std::enable_if_t<is_fluent<U>::value, U> {
        return value;
    }

    template <typename F, typename U = T>
    auto mapIfPresent(F&& func) -> std::enable_if_t<is_optional<U>::value,
        Fluent<std::optional<decltype(func(*std::declval<U>()))>>> {
        using OutputType = decltype(func(*std::declval<U>()));
        if (value.has_value()) {
            return Fluent<std::optional<OutputType>>::from(
                std::optional<OutputType>{func(*value)});
        }
        return Fluent<std::optional<OutputType>>::from(std::nullopt);
    }

    template <typename U = T>
    auto orElse(typename U::value_type defaultValue)
        -> std::enable_if_t<is_optional<U>::value, Fluent<typename U::value_type>> {
        using ValueType = typename U::value_type;
        if (value.has_value()) {
            return Fluent<ValueType>::from(*value);
        }
        return Fluent<ValueType>::from(std::move(defaultValue));
    }

    template <typename F>
    Fluent& apply(F&& func) {
        func(value);
        return *this;
    }

    T get() const {
        return value;
    }
};
