#pragma once

#define DLL_EXPORT_API extern "C" __declspec(dllexport)

#include "WinServiceHelper.h"

DLL_EXPORT_API
size_t ServiceCount();

DLL_EXPORT_API
void ServiceList(ServiceProcess* services, size_t count);

DLL_EXPORT_API
int StartWinService(BSTR serviceName, ServiceControlState* svcState);

DLL_EXPORT_API
int StopWinService(BSTR serviceName, ServiceControlState* pSCState);

DLL_EXPORT_API
int RestartWinService(BSTR serviceName, ServiceControlState* svcState);

