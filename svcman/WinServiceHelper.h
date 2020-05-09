#pragma once
#include<vector>

#include "OleAuto.h"

typedef struct _ServiceProcess
{
    DWORD          ProcessId;
    BSTR           Name;
    BSTR           Description;
    BSTR           ImagePath;
    BSTR           Status;
    //BSTR         GroupName;
} ServiceProcess;

typedef struct _MyStruct
{
   int IntValue;
   BSTR StringValue;
   BSTR StringValue2;
} MyStruct;

extern struct ServiceStatusProcess;


class WinServiceHelper
{
private:
    std::vector<ServiceStatusProcess> _winServices;
    void InitServiceList();
    void CopyStrValue(std::wstring svalue, BSTR& target);

public:
    WinServiceHelper();
    ~WinServiceHelper();

    void ServiceList(ServiceProcess* services, size_t count);
    size_t ServiceCount();
};


