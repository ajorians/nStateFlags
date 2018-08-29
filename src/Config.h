#ifndef CONFIG_H
#define CONFIG_H

#ifndef _WIN32
#include "ArchiveLib/ArchiveLib.h"
#endif

struct Config
{
#ifndef _WIN32
   ArchiveLib m_Archive;
#endif
   int m_nLastState;
};

void CreateConfig(struct Config** ppConfig);
void FreeConfig(struct Config** ppConfig);
void SetLastLevel(struct Config* pConfig, int nLastLevel);
int GetLastLevel(struct Config* pConfig);

#endif
