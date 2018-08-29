#ifndef MAINMENU_H
#define MAINMENU_H

#ifdef _TINSPIRE
#include <os.h>
#endif
#include "SDL/SDL.h"
#include "Config.h"
#include "Font.h"
#include "FlagInformation.h"
#include "FlagEnum.h"

enum MenuChoice
{
   ShowDetails,
   Quit
};

struct MainMenu
{
   enum MenuChoice m_eChoice;
   enum Flags m_eLastSelectedFlag;
   enum Flags m_eSelectedFlag;
   int m_nItemsPerRow;
   int m_nSrcScrollX;
   int m_nScrollX;
   Font *m_pFont;
   struct Config* m_pConfig;//Does not own
   struct SDL_Surface* m_pScreen;//Does not own
   struct SDL_Surface* m_pFlagsSurface;
   struct FlagInformation* m_pFlagInformation;
   short* m_pAlreadyLoadedImages;
   //short m_bAlreadyLoadedImages[200];
};

void CreateMainMenu(struct MainMenu** ppMenu, struct Config* pConfig, struct SDL_Surface* pScreen);
void FreeMainMenu(struct MainMenu** ppMenu);
int MainMenuLoop(struct MainMenu* pMenu);
int MainMenuShouldQuit(struct MainMenu* pMenu);
enum Flags MainMenuGetSelectedFlag(struct MainMenu* pMenu);

#endif
