#include "utils.h"

#include <fstream>
#include <iostream>
#include <string>

#include "absl/strings/str_cat.h"
#include "absl/strings/string_view.h"

namespace dennistwo {
void Print(absl::string_view message) {
    std::cout << message << std::endl;
}

std::string LoadTextFile(absl::string_view path) {
    std::string result;
    std::string line;
    std::ifstream file(path.data());
    if (file.is_open()) {
        while (std::getline(file, line)) {
            result += line + '\n';
        }
        file.close();
    } else {
        Print(absl::StrCat("File not found: ", path));
    }
    return result;
}
}  // namespace dennistwo