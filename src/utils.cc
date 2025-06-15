#include "utils.h"

#include <iostream>

#include "absl/strings/string_view.h"

void print(absl::string_view message) {
    std::cout << message << std::endl;
}