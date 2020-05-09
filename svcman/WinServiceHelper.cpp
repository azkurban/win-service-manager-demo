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

        // open the service
        auto service = ServiceController{ ws.ServiceName };
        auto config = service.GetServiceConfig();

        services[i].PID = i;

        CopyStrValue(ws.ServiceName, services[i].Name);
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
        //target = SysAllocStringLen(svalue.data(), size);

        target = (wchar_t*)CoTaskMemAlloc(size * sizeof(wchar_t));
        wmemcpy(target, svalue.c_str(), size);
        //swprintf_s(target, size, svalue.c_str());

    }
}


