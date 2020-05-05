#pragma once
#include "pch.h"
#include "WinServiceHelper.h"



void WinServiceHelper::InitServiceList()
{
    _winServices = ServiceEnumerator::EnumerateServices();
}

void WinServiceHelper::GetServiceList(ServiceProcess services[])
{
    int size = _winServices.size();

    for (int i = 0; i < size; i++) {
        auto ws = _winServices[i];

        auto sp = ServiceProcess{};

        // open the service
        auto service = ServiceController{ ws.ServiceName };
        auto config = service.GetServiceConfig();

        sp.PID = 0;
        sp.Name = ws.ServiceName;
        sp.Description = ws.DisplayName;
        sp.Status = ServiceStatusToString(static_cast<ServiceStatus>(ws.Status.dwCurrentState));
        sp.ImagePath = config.GetBinaryPathName();

        services[i] = sp;
    }
}

int WinServiceHelper::ServiceCount()
{
    return _winServices.size();
}

DLL_EXPORT_API
int GetServiceListSize() {
    auto helper = new WinServiceHelper();
    return helper->ServiceCount();
}

DLL_EXPORT_API
void GetServiceList(ServiceProcess services[]) {
    auto helper = new WinServiceHelper();
    helper->GetServiceList(services);
}

