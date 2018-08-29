#ifdef _TINSPIRE
#include <os.h>
#include <libndls.h>
#endif
#include <SDL/SDL.h>
#include "MainMenu.h"
#include "ShowFlagDetails.h"
#include "Config.h"
#ifndef _TINSPIRE
#include <SDL/SDL_ttf.h>
#endif
#include "Replacements.h"

#ifdef _TINSPIRE
#define SCREEN_VIDEO_MODE (SDL_SWSURFACE|SDL_FULLSCREEN|SDL_HWPALETTE)
#else
#define SCREEN_VIDEO_MODE (SDL_SWSURFACE|SDL_HWPALETTE)
#endif

#ifdef _WIN32
int WinMain(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
#ifdef _WIN32
   (argc);
   (argv);
#endif
#ifdef _TINSPIRE
   ArchiveSetCurrentDirectory( argv[0] );
#endif

   printf("Initializing SDL.\n");

   /* Initialize the SDL library (starts the event loop) */
   if ( SDL_Init(SDL_INIT_VIDEO ) < 0 )
   {
#ifdef _TINSPIRE
      fprintf(stderr,
              "Couldn't initialize SDL: %s\n", SDL_GetError());
#endif
      exit(1);
   }

   printf("SDL initialized.\n");

   SDL_Surface* pScreen = NULL;
   pScreen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BIT_DEPTH, SCREEN_VIDEO_MODE);

   if( pScreen == NULL )
   {
#ifdef _TINSPIRE
      fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BIT_DEPTH, SDL_GetError());
#endif
      exit(1);
   }
   else
   {
      /* Hides the cursor */
      SDL_ShowCursor(SDL_DISABLE);
   }

#ifndef _TINSPIRE
   //Initialize SDL_ttf
   if (TTF_Init() == -1)
   {
      exit(1);
   }
#endif

   struct Config* pConfig = NULL;
   printf("About to create Config\n");
   CreateConfig(&pConfig);
   printf("Created Config\n");
   struct MainMenu* pMenu = NULL;
   CreateMainMenu(&pMenu, pConfig, pScreen);
   printf("Created MainMenu\n");
   while(1) {
      while(MainMenuLoop(pMenu)){}
      if (MainMenuShouldQuit(pMenu) == 1)
      {
         break;
      }
      enum Flags eSelectedFlag = MainMenuGetSelectedFlag(pMenu);

      //Show Details
      struct ShowFlagDetails* pFlagDetails = NULL;
      CreateShowFlagDetails(&pFlagDetails, pScreen, eSelectedFlag);
      while (ShowFlagDetailsLoop(pFlagDetails)) {}
      FreeShowFlagDetails(&pFlagDetails);
      pFlagDetails = NULL;
   }
   FreeMainMenu(&pMenu);
   pMenu = NULL;

   FreeConfig(&pConfig);

   printf("Quitting SDL.\n");

#ifndef _TINSPIRE
   //Quit SDL_ttf
   TTF_Quit();
#endif

   /* Shutdown all subsystems */
   SDL_Quit();

   printf("Quitting...\n");

   return 0;
}
