#include "Composable.h"

#include <iostream>

Composable::Composable() : value(0), text("") {}

Composable& Composable::setValue(int v) {
    value = v;
    return *this;
}

Composable& Composable::setText(const std::string& t) {
    text = t;
    return *this;
}

Composable& Composable::add(int n) {
    value += n;
    return *this;
}

Composable& Composable::append(const std::string& s) {
    text += s;
    return *this;
}

void Composable::print() const {
    std::cout << "Value: " << value << ", Text: " << text << std::endl;
}
