#ifndef FLAGINFORMATION_H
#define FLAGINFORMATION_H

#ifdef _TINSPIRE
#include <os.h>
#else
#include <stdint.h>
#endif

#include "FlagEnum.h"

struct FlagInformation
{
   int FixMe;
};

void CreateFlagInformation(struct FlagInformation** ppFlagInformation);
void FreeFlagInformation(struct FlagInformation** ppFlagInformation);
int GetNumberOfFlags(struct FlagInformation* pFlagInformation);
const char* GetStateName(struct FlagInformation* pFlagInformation, enum Flags flag);
const char* GetCapital(struct FlagInformation* pFlagInformation, enum Flags flag);
int GetStateAreaSqKM(struct FlagInformation* pFlagInformation, enum Flags flag);
int GetStateAreaSqMiles(struct FlagInformation* pFlagInformation, enum Flags flag);
int GetStatePopulation(struct FlagInformation* pFlagInformation, enum Flags flag);
#ifdef _WIN32
const char* GetPathForFlag(struct FlagInformation* pFlagInformation, enum Flags flag);
#else
unsigned char* GetImgDataForFlag(struct FlagInformation* pFlagInformation, enum Flags flag);
int GetImgDataSizeForFlag(struct FlagInformation* pFlagInformation, enum Flags flag);
#endif

#endif
