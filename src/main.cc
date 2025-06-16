#include <SDL3/SDL.h>
#include <glad/gl.h>

#include <iostream>
#include <vector>

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "ecs/entity.h"
#include "utils.h"

using dennistwo::LoadTextFile;
using dennistwo::Print;
using dennistwo::ecs::Entity;

static void GLClearAllErrors() {
    while (glGetError() != GL_NO_ERROR) {
    }
}

static void GLCheckErrorStatus(const char *function, int line) {
    while (GLenum error = glGetError()) {
        Print(absl::StrFormat("OpenGL error %d from %s line %d", error, function, line));
    }
}

#define GL_CHECKED(f)   \
    GLClearAllErrors(); \
    f;                  \
    GLCheckErrorStatus(#f, __LINE__)

struct Globals {
    std::string *assetsDir;
    SDL_Window *window;
    SDL_GLContext glContext;
    SDL_Renderer *renderer;
    bool exitSignal = false;
    Entity *root;
};

int VIEWPORT_WIDTH = 640;
int VIEWPORT_HEIGHT = 480;

void GLDebug() {
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

GLuint VertexArrayObjects;
GLuint BufferObjects[2];
void VertexSpec() {
    // vertices
    const std::vector<GLfloat> data{
        -0.5f, -0.5f, 0.f,
        1.f, 0.f, 0.f,  // color
        0.5f, -0.5f, 0.0f,
        0.f, 1.f, 0.f,  // color
        -0.5f, 0.5f, 0.0f,
        0.f, 0.f, 1.f,  // color
        0.5f, 0.5f, 0.0f,
        0.f, 0.f, 1.f,  // color
    };
    // indices of the triangles
    const std::vector<GLuint> indices{
        0, 1, 2,
        2, 1, 3};

    glGenVertexArrays(1, &VertexArrayObjects);
    glBindVertexArray(VertexArrayObjects);

    glGenBuffers(2, BufferObjects);
    glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferObjects[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

GLuint PIPELINE_SHADER_PROGRAM = 0;

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

void CreateGraphicsPipeline(Globals &globals) {
    PIPELINE_SHADER_PROGRAM = CreateShaderProgram(
        LoadTextFile(
            absl::StrCat(*globals.assetsDir, "shaders/demo/vertex.glsl")),
        LoadTextFile(
            absl::StrCat(*globals.assetsDir, "shaders/demo/fragment.glsl")));
}

absl::StatusOr<Globals> Init(absl::string_view binaryDir) {
    std::string *assetsDir = new std::string(absl::StrCat(binaryDir, "assets/"));
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window *glWindow = SDL_CreateWindow("hello", VIEWPORT_WIDTH, VIEWPORT_HEIGHT, SDL_WINDOW_OPENGL);
    if (glWindow == nullptr) {
        return absl::InternalError(
            absl::StrCat("failed to create window: ", SDL_GetError()));
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(glWindow);

    if (glContext == nullptr) {
        return absl::InternalError(
            absl::StrCat("failed to create glContext: ", SDL_GetError()));
    }

    if (!gladLoadGL(SDL_GL_GetProcAddress)) {
        return absl::InternalError(
            "failed during gladLoadGL");
    }

    GLDebug();

    SDL_Renderer *renderer = SDL_CreateRenderer(glWindow, NULL);
    if (renderer == nullptr) {
        return absl::InternalError(
            absl::StrCat("failed to create renderer: ", SDL_GetError()));
    }

    return Globals{
        .assetsDir = assetsDir,
        .window = glWindow,
        .glContext = glContext,
        .renderer = renderer,
        .root = new Entity("Root"),
    };
}

void Cleanup(Globals &&globals) {
    SDL_DestroyRenderer(globals.renderer);
    SDL_GL_DestroyContext(globals.glContext);
    SDL_DestroyWindow(globals.window);
    SDL_Quit();
    delete globals.root;
    delete globals.assetsDir;
}

void PreDraw() {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    glClearColor(1.f, 1.f, 0.1f, 1.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glUseProgram(PIPELINE_SHADER_PROGRAM);
}
void Draw() {
    GL_CHECKED(glBindVertexArray(VertexArrayObjects));
    GL_CHECKED(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

void MainLoop(Globals &globals) {
    SDL_Event e;
    while (!globals.exitSignal) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                globals.exitSignal = true;
            }
        }

        PreDraw();
        Draw();
        SDL_GL_SwapWindow(globals.window);
    }
}

static std::string GetBinaryDir(absl::string_view path) {
    for (int i = path.size(); i >= 0; i--) {
        if (path[i] == '/') {
            return std::string(path.substr(0, i + 1));
        }
    }
    return "";
}

int main(int argc, char *argv[]) {
    std::string binaryDir = GetBinaryDir(argv[0]);
    absl::StatusOr<Globals> globals = Init(binaryDir);
    if (!globals.ok()) {
        Print(globals.status().ToString());
    }

    VertexSpec();
    CreateGraphicsPipeline(*globals);

    MainLoop(*globals);

    Cleanup(std::move(*globals));

    Print("exiting");
    return 0;
}