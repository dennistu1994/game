#include <iostream>
#include "absl/strings/string_view.h"

template <class T>
void print(const T& thing) {
    std::cout << thing << std::endl;
}

void print(absl::string_view message) {
    print(message);
}