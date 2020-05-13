#pragma once
#include "pch.h"
#include "dllExport.h"

std::vector<std::wstring> s_strings;

WinServiceHelper* _serviceHelper;

DLL_EXPORT_API
size_t ServiceCount() 
{
    if (_serviceHelper)
    {
        delete _serviceHelper;
    }
    
    _serviceHelper = new WinServiceHelper();
    return _serviceHelper->ServiceCount();
}

DLL_EXPORT_API
void ServiceList(ServiceProcess* services, size_t count) 
{
    _serviceHelper->ServiceList(services, count);
    //delete _serviceHelper;
    //_serviceHelper = NULL;
}

DLL_EXPORT_API
int StartWinService(BSTR serviceName, ServiceControlState* svcState)
{
    return _serviceHelper->StartSvc(serviceName, svcState);
}

DLL_EXPORT_API
int StopWinService(BSTR serviceName, ServiceControlState* pSCState)
{
    //pSCState->ErrorCode = 123;
    //return -1;

    return _serviceHelper->StopSvc(serviceName, pSCState);
}

DLL_EXPORT_API
int RestartWinService(BSTR serviceName, ServiceControlState* svcState)
{
    return _serviceHelper->RestartSvc(serviceName, svcState);
}
