#pragma once

#include <string>

#include "absl/strings/string_view.h"

namespace dennistwo {
void Print(absl::string_view message);
std::string LoadTextFile(absl::string_view path);
}  // namespace dennistwo
