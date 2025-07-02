#pragma once

#include <absl/strings/string_view.h>
#include <glad/gl.h>

#include <glm/glm.hpp>

namespace dennistwo::render {
extern GLuint SQUARE_PIPELINE_PROGRAM;
// setup VAO and program for rendering squares
void PrepareSquareDataProgram(absl::string_view assetsDir);
void RenderSquare(const glm::mat4 model);
}  // namespace dennistwo::render