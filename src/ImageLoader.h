#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#ifdef _TINSPIRE
#include <os.h>
#endif
#include "SDL/SDL.h"
#include "FlagEnum.h"
#include "FlagInformation.h"

SDL_Surface* GetFlagImage(struct FlagInformation* pFlagInformation, enum Flags flag);

#endif
