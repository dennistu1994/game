#include "render_system.h"

#include <absl/strings/str_cat.h>
#include <glad/gl.h>

#include "utils.h"

namespace dennistwo::render {

GLuint SQUARE_VAO;
GLuint SQUARE_BO[2];  // data and elements

void PrepareSquareData() {
    // vertices
    const std::vector<GLfloat> data{
        -0.5f, -0.5f, -5.f,
        1.f, 0.f, 0.f,  // color
        0.5f, -0.5f, -5.f,
        0.f, 1.f, 0.f,  // color
        -0.5f, 0.5f, -5.f,
        0.f, 0.f, 1.f,  // color
        0.5f, 0.5f, -5.f,
        0.f, 0.f, 1.f,  // color
    };
    // indices of the triangles
    const std::vector<GLuint> indices{
        0, 1, 2,
        2, 1, 3};

    glGenVertexArrays(1, &SQUARE_VAO);
    glBindVertexArray(SQUARE_VAO);

    glGenBuffers(2, SQUARE_BO);
    glBindBuffer(GL_ARRAY_BUFFER, SQUARE_BO[0]);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SQUARE_BO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

GLuint SQUARE_PIPELINE_PROGRAM = 0;

GLuint CompileShader(GLuint type, absl::string_view source) {
    GLuint shader;
    if (type == GL_VERTEX_SHADER) {
        shader = glCreateShader(GL_VERTEX_SHADER);
    } else if (type == GL_FRAGMENT_SHADER) {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }
    const char *data = source.data();
    glShaderSource(shader, 1, &data, nullptr);
    glCompileShader(shader);
    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        char *errorMessage = new char[length];
        glGetShaderInfoLog(shader, length, &length, errorMessage);
        Print(errorMessage);
        delete[] errorMessage;
    }
    return shader;
}

GLuint CreateShaderProgram(absl::string_view vs, absl::string_view fs) {
    GLuint program = glCreateProgram();
    GLuint cvs = CompileShader(GL_VERTEX_SHADER, vs);
    GLuint cfs = CompileShader(GL_FRAGMENT_SHADER, fs);
    glAttachShader(program, cvs);
    glAttachShader(program, cfs);
    glLinkProgram(program);
    glValidateProgram(program);
    // delete
    glDetachShader(program, cvs);
    glDetachShader(program, cfs);
    glDeleteShader(cvs);
    glDeleteShader(cfs);
    return program;
}

void CreateGraphicsPipeline(absl::string_view assetsDir) {
    PrepareSquareData();
    SQUARE_PIPELINE_PROGRAM = CreateShaderProgram(
        LoadTextFile(
            absl::StrCat(assetsDir, "shaders/demo/vertex.glsl")),
        LoadTextFile(
            absl::StrCat(assetsDir, "shaders/demo/fragment.glsl")));
}

void PrepareSquareDataProgram(absl::string_view assetsDir) {
    CreateGraphicsPipeline(assetsDir);
}

}  // namespace dennistwo::render