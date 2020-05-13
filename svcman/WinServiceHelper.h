#pragma once

#include<vector>
#include "OleAuto.h"

#define SERVICE_CONTROL_OPERATOR_SUCCEED 0
#define SERVICE_CONTROL_OPERATOR_FAILED -1

//#pragma pack(1)
//structure for service control status info retrieval
typedef struct _ServiceContrlolState
{
    DWORD   CurrentState;
    DWORD   Win32ExitCode;
    DWORD   CheckPoint;
    DWORD   WaitHint;
    DWORD   ProcessId;
    DWORD   ErrorCode;
    BSTR    Message;
} ServiceControlState;
//#pragma pack() //reset pack size to default

typedef struct _ServiceProcess
{
    DWORD          ProcessId;
    BSTR           Name;
    BSTR           Description;
    BSTR           ImagePath;
    BSTR           Status;
    //BSTR         GroupName;
} ServiceProcess;

extern struct ServiceStatusProcess;


class WinServiceHelper
{
private:
    std::vector<ServiceStatusProcess> _winServices;
    void InitServiceList();
    void CopyStrValue(std::wstring svalue, BSTR& target);
    //void CopyLPWStrValue(LPCWSTR svalue, BSTR& target);
    BOOL __stdcall StopDependentServices(SC_HANDLE schService, SC_HANDLE schSCManager);

public:
    WinServiceHelper();
    ~WinServiceHelper();

    void ServiceList(ServiceProcess* services, size_t count);
    size_t ServiceCount();

    int StartSvc(LPCWSTR serviceName, ServiceControlState* pSCState);
    int StopSvc(LPCWSTR serviceName, ServiceControlState* pSCState);
    int RestartSvc(LPCWSTR serviceName, ServiceControlState* pSCState);
};


