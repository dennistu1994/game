#pragma once

#include <absl/strings/string_view.h>

#include <string>

namespace dennistwo {
void Print(absl::string_view message);
std::string LoadTextFile(absl::string_view path);
}  // namespace dennistwo
