#pragma once

#include<vector>
#include "OleAuto.h"

#define SERVICE_CONTROL_OPERATOR_SUCCEED 0
#define SERVICE_CONTROL_OPERATOR_FAILED -1

#pragma pack(1)
//structure for service control status info retrieval
typedef struct _ServiceContrlolState
{
    DWORD   CurrentState;
    DWORD   Win32ExitCode;
    DWORD   CheckPoint;
    DWORD   WaitHint;
    DWORD   ProcessId;
    DWORD   ErrorCode;
    BSTR    ServiceName;
    BSTR    Message;
} ServiceControlState;

typedef struct _ServiceProcess
{
    DWORD          ProcessId;
    BSTR           Name;
    BSTR           Description;
    BSTR           ImagePath;
    BSTR           Status;
    //BSTR         GroupName;
} ServiceProcess;
#pragma pack() //reset pack size to default

extern struct ServiceStatusProcess;

// For the following definitions original code from: 
// https://github.com/microsoft/Windows-classic-samples/blob/master/Samples/Win7Samples/winbase/monitorservices/monsvc/MonSvc.cxx
//

//
// Structs
//
typedef struct _NOTIFY_CONTEXT
{
    LPCWSTR   ServiceName;
} NOTIFY_CONTEXT, * PNOTIFY_CONTEXT;

// **************************************************************************
// Function Name: NotifyCallback
//
// Purpose: Invoked on service state change notification. 
//
// Arguments:
//    IN PVOID pParameter - Callback context
//
// Return Values:
//    None
VOID CALLBACK NotifyCallback(PVOID pParameter);

// **************************************************************************
// Function Name: SvcDebugOut()
//
// Purpose: Output error message to the debugger. 
//
// Arguments:
//    IN LPTSTR String - Error message
//    IN DWORD Status - Error code
//
// Return Values:
//    None
VOID SvcDebugOut(const wchar_t* String, DWORD Status);

// **************************************************************************
// Function Name: WriteMonitorLog()
//
// Purpose: Helper routine for logging service status changes 
//
// Arguments:
//    IN LPTSTR szStatus - Log message
//
// Return Values:
//    None
VOID WriteMonitorLog(LPTSTR szStatus);

class WinServiceHelper
{
private:
    std::vector<ServiceStatusProcess> _winServices;
    void InitServiceList();
    void CopyStrValue(std::wstring svalue, BSTR& target);
    //void CopyLPWStrValue(LPCWSTR svalue, BSTR& target);
    BOOL __stdcall StopDependentServices(SC_HANDLE schService, SC_HANDLE schSCManager);

    // **************************************************************************
    // Function Name: StartMonitor
    //
    // Purpose: Monitoring routine
    //
    // Return Values:
    //    Win32 Exit code
    DWORD StartMonitor(LPCWSTR  serviceName, SC_HANDLE   hService, DWORD timeOut);

    void FillStatus(LPCWSTR statusMessage, SERVICE_STATUS_PROCESS ssStatus, ServiceControlState* pSCState, DWORD   ErrorCode);
    void FillStatus(LPCWSTR statusMessage, ServiceControlState* pSCState, DWORD   errorCode);

public:
    WinServiceHelper();
    ~WinServiceHelper();

    void ServiceList(ServiceProcess* services, size_t count);
    size_t ServiceCount();

    int StartSvc(LPCWSTR serviceName, ServiceControlState* pSCState);
    int StopSvc(LPCWSTR serviceName, ServiceControlState* pSCState);
    int RestartSvc(LPCWSTR serviceName, ServiceControlState* pSCState);

};


