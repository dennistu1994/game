#include <SDL3/SDL.h>
#include <absl/status/status.h>
#include <absl/status/statusor.h>
#include <absl/strings/str_cat.h>
#include <absl/strings/str_format.h>
#include <glad/gl.h>

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "ecs/entity.h"
#include "render/render_system.h"
#include "utils.h"

using dennistwo::LoadTextFile;
using dennistwo::Print;
using dennistwo::ecs::Entity;
using dennistwo::render::PrepareSquareDataProgram;

struct Globals {
    std::string assetsDir;
    SDL_Window *window;
    SDL_GLContext glContext;
    bool exitSignal = false;
    Entity *root;
};

int VIEWPORT_WIDTH = 450;
int VIEWPORT_HEIGHT = 800;

void GLDebug() {
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

absl::StatusOr<Globals> Init(absl::string_view binaryDir) {
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

    std::string assetsDir = absl::StrCat(binaryDir, "assets/");

    Globals globals{
        .assetsDir = assetsDir,
        .window = glWindow,
        .glContext = glContext,
        .root = new Entity("Root"),
    };

    PrepareSquareDataProgram(assetsDir);

    return globals;
}

void Cleanup(Globals &&globals) {
    SDL_GL_DestroyContext(globals.glContext);
    SDL_DestroyWindow(globals.window);
    SDL_Quit();
    delete globals.root;
}

float ASPECT_RATIO = (float)VIEWPORT_WIDTH / (float)VIEWPORT_HEIGHT;

float WORLD_WIDTH = 5.f;  // half
float WORLD_HEIGHT = WORLD_WIDTH / ASPECT_RATIO;

glm::mat4 projection = glm::ortho(-WORLD_WIDTH, WORLD_WIDTH, -WORLD_HEIGHT, WORLD_HEIGHT, 0.1f, 10.f);

void PreDraw() {
}

void Draw(Globals &globals) {
    globals.root->Render();
}

void Input() {
}

void MainLoop(Globals &globals) {
    SDL_Event e;
    while (!globals.exitSignal) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                globals.exitSignal = true;
            }
        }
        Input();
        PreDraw();
        Draw(globals);
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

    MainLoop(*globals);

    Cleanup(std::move(*globals));

    Print("exiting");
    return 0;
}