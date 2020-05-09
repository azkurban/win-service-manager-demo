#pragma once
#include "pch.h"

#include "WinServiceHelper.h"
#include "ServiceController.h"
#include "ServiceEnumerator.h"

WinServiceHelper::WinServiceHelper()
{
    InitServiceList();
}

void WinServiceHelper::InitServiceList()
{
    _winServices = ServiceEnumerator::EnumerateServices();
}

WinServiceHelper::~WinServiceHelper()
{
    for (auto s : _winServices)
    {
        delete &s;
    }
}

void WinServiceHelper::ServiceList(ServiceProcess* services, size_t count)
{
    size_t size = min(_winServices.size(), count);

    for (size_t i = 0; i < size; i++) {
        auto ws = _winServices[i];
        //auto sp = services[i];

        // open the service
        auto service = ServiceController{ ws.ServiceName };
        auto config = service.GetServiceConfig();

        services[i].PID = i;

        CopyStrValue(ws.ServiceName, services[i].Name);

        int size = ws.ServiceName.size() + 1;

        //if (!ws.ServiceName.empty())
        //{
        //    //auto bs = SysAllocStringLen(svalue.data(), size);
        //    services[i].Name = (wchar_t*)CoTaskMemAlloc(size * sizeof(wchar_t));
        //    wmemcpy(services[i].Name, ws.ServiceName.c_str(), size);
        //    //wcsncpy(target, svalue.c_str(), size - 1);

        //    //swprintf_s(target, size, svalue.c_str());

        //}

        CopyStrValue(ws.DisplayName, services[i].Description);

        auto status = ServiceStatusToString(static_cast<ServiceStatus>(ws.Status.dwCurrentState));
        CopyStrValue(status, services[i].Status);

        auto path = config.GetBinaryPathName();
        CopyStrValue(path, services[i].ImagePath);
    }
}

size_t WinServiceHelper::ServiceCount()
{
    return _winServices.size();
}

void  WinServiceHelper::CopyStrValue(std::wstring svalue, BSTR& target) {
    int size = svalue.size() +1;

    if (!svalue.empty())
    {
        //auto bs = SysAllocStringLen(svalue.data(), size);
        target = (wchar_t*)CoTaskMemAlloc(size * sizeof(wchar_t));
        wmemcpy(target, svalue.c_str(), size);
        //wcsncpy(target, svalue.c_str(), size - 1);

        //swprintf_s(target, size, svalue.c_str());

    }
}


