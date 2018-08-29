#ifndef SHOWFLAGDETAILS_H
#define SHOWFLAGDETAILS_H

#ifdef _TINSPIRE
#include <os.h>
#endif
#include "SDL/SDL.h"
#include "Font.h"
#include "ImageLoader.h"
#include "FlagInformation.h"
#include "FlagEnum.h"

struct ShowFlagDetails
{
   enum Flags m_eFlag;
   Font *m_pFont;
   struct SDL_Surface* m_pScreen;//Does not own
   struct FlagInformation* m_pFlagInformation;
   struct SDL_Surface* m_pFlag;
};

void CreateShowFlagDetails(struct ShowFlagDetails** ppShowFlagDetails, struct SDL_Surface* pScreen, enum Flags eFlag);
void FreeShowFlagDetails(struct ShowFlagDetails** ppShowFlagDetails);
int ShowFlagDetailsLoop(struct ShowFlagDetails* pShowFlagDetails);

#endif
