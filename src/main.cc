#include <SDL3/SDL.h>
#include <glad/gl.h>

#include <iostream>
#include <vector>

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "ecs/entity.h"
#include "utils.h"

using dennistwo::LoadTextFile;
using dennistwo::Print;
using dennistwo::ecs::Entity;

struct Globals {
    std::string *assetsDir;
    SDL_Window *window;
    SDL_GLContext glContext;
    SDL_Renderer *renderer;
    bool exitSignal = false;
    Entity *root;
};

void GLDebug() {
    std::cout << glGetString(GL_VENDOR) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

GLuint VAO = 0;
GLuint VBO = 0;

int VIEWPORT_WIDTH = 640;
int VIEWPORT_HEIGHT = 480;

void VertexSpec() {
    const std::vector<GLfloat> vertices{
        -0.5, -0.5, 0,
        0.5, -0.5, 0.0,
        0.0, 0.8, 0.0};

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
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
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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