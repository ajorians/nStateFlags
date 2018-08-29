#ifdef _TINSPIRE
#include <os.h>
#include <libndls.h>
#else
#include "SDL/SDL.h"
#endif
#include "FlagInformation.h"
#include "Replacements.h"

#ifndef _WIN32
#include "Images/af.h"
#endif

#ifndef _WIN32
#define C(e, n, cap, pop, km, img, path) { e, n, cap, km, pop, img, sizeof(img)}
#define P(s) "../../src/" s
#else
#define P(s) s
#define C(e, n, cap, pop, km, img, path) { e, n, cap, km, pop, path}
#endif

struct FlagInfo
{
   enum Flags eFlag;
   const char* pstrStateName;
   const char* pstrCapital;
   int nSqKMs;
   int nPopulation;
#ifdef _WIN32
   const char* pstrImagePath;
#else
   unsigned char* pImg;
   int nImgSize;
#endif
} g_FlagInfo[] = {
   C( Alabama, "Alabama", "Montgomery", 4833722, 135767, al, P("Images/al.bmp")),
   C( Alaska, "Alaska", "Juneau", 735132, 1723337, ak, P("Images/ak.bmp")),
   C( Arizona, "Arizona", "Phoenix", 6626624, 295233, az, P("Images/az.bmp")),
   C( Arkansas, "Arkansas", "Little Rock", 2959373, 137733, ar, P("Images/ar.bmp")),
   C( California, "California", "Sacramento", 38332521, 423968, ca, P("Images/ca.bmp")),
   C( Colorado, "Colorado", "Denver", 5268367, 269602, co, P("Images/co.bmp")),
   C( Connecticut, "Connecticut", "Hartford", 3596080, 14356, ct, P("Images/ct.bmp")),
   C( Delaware, "Delaware", "Dover", 925749, 6446, de, P("Images/de.bmp")),
   C( Florida, "Florida", "Tallahassee", 19552860, 170312, fl, P("Images/fl.bmp")),
   C( Georgia, "Georgia", "Atlanta", 9992167, 153910, ga, P("Images/ga.bmp")),
   C( Hawaii, "Hawaii", "Honolulu", 1404054, 28314, hi, P("Images/hi.bmp")),
   C( Idaho, "Idaho", "Boise", 1612136, 216443, id, P("Images/id.bmp")),
   C( Illinois, "Illinois", "Springfield", 12882135, 149997, il, P("Images/il.bmp")),
   C( Indiana, "Indiana", "Indianapolis", 6570902, 94327, in, P("Images/in.bmp")),
   C( Iowa, "Iowa", "Des Moines", 3090416, 145746, ia, P("Images/ia.bmp")),
   C( Kansas, "Kansas", "Topeka", 2893957, 213099, ks, P("Images/ks.bmp")),
   C( Kentucky, "Kentucky", "Frankfort", 4395295, 104656, ky, P("Images/ky.bmp")),
   C( Louisiana, "Louisiana", "Baton Rouge", 4625470, 135658, la, P("Images/la.bmp")),
   C( Maine, "Maine", "Augusta", 1328302, 91634, me, P("Images/me.bmp")),
   C( Maryland, "Maryland", "Annapolis", 5928814, 32131, md, P("Images/md.bmp")),
   C( Massachusetts, "Massachusetts", "Boston", 6692824, 27335, ma, P("Images/ma.bmp")),
   C( Michigan, "Michigan", "Lansing", 9895622, 250488, mi, P("Images/mi.bmp")),
   C( Minnesota, "Minnesota", "St. Paul", 5420380, 225163, mn, P("Images/mn.bmp")),
   C( Mississippi, "Mississippi", "Jackson", 2991207, 125438, ms, P("Images/ms.bmp")),
   C( Missouri, "Missouri", "Jefferson City", 6021988, 180540, mo, P("Images/mo.bmp")),
   C( Montana, "Montana", "Helena", 1015165, 380832, mt, P("Images/mt.bmp")),
   C( Nebraska, "Nebraska", "Lincoln", 1868516, 200330, ne, P("Images/ne.bmp")),
   C( Nevada, "Nevada", "Carson City", 2790136, 286380, nv, P("Images/nv.bmp")),
   C( NewHampshire, "New Hampshire", "Concord", 1323459, 24214, af, P("Images/ak.bmp")),
   C( NewJersey, "New Jersey", "Trenton", 8899339, 22592, nh, P("Images/nh.bmp")),
   C( NewMexico, "New Mexico", "Santa Fe", 2085287, 314917, nm, P("Images/nm.bmp")),
   C( NewYork, "New York", "Albany", 19651127, 141297, ny, P("Images/ny.bmp")),
   C( NorthCarolina, "North Carolina", "Raleigh", 9848060, 139391, nc, P("Images/nc.bmp")),
   C( NorthDakota, "North Dakota", "Bismarck", 723393, 183107, nd, P("Images/nd.bmp")),
   C( Ohio, "Ohio", "Columbus", 11570808, 116099, oh, P("Images/oh.bmp")),
   C( Oklahoma, "Oklahoma", "Oklahoma City", 3850568, 181038, ok, P("Images/ok.bmp")),
   C( Oregon, "Oregon", "Salem", 3930065, 254800, or, P("Images/or.bmp")),
   C( Pennsylvania, "Pennsylvania", "Harrisburg", 12773801, 119279, pa, P("Images/pa.bmp")),
   C( RhodeIsland, "Rhode Island", "Providence", 1051511, 4002, ri, P("Images/ri.bmp")),
   C( SouthCarolina, "South Carolina", "Columbia", 4774839, 82931, sc, P("Images/sc.bmp")),
   C( SouthDakota, "South Dakota", "Pierre", 844877, 199730, sd, P("Images/sd.bmp")),
   C( Tennessee, "Tennessee", "Nashville", 6495978, 109152, tn, P("Images/tn.bmp")),
   C( Texas, "Texas", "Austin", 26448193, 695660, tx, P("Images/tx.bmp")),
   C( Utah, "Utah", "Salt Lake City", 2900872, 219882, ut, P("Images/ut.bmp")),
   C( Vermont, "Vermont", "Montpelier", 626630, 24905, vt, P("Images/vt.bmp")),
   C( Virginia, "Virginia", "Richmond", 8260405, 110787, va, P("Images/va.bmp")),
   C( Washington, "Washington", "Olympia", 6971406, 184661, wa, P("Images/wa.bmp")),
   C( WestVirginia, "West Virginia", "Charleston", 1854304, 62755, wv, P("Images/wv.bmp")),
   C( Wisconsin, "Wisconsin", "Madison", 5742713, 169634, wi, P("Images/wi.bmp")),
   C( Wyoming, "Wyoming", "Cheyenne", 582658, 253335, wy, P("Images/wy.bmp"))
};

#undef C
#undef P

void CreateFlagInformation(struct FlagInformation** ppFlagInformation)
{
   *ppFlagInformation = malloc(sizeof(struct FlagInformation));
   struct FlagInformation* pFlagInformation = (*ppFlagInformation);

   //Initialize stuff here
   UNUSED(pFlagInformation);
}

void FreeFlagInformation(struct FlagInformation** ppFlagInformation)
{
   struct FlagInformation* pFlagInformation = (*ppFlagInformation);
   UNUSED(pFlagInformation);

   free(*ppFlagInformation);
   *ppFlagInformation = NULL;
}

int GetNumberOfFlags(struct FlagInformation* pFlagInformation)
{
   UNUSED(pFlagInformation);
   return (int)( sizeof(g_FlagInfo) / sizeof(g_FlagInfo[0]) );
}

const char* GetStateName(struct FlagInformation* pFlagInformation, enum Flags flag)
{
   UNUSED(pFlagInformation);
   for (int i = 0; i < (int)(sizeof(g_FlagInfo) / sizeof(g_FlagInfo[0])); i++)
      if (g_FlagInfo[i].eFlag == flag)
         return g_FlagInfo[i].pstrStateName;
   return "";
}

const char* GetCapital(struct FlagInformation* pFlagInformation, enum Flags flag)
{
   UNUSED(pFlagInformation);
   for (int i = 0; i < (int)(sizeof(g_FlagInfo) / sizeof(g_FlagInfo[0])); i++)
      if (g_FlagInfo[i].eFlag == flag)
         return g_FlagInfo[i].pstrCapital;
   return "";
}

int GetStateAreaSqKM(struct FlagInformation* pFlagInformation, enum Flags flag)
{
   UNUSED(pFlagInformation);
   for (int i = 0; i < (int)(sizeof(g_FlagInfo) / sizeof(g_FlagInfo[0])); i++)
      if (g_FlagInfo[i].eFlag == flag)
         return g_FlagInfo[i].nSqKMs;
   return -1;
}

int GetStateAreaSqMiles(struct FlagInformation* pFlagInformation, enum Flags flag)
{
   double dSquareKM = (double)GetStateAreaSqKM(pFlagInformation, flag);
   double dSquareMiles = dSquareKM * 0.386102;
   return (int)dSquareMiles;
}

int GetStatePopulation(struct FlagInformation* pFlagInformation, enum Flags flag)
{
   UNUSED(pFlagInformation);
   for (int i = 0; i < (int)(sizeof(g_FlagInfo) / sizeof(g_FlagInfo[0])); i++)
      if (g_FlagInfo[i].eFlag == flag)
         return g_FlagInfo[i].nPopulation;
   return -1;
}

#ifdef _WIN32
const char* GetPathForFlag(struct FlagInformation* pFlagInformation, enum Flags flag)
{
   (pFlagInformation);
   for (int i = 0; i < sizeof(g_FlagInfo) / sizeof(g_FlagInfo[0]); i++)
      if (g_FlagInfo[i].eFlag == flag)
         return g_FlagInfo[i].pstrImagePath;
   return "";
}
#else
unsigned char* GetImgDataForFlag(struct FlagInformation* pFlagInformation, enum Flags flag)
{
   UNUSED(pFlagInformation);
   for (int i = 0; i < (int)(sizeof(g_FlagInfo) / sizeof(g_FlagInfo[0])); i++)
      if (g_FlagInfo[i].eFlag == flag)
         return g_FlagInfo[i].pImg;
   return NULL;
}

int GetImgDataSizeForFlag(struct FlagInformation* pFlagInformation, enum Flags flag)
{
   UNUSED(pFlagInformation);
   for (int i = 0; i < (int)(sizeof(g_FlagInfo) / sizeof(g_FlagInfo[0])); i++)
      if (g_FlagInfo[i].eFlag == flag)
         return g_FlagInfo[i].nImgSize;
   return -1;
}
#endif
