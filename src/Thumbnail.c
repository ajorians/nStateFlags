#ifdef _TINSPIRE
#include <os.h>
#include <libndls.h>
#else
#endif
#include "Thumbnail.h"
#include "Font.h"
#include "Replacements.h"
#include "SDL/SDL_gfxPrimitives.h"

Uint16 get_pixel16(SDL_Surface *surface, int x, int y)
{
   int bpp = surface->format->BytesPerPixel;
   Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp; //Get the requested pixel 
   return *(Uint16 *)p;
}

void put_pixel16(SDL_Surface *surface, int x, int y, Uint16 pixel)
{
   int bpp = surface->format->BytesPerPixel;
   Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
   *(Uint16 *)p = pixel;
}

SDL_Surface *ScaleSurface(SDL_Surface *Surface, Uint16 Width, Uint16 Height)
{
   if (!Surface || !Width || !Height)
      return 0;

   SDL_Surface *_ret = SDL_CreateRGBSurface(Surface->flags, Width, Height, Surface->format->BitsPerPixel,
      Surface->format->Rmask, Surface->format->Gmask, Surface->format->Bmask, Surface->format->Amask);

   double    _stretch_factor_x = ((double)(Width) / (double)(Surface->w)),
      _stretch_factor_y = ((double)(Height) / (double)(Surface->h));

   for (Sint32 y = 0; y < Surface->h; y++)
      for (Sint32 x = 0; x < Surface->w; x++)
         for (Sint32 o_y = 0; o_y < _stretch_factor_y; ++o_y)
            for (Sint32 o_x = 0; o_x < _stretch_factor_x; ++o_x)
               put_pixel16(_ret, (Sint32)(_stretch_factor_x * x) + o_x,
               (Sint32)(_stretch_factor_y * y) + o_y, get_pixel16(Surface, x, y));

   return _ret;
}

void SmartDrawText(SDL_Surface* pSurface, Font* pFont, int x, int y, int nWidth, const char* pstrBuffer, int r, int g, int b)
{
   UNUSED(nWidth);
   char buffer[50];
   int nLen = (int)strlen(pstrBuffer);

   if (nLen < 15)
   {
      DrawText(pSurface, pFont, x, y, pstrBuffer, r, g, b);
   }

   int j = 0;
   int nCharsInRow = 0;
   for (int i = 0; i < nLen; i++)
   {
      buffer[j++] = pstrBuffer[i];
      nCharsInRow++;
      if (nCharsInRow > 9 && pstrBuffer[i] == ' ')
      {
         buffer[j++] = '\n';
         nCharsInRow = 0;
      }
   }
   buffer[j] = '\0';
   DrawText(pSurface, pFont, x, y, buffer, r, g, b);
}

SDL_Surface* GetFlagThumbnail(struct FlagInformation* pFlagInformation, enum Flags eFlag, int maxWidth, int maxHeight)
{
   SDL_Surface* pSurface = GetFlagImage(pFlagInformation, eFlag);

   int nImgWidth = pSurface->w;
   int nImgHeight = pSurface->h;

   if (nImgWidth > maxWidth)
   {
      double d = (double)maxWidth / nImgWidth;
      nImgWidth = (int)(nImgWidth * d);
      nImgHeight = (int)(nImgHeight * d);
   }
   if (nImgHeight > maxHeight)
   {
      double d = (double)maxHeight / nImgHeight;
      nImgWidth = (int)(nImgWidth * d);
      nImgHeight = (int)(nImgHeight * d);
   }

   SDL_Surface* pScaledSurface = ScaleSurface(pSurface, (Uint16)nImgWidth, (Uint16)nImgHeight);
   SDL_FreeSurface(pSurface);
   pSurface = NULL;

   return pScaledSurface;
}

void DrawFlagThumbnail(struct FlagInformation* pFlagInformation, enum Flags eFlag, SDL_Surface* pScreen, int x, int y, int maxWidth, int maxHeight)
{
   SDL_Surface* pScaledSurface = GetFlagThumbnail( pFlagInformation, eFlag, maxWidth, maxHeight);

   SDL_Rect dst;
   dst.w = (Uint16)pScaledSurface->w;
   dst.h = (Uint16)pScaledSurface->h;
   dst.x = (Sint16)(x + ((maxWidth - dst.w) / 2));
   dst.y = (Sint16)(y + ((maxHeight - dst.h) / 2));

   SDL_BlitSurface(pScaledSurface, NULL, pScreen, &dst);

   SDL_FreeSurface(pScaledSurface);
   pScaledSurface = NULL;
}
