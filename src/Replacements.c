#include "Replacements.h"
#include <string.h>

#ifdef _TINSPIRE
#include <os.h>
#else
#include <stdio.h>
#endif
#include <stdarg.h>

int min(int a, int b)
{
   return a < b ? a : b;
}

int max(int a, int b)
{
   return a > b ? a : b;
}

void IntToA(char* pstrBuffer, int bufferSize, int n)
{
#ifdef _TINSPIRE
   sprintf(pstrBuffer, "%d", n);
#elif defined(_WIN32)
   sprintf_s(pstrBuffer, bufferSize, "%d", n);
#else
   sprintf(pstrBuffer, "%d", n);
#endif
}

void CommaSeparate(char* pstrBuffer)
{
   int nLen = (int)strlen(pstrBuffer);
   int nCommasToAdd = (nLen - 1) / 3;
   if (nCommasToAdd <= 0)
      return;

   pstrBuffer[nLen - 1 /*index*/ + nCommasToAdd + 1] = '\0';
   int nDigit = 0;
   for (int i = nLen-1/*index*/; i >= 0; i--)
   {
      pstrBuffer[i + nCommasToAdd] = pstrBuffer[i];
      nDigit++;
      if (nDigit == 3 && nCommasToAdd > 0)
      {
         nCommasToAdd--;
         pstrBuffer[i + nCommasToAdd] = ',';
         nDigit = 0;
      }
   }
}

void StringCopy(char* pstrOutput, int bufferSize, const char* pstrInput)
{
#ifdef _TINSPIRE
   strcpy(pstrOutput, pstrInput);
#elif defined(_WIN32)
   strcpy_s(pstrOutput, bufferSize, pstrInput);
#else
   strcpy(pstrOutput, pstrInput);
#endif
}

void StringAppend(char* pstrBuffer, int nSize, const char* pstrAddition)
{
#ifdef _TINSPIRE
   strcat(pstrBuffer, pstrAddition);
#elif defined(_WIN32)
   strcat_s(pstrBuffer, nSize, pstrAddition);
#else
   strcat(pstrBuffer, pstrAddition);
#endif
}

void StringFmt(char* pstrBuffer, int nSize, char* pstrFormat, ...)
{
   va_list args
#ifdef _WIN32
    = NULL
#endif
   ;

   va_start(args, pstrFormat);
#ifdef _WIN32
   vsprintf_s(pstrBuffer, nSize, pstrFormat, args);
#else
   vsprintf(pstrBuffer, pstrFormat, args);
#endif
   va_end(args);
}

