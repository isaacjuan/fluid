#pragma once

#include <string>

class Composable {
    int value;
    std::string text;

public:
    Composable();

    Composable& setValue(int v);
    Composable& setText(const std::string& t);
    Composable& add(int n);
    Composable& append(const std::string& s);

    void print() const;
};
