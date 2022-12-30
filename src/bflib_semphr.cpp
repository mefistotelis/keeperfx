/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_semphr.cpp
 *     Semaphores wrapper.
 * @par Purpose:
 *     Creates unified interface for semaphores, which are used for
 *      sharing access to resources on multiple threads.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     21 May 2009 - 20 Jul 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "pre_inc.h"
#include "bflib_semphr.hpp"

#include "bflib_basics.h"
#include "post_inc.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
#if defined(_WIN32)
//Selected declarations from Win32 API - I don't want to use whole API
// since it influences everything
#ifndef WINBASEAPI
#ifdef __W32API_USE_DLLIMPORT__
#define WINBASEAPI DECLSPEC_IMPORT
#else
#define WINBASEAPI
#endif
#endif
#define WINAPI __stdcall
#ifndef CONST
#define CONST const
#endif
typedef unsigned long DWORD;
typedef long LONG;
typedef char CHAR;
typedef int WINBOOL,*PWINBOOL,*LPWINBOOL;
typedef CONST CHAR *LPCCH,*PCSTR,*LPCSTR;
#define BOOL WINBOOL
#define DECLARE_HANDLE(n) typedef HANDLE n
typedef HANDLE *PHANDLE,*LPHANDLE;
typedef long *LPLONG;
typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES,*PSECURITY_ATTRIBUTES,*LPSECURITY_ATTRIBUTES;
#define INFINITE    0xFFFFFFFF
WINBASEAPI HANDLE WINAPI CreateSemaphoreA(LPSECURITY_ATTRIBUTES,LONG,LONG,LPCSTR);
WINBASEAPI BOOL WINAPI ReleaseSemaphore(HANDLE,LONG,LPLONG);
WINBASEAPI DWORD WINAPI WaitForSingleObject(HANDLE,DWORD);
WINBASEAPI BOOL WINAPI CloseHandle(HANDLE);
#endif
/******************************************************************************/
#ifdef __cplusplus
}
#endif
/******************************************************************************/

LbSemaphore::LbSemaphore(void)
{
#ifdef _WIN32
  this->sHandle = CreateSemaphoreA(NULL, 1, 1, NULL);
#endif
}

LbSemaphore::~LbSemaphore(void)
{
#ifdef _WIN32
  CloseHandle(this->sHandle);
#endif
}

/******************************************************************************/

LbSemaLock::LbSemaLock(class LbSemaphore *sem, int a2)
{
  this->field_4 = a2;
  this->field_8 = 0;
#ifndef _WIN32
  this->semaphore = sem;
#endif
}

LbSemaLock::~LbSemaLock(void)
{
  Release();
}

void LbSemaLock::Release(void)
{
  if ( this->field_4 )
  {
    if ( !this->field_8 )
    {
#ifdef _WIN32
      ReleaseSemaphore(this->sHandle, 1, 0);
#else
      this->sHandle = {};
#endif
      this->field_4 = 0;
    }
  }
}

int LbSemaLock::Lock(TbBool wait_forever)
{
  if (wait_forever)
  {
#ifdef _WIN32
    this->field_4 = WaitForSingleObject(this->sHandle, INFINITE) < 1;
#else
    this->sHandle = std::unique_lock<std::timed_mutex>(this->semaphore->sHandle);
    this->field_4 = this->sHandle.owns_lock() ? 1 : 0;
#endif
    return this->field_4;
  } else
  {
#ifdef _WIN32
    this->field_4 = WaitForSingleObject(this->sHandle, 5) < 1;
#else
    this->sHandle = std::unique_lock<std::timed_mutex>(this->semaphore->sHandle, std::chrono::milliseconds(5));
    this->field_4 = this->sHandle.owns_lock() ? 1 : 0;
#endif
    return this->field_4;
  }
}

/******************************************************************************/
