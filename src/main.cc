#include <SDL3/SDL.h>
#include <iostream>
#include "utils.h"
#include "absl/status/statusor.h"
#include "absl/status/status.h"
#include "absl/strings/str_cat.h"
#include <glad/gl.h>

struct Globals
{
    SDL_Window *window;
    SDL_GLContext glContext;
    SDL_Renderer *renderer;
    bool exitSignal = false;
};

void GLDebug()
{
    std::cout << glGetString(GL_VENDOR) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

absl::StatusOr<Globals> Init()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window *glWindow = SDL_CreateWindow("hello", 640, 480, SDL_WINDOW_OPENGL);
    if (glWindow == nullptr)
    {
        return absl::InternalError(
            absl::StrCat("failed to create window: ", SDL_GetError()));
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(glWindow);

    if (glContext == nullptr)
    {
        return absl::InternalError(
            absl::StrCat("failed to create glContext: ", SDL_GetError()));
    }

    if (!gladLoadGL(SDL_GL_GetProcAddress))
    {
        return absl::InternalError(
            "failed during gladLoadGL");
    }

    GLDebug();

    SDL_Renderer *renderer = SDL_CreateRenderer(glWindow, NULL);
    if (renderer == nullptr)
    {
        return absl::InternalError(
            absl::StrCat("failed to create renderer: ", SDL_GetError()));
    }

    return Globals{
        .window = glWindow,
        .glContext = glContext,
        .renderer = renderer,
    };
}

void Cleanup(Globals &&globals)
{
    SDL_DestroyRenderer(globals.renderer);
    SDL_DestroyWindow(globals.window);
    SDL_Quit();
}

void MainLoop(Globals &globals)
{
    auto [window, glContext, renderer, _] = globals;
    SDL_Event e;
    while (!globals.exitSignal)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                globals.exitSignal = true;
            }
        }
    }
}

int main()
{
    absl::StatusOr<Globals> globals = Init();
    if (!globals.ok())
    {
        print(globals.status().ToString());
    }

    MainLoop(*globals);

    Cleanup(std::move(*globals));

    print("exiting");
    return 0;
}