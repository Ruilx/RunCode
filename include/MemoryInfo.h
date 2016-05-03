#ifndef _MEMORYSIZE_H_
#define _MEMORYSIZE_H_

#include <Windows.h>
#include <Psapi.h>

#ifdef __cplusplus
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

EXPORT DWORD CALLBACK getProcessMemory(DWORD processID);

#endif
