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
    int size = min(_winServices.size(), count);

    for (int i = 0; i < size; i++) {
        auto ws = _winServices[i];
        auto sp = services[i];

        // open the service
        auto service = ServiceController{ ws.ServiceName };
        auto config = service.GetServiceConfig();

        sp.PID = 0;

        CopyStrValue(ws.ServiceName, sp.Name);
        CopyStrValue(ws.DisplayName, sp.Description);

        auto status = ServiceStatusToString(static_cast<ServiceStatus>(ws.Status.dwCurrentState));
        CopyStrValue(status, sp.Status);

        auto path = config.GetBinaryPathName();
        CopyStrValue(path, sp.ImagePath);
    }
}

size_t WinServiceHelper::ServiceCount()
{
    return _winServices.size();
}

void  WinServiceHelper::CopyStrValue(std::wstring svalue, BSTR target) {
    auto size = svalue.size() + 1;
    target = (wchar_t*)CoTaskMemAlloc(size * sizeof(wchar_t));
    swprintf_s(target, size, svalue.c_str());
}


