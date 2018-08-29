#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#ifdef _TINSPIRE
#include <os.h>
#endif
#include "SDL/SDL.h"
#include "ImageLoader.h"
#include "FlagInformation.h"
#include "FlagEnum.h"
#include "Font.h"

void SmartDrawText(SDL_Surface* pSurface, Font* pFont, int x, int y, int nWidth, const char* pstrBuffer, int r, int g, int b);
//SDL_Surface* GetFlagThumbnail(struct FlagInformation* pFlagInformation, enum Flags eFlag, int maxWidth, int maxHeight);
void DrawFlagThumbnail(struct FlagInformation* pFlagInformation, enum Flags eFlag, SDL_Surface* pScreen, int x, int y, int maxWidth, int maxHeight);

#endif
