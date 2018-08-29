#ifdef _TINSPIRE
#include <os.h>
#include <libndls.h>
#else
#endif
#include "MainMenu.h"
#include "Replacements.h"
#include "ImageLoader.h"
#include "Thumbnail.h"
#include "SDL/SDL_gfxPrimitives.h"

#define MENU_SELECTOR_LINE_WIDTH (2)

#define MENU_FLAG_MAX_WIDTH   (72)
#define MENU_FLAG_MAX_HEIGHT  (50)

#define MENU_FLAG_SPACING_HORIZ  (15)
#define MENU_FLAG_SPACING_VERT   (22)

#define MENU_TITLE_TOP        (2)

void UpdateDisplay(struct MainMenu* pMenu);

void GetCoordinatesForFlag(enum Flags eFlag, int nFlagsPerRow, int* pX, int* pY)
{
   int nIndex  = (int)eFlag;
   int nColumn = nIndex % nFlagsPerRow;
   int nRow    = nIndex / nFlagsPerRow;

   if( pX != NULL)
      *pX = nColumn * (MENU_FLAG_MAX_WIDTH + MENU_FLAG_SPACING_HORIZ);
   if( pY != NULL)
      *pY = nRow * (MENU_FLAG_MAX_HEIGHT + MENU_FLAG_SPACING_VERT);
}

void CreateMainMenu(struct MainMenu** ppMenu, struct Config* pConfig, struct SDL_Surface* pScreen)
{
   *ppMenu = malloc(sizeof(struct MainMenu));
   struct MainMenu* pMenu = (*ppMenu);

   pMenu->m_pConfig = pConfig;
   pMenu->m_pScreen = pScreen;

   pMenu->m_eChoice = ShowDetails;
   pMenu->m_eLastSelectedFlag = FLAGS_MAX;
   pMenu->m_eSelectedFlag = pConfig != NULL ? pConfig->m_nLastState : Michigan;

   pMenu->m_pFont = LoadFont("arial.ttf", NSDL_FONT_THIN, 255/*R*/, 0/*G*/, 0/*B*/, 16);

   CreateFlagInformation(&pMenu->m_pFlagInformation);

   int nNumFlags = GetNumberOfFlags(pMenu->m_pFlagInformation);

   pMenu->m_pAlreadyLoadedImages = malloc(nNumFlags * sizeof(short));

   const int nNumFlagRows = (SCREEN_HEIGHT - MENU_TITLE_TOP - 15/*Height*/) / (MENU_FLAG_MAX_HEIGHT + MENU_FLAG_SPACING_VERT);
   //const int nNumFlagColumns = SCREEN_WIDTH / (MENU_FLAG_MAX_WIDTH + MENU_FLAG_SPACING_HORIZ);

   const int nNumRowsNeeded = min(max(1, (nNumFlags + nNumFlagRows) / nNumFlagRows), nNumFlagRows);
   const int nNumColsNeeded = max(1, (nNumFlags + nNumRowsNeeded) / nNumRowsNeeded);

   const int nFlagSurfaceHeight = nNumRowsNeeded * (MENU_FLAG_MAX_HEIGHT + MENU_FLAG_SPACING_VERT);
   const int nFlagSurfaceWidth = nNumColsNeeded * (MENU_FLAG_MAX_WIDTH + MENU_FLAG_SPACING_HORIZ);

   pMenu->m_nItemsPerRow = nNumColsNeeded;
   pMenu->m_nSrcScrollX = 0;
   pMenu->m_nScrollX = -1;

   pMenu->m_pFlagsSurface = SDL_CreateRGBSurface(0, nFlagSurfaceWidth, nFlagSurfaceHeight, SCREEN_BIT_DEPTH, 0, 0, 0, 0);
   SDL_FillRect(pMenu->m_pFlagsSurface, NULL, SDL_MapRGB(pMenu->m_pFlagsSurface->format, 255, 215, 139));
   for (int i = 0; i < nNumFlags; i++)
   {
      int x, y;
      enum Flags eFlag = (enum Flags)i;
      GetCoordinatesForFlag(eFlag, pMenu->m_nItemsPerRow, &x, &y);

      int nLoadingX = x + MENU_FLAG_MAX_WIDTH/2 - 5;
      int nLoadingY = y + MENU_FLAG_MAX_HEIGHT/2 - 14;

      DrawText(pMenu->m_pFlagsSurface, pMenu->m_pFont, nLoadingX, nLoadingY, "...", 0, 0, 0);
   }

   for (int i = 0; i < nNumFlags; i++)
   {
      pMenu->m_pAlreadyLoadedImages[i] = 0;
   }
}

void FreeMainMenu(struct MainMenu** ppMenu)
{
   struct MainMenu* pMenu = *ppMenu;

   FreeFont(pMenu->m_pFont);
   FreeFlagInformation(&pMenu->m_pFlagInformation);
   SDL_FreeSurface(pMenu->m_pFlagsSurface);
   pMenu->m_pFlagsSurface = NULL;

   if( pMenu->m_pConfig != NULL)
   {
      pMenu->m_pConfig->m_nLastState = pMenu->m_eSelectedFlag;
   }

   free( pMenu->m_pAlreadyLoadedImages);
   pMenu->m_pAlreadyLoadedImages = NULL;

   pMenu->m_pConfig = NULL;//Does not own
   pMenu->m_pScreen = NULL;//Does not own

   free(*ppMenu);
   *ppMenu = NULL;
}

int PollEvents(struct MainMenu* pMenu)
{
   SDL_Event event;
   while (SDL_PollEvent(&event))
   {
      switch (event.type)
      {
      case SDL_KEYDOWN:
         printf("Key press detected\n");
         switch (event.key.keysym.sym)
         {
         case SDLK_ESCAPE:
            printf("Hit Escape!n");
            pMenu->m_eChoice = Quit;
            return 0;
            break;

         case SDLK_4:
         case SDLK_LEFT:
            if (pMenu->m_eSelectedFlag > (enum Flags)0)
            {
               pMenu->m_eLastSelectedFlag = pMenu->m_eSelectedFlag;
               pMenu->m_eSelectedFlag--;
	       UpdateDisplay(pMenu);
            }
            break;

         case SDLK_6:
         case SDLK_RIGHT:
            if (pMenu->m_eSelectedFlag < (enum Flags)(FLAGS_MAX-1) )
            {
               pMenu->m_eLastSelectedFlag = pMenu->m_eSelectedFlag;
               pMenu->m_eSelectedFlag++;
	       UpdateDisplay(pMenu);
            }
            break;

         case SDLK_8:
         case SDLK_UP:
            if (pMenu->m_eSelectedFlag >= (enum Flags)(0+pMenu->m_nItemsPerRow) )
            {
               pMenu->m_eLastSelectedFlag = pMenu->m_eSelectedFlag;
               pMenu->m_eSelectedFlag-= pMenu->m_nItemsPerRow;
	       UpdateDisplay(pMenu);
            }
            break;

         case SDLK_2:
         case SDLK_DOWN:
            if (pMenu->m_eSelectedFlag <= (enum Flags)(FLAGS_MAX - 1 - pMenu->m_nItemsPerRow))
            {
               pMenu->m_eLastSelectedFlag = pMenu->m_eSelectedFlag;
               pMenu->m_eSelectedFlag += pMenu->m_nItemsPerRow;
	       UpdateDisplay(pMenu);
            }
            break;

         case SDLK_5:
         case SDLK_SPACE:
         case SDLK_RETURN:
         case SDLK_LCTRL:
            if (pMenu->m_eChoice == ShowDetails) {
               return 0;
            }
            break;

         default:
            break;
         }
      }
   }

   return 1;
}

void DrawFlagsSurface(struct MainMenu* pMenu, SDL_Surface* pFlagsSurface)
{
   //Clear text from last selected flag
   if( pMenu->m_eLastSelectedFlag != FLAGS_MAX )
   {
      int nX, nY;
      GetCoordinatesForFlag(pMenu->m_eLastSelectedFlag, pMenu->m_nItemsPerRow, &nX, &nY);

      SDL_Rect dst;
      dst.x = (Sint16)nX;
      dst.y = (Sint16)nY + MENU_FLAG_MAX_HEIGHT;
      dst.w = (Uint16)(pMenu->m_pFlagsSurface->w - dst.x);
      dst.h = (Uint16)min(pMenu->m_pFlagsSurface->h - dst.y, MENU_FLAG_SPACING_VERT);

      SDL_FillRect(pFlagsSurface, &dst, SDL_MapRGB(pMenu->m_pFlagsSurface->format, 255, 215, 139));

      pMenu->m_eLastSelectedFlag = FLAGS_MAX;
   }

   //Draw text for selected flag
   {
      int nX, nY;
      GetCoordinatesForFlag(pMenu->m_eSelectedFlag, pMenu->m_nItemsPerRow, &nX, &nY);

      SmartDrawText(pFlagsSurface, pMenu->m_pFont, nX, nY + MENU_FLAG_MAX_HEIGHT, MENU_FLAG_MAX_WIDTH, GetStateName(pMenu->m_pFlagInformation, pMenu->m_eSelectedFlag), 0, 0, 0);
   }

   int nSelectedIndex = (int)pMenu->m_eSelectedFlag;
   for (int nDistanceFromSelectedFlag = 1; nDistanceFromSelectedFlag < (3/*Spots away*/ * 2/*both directions*/ + 1/*Current item*/); nDistanceFromSelectedFlag++)
   {
      for (int xDist = 0; xDist <= nDistanceFromSelectedFlag; xDist++)
      {
         for (int yDist = 0; yDist <= nDistanceFromSelectedFlag; yDist++)
         {
            int xOff = (-nDistanceFromSelectedFlag) / 2 + xDist;
            int yOff = (-nDistanceFromSelectedFlag) / 2 + yDist;
            int nPotentialIndex = nSelectedIndex + xOff + (pMenu->m_nItemsPerRow * yOff);

            if (nPotentialIndex < 0 || nPotentialIndex >= (int)FLAGS_MAX)
               continue;

            enum Flags eFlag = (enum Flags)nPotentialIndex;

            if (pMenu->m_pAlreadyLoadedImages[(int)eFlag] == 0)
            {
               int nX, nY;
               GetCoordinatesForFlag(eFlag, pMenu->m_nItemsPerRow, &nX, &nY);

               DrawFlagThumbnail(pMenu->m_pFlagInformation, eFlag, pFlagsSurface, nX, nY, MENU_FLAG_MAX_WIDTH, MENU_FLAG_MAX_HEIGHT);
               pMenu->m_pAlreadyLoadedImages[(int)eFlag] = 1;
               return;//Optimization so other thumbnails will say loading for now
            }
         }
      }
   }
}

void UpdateDisplay(struct MainMenu* pMenu)
{
   Uint8 r = 255, g = 215, b = 139;
#ifdef _TINSPIRE
   if (!has_colors)
   {
      r = 255, g = 255, b = 255;
   }
#endif

   SDL_FillRect(pMenu->m_pScreen, NULL, SDL_MapRGB(pMenu->m_pScreen->format, r, g, b));

   DrawText(pMenu->m_pScreen, pMenu->m_pFont, 15, MENU_TITLE_TOP, "nStateFlags", 0, 0, 0);

   DrawFlagsSurface(pMenu, pMenu->m_pFlagsSurface);

   int nFlagPieceWidth = MENU_FLAG_MAX_WIDTH + MENU_FLAG_SPACING_HORIZ;

   int nCurrentX = 0;
   GetCoordinatesForFlag(pMenu->m_eSelectedFlag, pMenu->m_nItemsPerRow, &nCurrentX, NULL);

   //Initial scroll X if too jarring to scroll on opening
   if (pMenu->m_nScrollX == -1) {
      int n = nCurrentX - nFlagPieceWidth;
      if (n < 0)
         n = 0;
      if (n >= (pMenu->m_pFlagsSurface->w - nFlagPieceWidth))
         n = pMenu->m_pFlagsSurface->w - nFlagPieceWidth;
      pMenu->m_nScrollX = pMenu->m_nSrcScrollX = n;
   }

   const int nDestinationLeft = 10;
   const int nDestinationWidth = SCREEN_WIDTH - nDestinationLeft;

   SDL_Rect src;
   src.w = (Uint16)nDestinationWidth;
   src.h = (Uint16)pMenu->m_pFlagsSurface->h;
   src.x = (Sint16)pMenu->m_nSrcScrollX;
   src.y = (Sint16)0;

   int nDstY = (SCREEN_HEIGHT - MENU_TITLE_TOP - 15/*height*/ - src.h - 3/*Little padding*/) / 2 + MENU_TITLE_TOP + 15/*height*/;

   SDL_Rect dst;
   dst.w = (Uint16)nDestinationWidth;
   dst.h = (Uint16)src.h;
   dst.x = (Sint16)nDestinationLeft;
   dst.y = (Sint16)nDstY;
   SDL_BlitSurface(pMenu->m_pFlagsSurface, &src, pMenu->m_pScreen, &dst);

   if (pMenu->m_nSrcScrollX < pMenu->m_nScrollX)
   {
      int nAmount = 3;
      if ((pMenu->m_nScrollX - pMenu->m_nSrcScrollX) > (4 * nFlagPieceWidth))
         nAmount = 58;
      else if ((pMenu->m_nScrollX - pMenu->m_nSrcScrollX) > (3 * nFlagPieceWidth))
         nAmount = 28;
      else if ((pMenu->m_nScrollX - pMenu->m_nSrcScrollX) > (2 * nFlagPieceWidth))
         nAmount = 16;
      pMenu->m_nSrcScrollX += nAmount;
   }
   if (pMenu->m_nSrcScrollX > pMenu->m_nScrollX)
   {
      int nAmount = 3;
      if ((pMenu->m_nSrcScrollX - pMenu->m_nScrollX) > (4 * nFlagPieceWidth))
         nAmount = 58;
      else if ((pMenu->m_nSrcScrollX - pMenu->m_nScrollX) > (3 * nFlagPieceWidth))
         nAmount = 28;
      else if ((pMenu->m_nSrcScrollX - pMenu->m_nScrollX) > (2 * nFlagPieceWidth))
         nAmount = 16;
      pMenu->m_nSrcScrollX -= nAmount;
   }

   //Check if needs to scroll
   int nCurrentFlagLeftScreen = pMenu->m_nScrollX > nCurrentX ? 1 : 0;
   int nCurrentFlagRightScreen = (pMenu->m_nScrollX + nDestinationWidth - (nFlagPieceWidth + 15/*Little extra*/) - nCurrentX) < 0 ? 1 : 0;
   if (nCurrentFlagLeftScreen == 1 || nCurrentFlagRightScreen == 1)
   {
      if (nCurrentFlagLeftScreen == 1) {
         pMenu->m_nScrollX -= (int)(nFlagPieceWidth*1.5);
         if (pMenu->m_nScrollX < 0)
            pMenu->m_nScrollX = 0;
      }
      else if (nCurrentFlagRightScreen == 1) {
         pMenu->m_nScrollX += (int)(nFlagPieceWidth*1.5);
         if (pMenu->m_nScrollX > (pMenu->m_pFlagsSurface->w - nDestinationWidth))
            pMenu->m_nScrollX = pMenu->m_pFlagsSurface->w - nDestinationWidth;
      }
   }

   SDL_UpdateRect(pMenu->m_pScreen, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

int MainMenuLoop(struct MainMenu* pMenu)
{
   if (PollEvents(pMenu) == 0)
      return 0;

   UpdateDisplay(pMenu);

   return 1;
}

int MainMenuShouldQuit(struct MainMenu* pMenu)
{
   return pMenu->m_eChoice == Quit;
}

enum Flags MainMenuGetSelectedFlag(struct MainMenu* pMenu)
{
   return pMenu->m_eSelectedFlag;
}


