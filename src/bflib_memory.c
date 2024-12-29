/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_memory.c
 *     Memory managing routines.
 * @par Purpose:
 *     Memory related routines - allocation and freeing of memory blocks.
 * @par Comment:
 *     Original SW used different functions for allocating low and extended memory.
 *     This is now outdated way, so functions here are simplified to originals.
 * @author   Tomasz Lis
 * @date     10 Feb 2008 - 30 Dec 2008
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "pre_inc.h"
#include "bflib_memory.h"

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "post_inc.h"

/******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#if defined(_WIN32)

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void*)0)
#endif
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#ifndef VOID
#define VOID void
#endif

#ifndef WINBASEAPI
#ifdef __W32API_USE_DLLIMPORT__
#define WINBASEAPI DECLSPEC_IMPORT
#else
#define WINBASEAPI
#endif
#endif

#define WINAPI __stdcall
#define WINAPIV __cdecl
#define APIENTRY __stdcall
#define CALLBACK __stdcall

typedef unsigned long DWORD;
typedef char CHAR;
typedef short SHORT;
typedef long LONG;
typedef char CCHAR, *PCCHAR;
typedef unsigned char UCHAR,*PUCHAR;
typedef unsigned short USHORT,*PUSHORT;
typedef unsigned long ULONG,*PULONG;
typedef char *PSZ;

typedef struct _MEMORYSTATUS {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    DWORD dwTotalPhys;
    DWORD dwAvailPhys;
    DWORD dwTotalPageFile;
    DWORD dwAvailPageFile;
    DWORD dwTotalVirtual;
    DWORD dwAvailVirtual;
} MEMORYSTATUS,*LPMEMORYSTATUS;

WINBASEAPI VOID WINAPI GlobalMemoryStatus(LPMEMORYSTATUS);

#endif
/******************************************************************************/
#ifdef __cplusplus
}
#endif
/******************************************************************************/
static unsigned long lbMemoryAvailable=0;
static short lbMemorySetup=0;

char lbEmptyString[] = "";
unsigned long mem_size;
/******************************************************************************/
/**
 * Updates memory status variables.
 */
short update_memory_constraits(void)
{
  LbMemoryCheck();
  if (lbMemoryAvailable <= (8 * 1024 * 1024))
  {
      mem_size = 8;
      WARNLOG("Very limited memory available: %lu, PhysicalMemory %lu\n", lbMemoryAvailable, mem_size);
  }
  else
  {
      mem_size = 64;
      LbSyncLog("PhysicalMemory %lu\n", mem_size);
  }
  return true;
}

/**
 *
 * Appends characters of source to destination, plus a terminating null-character.
 * Prevents string in dst of getting bigger than maxlen characters.
 */
void * LbStringConcat(char *dst, const char *src, const ulong dst_buflen)
{
  int max_num=dst_buflen-strlen(dst);
  if (max_num<=0) return dst;
  strncat(dst, src, max_num);
  dst[dst_buflen-1]='\0';
  return dst;
}

void * LbStringCopy(char *dst, const char *src, const ulong dst_buflen)
{
  if (dst_buflen < 1)
    return dst;
  snprintf(dst, dst_buflen, "%s", src);
  return dst;
}

void * LbStringToLowerCopy(char *dst, const char *src, const ulong dst_buflen)
{
  if (dst_buflen < 1)
    return dst;
  for (int i = 0; i < dst_buflen; i++)
  {
      char chr = tolower(src[i]);
      dst[i] = chr;
      if (chr == '\0')
          break;
  }
  dst[dst_buflen-1]='\0';
  return dst;
}

ulong LbStringLength(const char *str)
{
    if (str == NULL) return 0;
    return strlen(str);
}

int LbMemorySetup()
{
    if (lbMemorySetup != 0)
        return 0;
    lbMemorySetup = 1;
    /* Heap handling by application is only required for some platforms
    if (heap_handle == NULL)
    {
        heap_handle = HeapCreate(heap_handle, heap_handle, heap_handle);
    }
    if (heap_handle == NULL)
        return -1;
    */
    return 1;
}

int LbMemoryReset(void)
{
    if (lbMemorySetup == 0)
        return 0;
    lbMemorySetup = 0;
    /* Heap handling by application is only required for some platforms
    if (heap_handle != NULL)
    {
        if (!HeapDestroy(heap_handle))
            return -1;
        heap_handle = NULL;
    }
    */
    return 1;
}

void * LbMemoryAlloc(ulong size)
{
    void * ptr = malloc(size);
    if (ptr) memset(ptr, 0, size);
    return ptr;
}

short LbMemoryCheck(void)
{
#if defined(_WIN32)
  struct _MEMORYSTATUS msbuffer;
  msbuffer.dwLength = 32;
  GlobalMemoryStatus(&msbuffer);
  lbMemoryAvailable=msbuffer.dwTotalPhys;
#else
  lbMemoryAvailable=536870912;
#endif
  return 1;
}

/******************************************************************************/
