#pragma once

#define DLL_EXPORT_API extern "C" __declspec(dllexport)

#include "OleAuto.h"
#include "ServiceController.h"
#include "ServiceEnumerator.h"

struct ServiceProcess
{
    //int              PID;
    LPWSTR           Name;
    LPWSTR           Description;
    LPWSTR           GroupName;
    LPWSTR           ImagePath;
    LPWSTR           Status;
};

typedef struct _MyStruct
{
    wchar_t* StringValue;
    //int IntValue;
} MyStruct;


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
    //int ServiceCount();
};

//DLL_EXPORT_API
//int GetServiceListSize();

DLL_EXPORT_API
void GetServiceList(SAFEARRAY& services);

DLL_EXPORT_API
void __stdcall SetStringArray(SAFEARRAY& safeArray);

DLL_EXPORT_API
void GetStringArray(SAFEARRAY*& pSafeArray);

DLL_EXPORT_API
int SendArray(MyStruct* arr, int recordsCount);


