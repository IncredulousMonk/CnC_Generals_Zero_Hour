#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "LinuxDevice/Common/LinuxGameEngine.h"

int main(int argc, char* argv[]) {
   if (!SDL_Init(SDL_INIT_VIDEO)) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not initialise SDL: %s", SDL_GetError());
      SDL_Quit();
      return 1;
   }

   SDL_Window* win {SDL_CreateWindow("Command and Conquer Generals", 800, 600, SDL_WINDOW_HIDDEN)};
   if (!win) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
      SDL_Quit();
      return 1;
   }

   SDL_Renderer* ren = SDL_CreateRenderer(win, NULL);
   if (!ren) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create renderer: %s", SDL_GetError());
      SDL_DestroyWindow(win);
      SDL_Quit();
      return 1;
   }

   SDL_Surface* bmp = SDL_LoadBMP("../assets/Install_Final.bmp");
   if (!bmp) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not load bitmap: %s", SDL_GetError());
      SDL_DestroyRenderer(ren);
      SDL_DestroyWindow(win);
      SDL_Quit();
      return 1;
   }

   SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, bmp);
   SDL_DestroySurface(bmp);

   if (!tex) {
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create texture: %s", SDL_GetError());
      SDL_DestroyRenderer(ren);
      SDL_DestroyWindow(win);
      SDL_Quit();
      return 1;
   }

   SDL_ShowWindow(win);

   // run the game main loop
   GameMain(argc, argv);

      bool done {false};

   while (!done) {
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
         if (e.type == SDL_EVENT_QUIT) {
            done = true;
         }
      }

      SDL_RenderClear(ren);
      SDL_RenderTexture(ren, tex, NULL, NULL);
      SDL_RenderPresent(ren);
   }

   SDL_DestroyTexture(tex);
   SDL_DestroyRenderer(ren);
   SDL_DestroyWindow(win);
   SDL_Quit();

   return 0;
}

// CreateGameEngine ===========================================================
/** Create the Linux game engine we're going to use */
//=============================================================================
GameEngine* CreateGameEngine(void) {
   LinuxGameEngine *engine;

   // engine = NEW LinuxGameEngine;
   engine = new LinuxGameEngine;
   //game engine may not have existed when app got focus so make sure it
   //knows about current focus state.
   // engine->setIsActive(isWinMainActive);

   return engine;

}  // end CreateGameEngine
