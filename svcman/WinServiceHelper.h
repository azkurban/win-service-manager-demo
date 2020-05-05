#pragma once

#define DLL_EXPORT_API extern "C" __declspec(dllexport)

#include "ServiceController.h"
#include "ServiceEnumerator.h"

struct ServiceProcess
{
    int                     PID;
    ServiceString           Name;
    ServiceString           Description;
    ServiceString           GroupName;
    ServiceString           ImagePath;
    ServiceString           Status;
};


class WinServiceHelper
{
private:
    std::vector<ServiceStatusProcess> _winServices;
    void InitServiceList();

    static WinServiceHelper* _instance;

public:
    WinServiceHelper() 
    {
        InitServiceList();
    }
    static WinServiceHelper* GetInstanse() {
        if (!_instance) {
            _instance = new WinServiceHelper();
        }
        _instance->InitServiceList();
        return _instance;
    }

    void GetServiceList(ServiceProcess services[]);
    int ServiceCount();
};

DLL_EXPORT_API
int GetServiceListSize();

DLL_EXPORT_API
void GetServiceList(ServiceProcess services[]);


