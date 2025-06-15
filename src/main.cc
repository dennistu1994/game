#include <SDL3/SDL.h>
#include <iostream>
#include "utils.h"
#include "absl/status/statusor.h"
#include "absl/status/status.h"
#include "absl/strings/str_cat.h"

struct Globals
{
    SDL_Window *window;
    SDL_GLContext glContext;
    SDL_Renderer *renderer;
};

absl::StatusOr<Globals> init()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *glWindow = SDL_CreateWindow("hello", 640, 480, SDL_WINDOW_OPENGL);
    if (glWindow == nullptr)
    {
        return absl::InternalError(
            absl::StrCat("failed to create window: ", SDL_GetError()));
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GLContext glContext = SDL_GL_CreateContext(glWindow);
    if (glContext == nullptr)
    {
        return absl::InternalError(
            absl::StrCat("failed to create glContext: ", SDL_GetError()));
    }
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

absl::Status cleanup(Globals &globals)
{
    SDL_DestroyRenderer(globals.renderer);
    SDL_DestroyWindow(globals.window);
    SDL_Quit();
    return absl::OkStatus();
}

int main()
{
    absl::StatusOr<Globals> globals = init();
    if (!globals.ok())
    {
        print(globals.status().ToString());
    }
    auto [window, glContext, renderer] = *globals;

    SDL_Event e;
    bool quit = false;

    // Define a rectangle
    SDL_FRect greenSquare{270, 190, 100, 100};

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set render draw color to black
        SDL_RenderClear(renderer);                      // Clear the renderer

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Set render draw color to green
        SDL_RenderFillRect(renderer, &greenSquare);       // Render the rectangle

        SDL_RenderPresent(renderer); // Render the screen
    }

    absl::Status status = cleanup(*globals);
    if (!status.ok())
    {
        print(status.ToString());
        return int(status.code());
    }
    print("exiting");
    return 0;
}