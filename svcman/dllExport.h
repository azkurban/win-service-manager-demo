#pragma once

#define DLL_EXPORT_API extern "C" __declspec(dllexport)
#include "WinServiceHelper.h"

DLL_EXPORT_API
size_t ServiceCount();

DLL_EXPORT_API
void ServiceList(ServiceProcess* services, size_t count);

DLL_EXPORT_API
void __stdcall SetStringArray(SAFEARRAY& safeArray);

DLL_EXPORT_API
void GetStringArray(SAFEARRAY*& pSafeArray);

DLL_EXPORT_API
int SendArray(MyStruct* arr, int recordsCount);


