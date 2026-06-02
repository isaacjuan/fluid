#pragma once

template <typename T>
class StaticFluent {
public:
    template <typename F>
    static auto transform(T value, F&& func) {
        return func(value);
    }

    template <typename F>
    static T apply(T value, F&& func) {
        func(value);
        return value;
    }
};
