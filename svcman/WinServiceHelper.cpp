#pragma once
#include "pch.h"

#include "WinServiceHelper.h"
#include "ServiceController.h"
#include "ServiceEnumerator.h"

// Globals
//
//SERVICE_STATUS          ssService;
//SERVICE_STATUS_HANDLE   hssService;

HANDLE                  g_hEvent;


WinServiceHelper::WinServiceHelper()
{
    InitServiceList();
}

void WinServiceHelper::InitServiceList()
{
    _winServices = ServiceEnumerator::EnumerateServices();
}

WinServiceHelper::~WinServiceHelper() {}

void WinServiceHelper::ServiceList(ServiceProcess* services, size_t count)
{
    size_t size = min(_winServices.size(), count);

    for (size_t i = 0; i < size; i++) {
        auto ws = _winServices[i];

        // open the service
        auto service = ServiceController{ ws.ServiceName };
        auto config = service.GetServiceConfig();

        services[i].ProcessId = ws.Status.dwProcessId;

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

int WinServiceHelper::RestartSvc(LPCWSTR serviceName, ServiceControlState* pSCState) {

    auto result = StopSvc(serviceName, pSCState);

    if (result == SERVICE_CONTROL_OPERATOR_FAILED)
        return SERVICE_CONTROL_OPERATOR_FAILED;

    return StartSvc(serviceName, pSCState);
}

// 
// Purpose: 
//   Starts the service if possible.
//
// Parameters:
//   None
// 
// Return value:
//   None
// Original sample code here: https://docs.microsoft.com/en-us/windows/win32/services/starting-a-service
//
int WinServiceHelper::StartSvc(LPCWSTR serviceName, ServiceControlState* pSCState)
{
    SERVICE_STATUS_PROCESS ssStatus;
    DWORD dwOldCheckPoint;
    DWORD dwStartTickCount;
    DWORD dwWaitTime;
    DWORD dwBytesNeeded;

    LPCWSTR statusMessage;

    // Get a handle to the SCM database. 
    auto schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // servicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
   
    if (NULL == schSCManager)
    {
        pSCState->ErrorCode = GetLastError();
        statusMessage = _T("OpenSCManager failed");
        CopyStrValue(statusMessage, pSCState->Message);
        printf("%ws (%d)\n", statusMessage, pSCState->ErrorCode);

        return SERVICE_CONTROL_OPERATOR_FAILED;
    }

    
    // Get a handle to the service.

    auto schService = OpenService(
        schSCManager,         // SCM database 
        serviceName,            // name of service 
        SERVICE_ALL_ACCESS);  // full access 

    if (schService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return SERVICE_CONTROL_OPERATOR_FAILED;
    }

    // Check the status in case the service is not stopped. 

    if (!QueryServiceStatusEx(
        schService,                     // handle to service 
        SC_STATUS_PROCESS_INFO,         // information level
        (LPBYTE)&ssStatus,             // address of structure
        sizeof(SERVICE_STATUS_PROCESS), // size of structure
        &dwBytesNeeded))              // size needed if buffer is too small
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return SERVICE_CONTROL_OPERATOR_FAILED;
    }

    // Check if the service is already running. It would be possible 
    // to stop the service here, but for simplicity this example just returns. 

    if (ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
    {
        statusMessage = _T("Service is already running");
        CopyStrValue(serviceName, pSCState->ServiceName);
        CopyStrValue(statusMessage, pSCState->Message);
        printf("%ws.\n", statusMessage);
        //printf("Cannot start the service because it is already running\n");
        
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return SERVICE_CONTROL_OPERATOR_FAILED;
    }

    // Save the tick count and initial checkpoint.

    dwStartTickCount = GetTickCount();
    dwOldCheckPoint = ssStatus.dwCheckPoint;

    // Wait for the service to stop before attempting to start it.

    while (ssStatus.dwCurrentState == SERVICE_STOP_PENDING)
    {
        // Do not wait longer than the wait hint. A good interval is 
        // one-tenth of the wait hint but not less than 1 second  
        // and not more than 10 seconds. 

        dwWaitTime = ssStatus.dwWaitHint / 10;

        if (dwWaitTime < 1000)
            dwWaitTime = 1000;
        else if (dwWaitTime > 10000)
            dwWaitTime = 10000;

        //Sleep(dwWaitTime);

        auto errorCode = StartMonitor(serviceName, schService, dwWaitTime);

        if (errorCode != ERROR_SUCCESS)
        {
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return SERVICE_CONTROL_OPERATOR_FAILED;
        }


        // Check the status until the service is no longer stop pending. 

        if (!QueryServiceStatusEx(
            schService,                     // handle to service 
            SC_STATUS_PROCESS_INFO,         // information level
            (LPBYTE)&ssStatus,             // address of structure
            sizeof(SERVICE_STATUS_PROCESS), // size of structure
            &dwBytesNeeded))              // size needed if buffer is too small
        {
            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return SERVICE_CONTROL_OPERATOR_FAILED;
        }

        if (ssStatus.dwCheckPoint > dwOldCheckPoint)
        {
            // Continue to wait and check.

            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        }
        else
        {
            if (GetTickCount() - dwStartTickCount > ssStatus.dwWaitHint)
            {
                printf("Timeout waiting for service to stop\n");
                CloseServiceHandle(schService);
                CloseServiceHandle(schSCManager);
                return SERVICE_CONTROL_OPERATOR_FAILED;
            }
        }
    }

    // Attempt to start the service.

    if (!StartService(
        schService,  // handle to service 
        0,           // number of arguments 
        NULL))      // no arguments 
    {
        printf("StartService failed (%d)\n", GetLastError());
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return SERVICE_CONTROL_OPERATOR_FAILED;
    }
    else printf("Service start pending...\n");

    // Check the status until the service is no longer start pending. 

    if (!QueryServiceStatusEx(
        schService,                     // handle to service 
        SC_STATUS_PROCESS_INFO,         // info level
        (LPBYTE)&ssStatus,             // address of structure
        sizeof(SERVICE_STATUS_PROCESS), // size of structure
        &dwBytesNeeded))              // if buffer too small
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        return SERVICE_CONTROL_OPERATOR_FAILED;
    }

    // Save the tick count and initial checkpoint.

    dwStartTickCount = GetTickCount();
    dwOldCheckPoint = ssStatus.dwCheckPoint;

    while (ssStatus.dwCurrentState == SERVICE_START_PENDING)
    {
        // Do not wait longer than the wait hint. A good interval is 
        // one-tenth the wait hint, but no less than 1 second and no 
        // more than 10 seconds. 

        dwWaitTime = ssStatus.dwWaitHint / 10;

        if (dwWaitTime < 1000)
            dwWaitTime = 1000;
        else if (dwWaitTime > 10000)
            dwWaitTime = 10000;

        //Sleep(dwWaitTime);
        auto errorCode = StartMonitor(serviceName, schService, dwWaitTime);

        if (errorCode != ERROR_SUCCESS)
        {
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            return SERVICE_CONTROL_OPERATOR_FAILED;
        }

        // Check the status again. 

        if (!QueryServiceStatusEx(
            schService,             // handle to service 
            SC_STATUS_PROCESS_INFO, // info level
            (LPBYTE)&ssStatus,             // address of structure
            sizeof(SERVICE_STATUS_PROCESS), // size of structure
            &dwBytesNeeded))              // if buffer too small
        {
            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
            break;
        }

        if (ssStatus.dwCheckPoint > dwOldCheckPoint)
        {
            // Continue to wait and check.

            dwStartTickCount = GetTickCount();
            dwOldCheckPoint = ssStatus.dwCheckPoint;
        }
        else
        {
            if (GetTickCount() - dwStartTickCount > ssStatus.dwWaitHint)
            {
                // No progress made within the wait hint.
                break;
            }
        }
    }

    // Determine whether the service is running.

    if (ssStatus.dwCurrentState == SERVICE_RUNNING)
    {
        statusMessage = _T("Service started successfully");
    }
    else
    {
        statusMessage = _T("Service not started");
        //printf("Service not started. \n");
    }

    CopyStrValue(serviceName, pSCState->ServiceName);
    CopyStrValue(statusMessage, pSCState->Message);
    printf("%ws.\n", statusMessage);

    pSCState->CurrentState= ssStatus.dwCurrentState;
    pSCState->Win32ExitCode= ssStatus.dwWin32ExitCode;
    pSCState->CheckPoint= ssStatus.dwCheckPoint;
    pSCState->WaitHint= ssStatus.dwWaitHint;
    pSCState->ProcessId= ssStatus.dwProcessId;

    printf("  Current State: %d\n", ssStatus.dwCurrentState);
    printf("  Exit Code: %d\n", ssStatus.dwWin32ExitCode);
    printf("  Check Point: %d\n", ssStatus.dwCheckPoint);
    printf("  Wait Hint: %d\n", ssStatus.dwWaitHint);

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return SERVICE_CONTROL_OPERATOR_SUCCEED;
}

//
// Purpose: 
//   Stops the service.
//
// Parameters:
//   None
// 
// Return value:
//   None
// Original sample code here: https://docs.microsoft.com/en-us/windows/win32/services/stopping-a-service
//
int WinServiceHelper::StopSvc(LPCWSTR serviceName, ServiceControlState* pSCState)
{
    SERVICE_STATUS_PROCESS ssp;
    DWORD dwStartTime = GetTickCount();
    DWORD dwBytesNeeded;
    DWORD dwTimeout = 30000; // 30-second time-out
    DWORD dwWaitTime;

    LPCWSTR statusMessage;

    // Get a handle to the SCM database. 
    auto schSCManager = OpenSCManager(
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 

    if (NULL == schSCManager)
    {
        printf("OpenSCManager failed (%d)\n", GetLastError());
        return SERVICE_CONTROL_OPERATOR_FAILED;
    }

    // Get a handle to the service.

    auto schService = OpenService(
        schSCManager,         // SCM database 
        serviceName,            // name of service 
        SERVICE_STOP |
        SERVICE_QUERY_STATUS |
        SERVICE_ENUMERATE_DEPENDENTS);

    if (schService == NULL)
    {
        printf("OpenService failed (%d)\n", GetLastError());
        CloseServiceHandle(schSCManager);
        return SERVICE_CONTROL_OPERATOR_FAILED;
    }

    // Make sure the service is not already stopped.

    if (!QueryServiceStatusEx(
        schService,
        SC_STATUS_PROCESS_INFO,
        (LPBYTE)&ssp,
        sizeof(SERVICE_STATUS_PROCESS),
        &dwBytesNeeded))
    {
        printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
        goto stop_cleanup;
    }

    if (ssp.dwCurrentState == SERVICE_STOPPED)
    {
        statusMessage = _T("Service is already stopped");
        CopyStrValue(serviceName, pSCState->ServiceName);
        CopyStrValue(statusMessage, pSCState->Message);
        printf("%ws.\n", statusMessage);
        //printf("Service is already stopped.\n");
        goto stop_cleanup;
    }

    // If a stop is pending, wait for it.

    while (ssp.dwCurrentState == SERVICE_STOP_PENDING)
    {
        printf("Service stop pending...\n");

        // Do not wait longer than the wait hint. A good interval is 
        // one-tenth of the wait hint but not less than 1 second  
        // and not more than 10 seconds. 

        dwWaitTime = ssp.dwWaitHint / 10;

        if (dwWaitTime < 1000)
            dwWaitTime = 1000;
        else if (dwWaitTime > 10000)
            dwWaitTime = 10000;

        Sleep(dwWaitTime);

        if (!QueryServiceStatusEx(
            schService,
            SC_STATUS_PROCESS_INFO,
            (LPBYTE)&ssp,
            sizeof(SERVICE_STATUS_PROCESS),
            &dwBytesNeeded))
        {
            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
            goto stop_cleanup;
        }

        if (ssp.dwCurrentState == SERVICE_STOPPED)
        {
            statusMessage = _T("Service stopped successfully");
            CopyStrValue(serviceName, pSCState->ServiceName);
            CopyStrValue(statusMessage, pSCState->Message);
            printf("%ws.\n", statusMessage);
            //printf("Service stopped successfully.\n");
            goto stop_cleanup;
        }

        if (GetTickCount() - dwStartTime > dwTimeout)
        {
            printf("Service stop timed out.\n");
            goto stop_cleanup;
        }
    }

    // If the service is running, dependencies must be stopped first.

    StopDependentServices(schService, schSCManager);

    // Send a stop code to the service.

    if (!ControlService(
        schService,
        SERVICE_CONTROL_STOP,
        (LPSERVICE_STATUS)&ssp))
    {
        printf("ControlService failed (%d)\n", GetLastError());
        goto stop_cleanup;
    }

    // Wait for the service to stop.

    while (ssp.dwCurrentState != SERVICE_STOPPED)
    {
        Sleep(ssp.dwWaitHint);
        if (!QueryServiceStatusEx(
            schService,
            SC_STATUS_PROCESS_INFO,
            (LPBYTE)&ssp,
            sizeof(SERVICE_STATUS_PROCESS),
            &dwBytesNeeded))
        {
            printf("QueryServiceStatusEx failed (%d)\n", GetLastError());
            goto stop_cleanup;
        }

        if (ssp.dwCurrentState == SERVICE_STOPPED)
            break;

        if (GetTickCount() - dwStartTime > dwTimeout)
        {
            printf("Wait timed out\n");
            goto stop_cleanup;
        }
    }

    statusMessage = _T("Service stopped successfully");
    CopyStrValue(serviceName, pSCState->ServiceName);
    CopyStrValue(statusMessage, pSCState->Message);
    printf("%ws.\n", statusMessage);

stop_cleanup:
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return SERVICE_CONTROL_OPERATOR_SUCCEED;
}

BOOL __stdcall WinServiceHelper::StopDependentServices(SC_HANDLE schService, SC_HANDLE schSCManager)
{
    DWORD i;
    DWORD dwBytesNeeded;
    DWORD dwCount;

    LPENUM_SERVICE_STATUS   lpDependencies = NULL;
    ENUM_SERVICE_STATUS     ess;
    SC_HANDLE               hDepService;
    SERVICE_STATUS_PROCESS  ssp;

    DWORD dwStartTime = GetTickCount();
    DWORD dwTimeout = 30000; // 30-second time-out

    // Pass a zero-length buffer to get the required buffer size.
    if (EnumDependentServices(schService, SERVICE_ACTIVE,
        lpDependencies, 0, &dwBytesNeeded, &dwCount))
    {
        // If the Enum call succeeds, then there are no dependent
        // services, so do nothing.
        return TRUE;
    }
    else
    {
        if (GetLastError() != ERROR_MORE_DATA)
            return FALSE; // Unexpected error

        // Allocate a buffer for the dependencies.
        lpDependencies = (LPENUM_SERVICE_STATUS)HeapAlloc(
            GetProcessHeap(), HEAP_ZERO_MEMORY, dwBytesNeeded);

        if (!lpDependencies)
            return FALSE;

        __try {
            // Enumerate the dependencies.
            if (!EnumDependentServices(schService, SERVICE_ACTIVE,
                lpDependencies, dwBytesNeeded, &dwBytesNeeded,
                &dwCount))
                return FALSE;

            for (i = 0; i < dwCount; i++)
            {
                ess = *(lpDependencies + i);
                // Open the service.
                hDepService = OpenService(schSCManager,
                    ess.lpServiceName,
                    SERVICE_STOP | SERVICE_QUERY_STATUS);

                if (!hDepService)
                    return FALSE;

                __try {
                    // Send a stop code.
                    if (!ControlService(hDepService,
                        SERVICE_CONTROL_STOP,
                        (LPSERVICE_STATUS)&ssp))
                        return FALSE;

                    // Wait for the service to stop.
                    while (ssp.dwCurrentState != SERVICE_STOPPED)
                    {
                        Sleep(ssp.dwWaitHint);
                        if (!QueryServiceStatusEx(
                            hDepService,
                            SC_STATUS_PROCESS_INFO,
                            (LPBYTE)&ssp,
                            sizeof(SERVICE_STATUS_PROCESS),
                            &dwBytesNeeded))
                            return FALSE;

                        if (ssp.dwCurrentState == SERVICE_STOPPED)
                            break;

                        if (GetTickCount() - dwStartTime > dwTimeout)
                            return FALSE;
                    }
                }
                __finally
                {
                    // Always release the service handle.
                    CloseServiceHandle(hDepService);
                }
            }
        }
        __finally
        {
            // Always free the enumeration buffer.
            HeapFree(GetProcessHeap(), 0, lpDependencies);
        }
    }
    return TRUE;
}

DWORD WinServiceHelper::StartMonitor(LPCWSTR  serviceName, SC_HANDLE   hService, DWORD timeOut)
{
    long lResult;

    DWORD       dwError = ERROR_SUCCESS;
    DWORD       dwStatus;
    DWORD       dwMask;
    DWORD       dwBufSize;
    
    NOTIFY_CONTEXT NotifyContext = { 0 };
    SERVICE_NOTIFY  snServiceNotify;

    // Initialize callback context
    NotifyContext.ServiceName = serviceName;

    // Intialize notification struct
    snServiceNotify.dwVersion = SERVICE_NOTIFY_STATUS_CHANGE;
    snServiceNotify.pfnNotifyCallback = (PFN_SC_NOTIFY_CALLBACK)NotifyCallback;
    snServiceNotify.pContext = &NotifyContext;

    // We care about changes to RUNNING and STOPPED states only
    dwMask = SERVICE_NOTIFY_RUNNING | SERVICE_NOTIFY_STOPPED;

    while (TRUE)
    {
        // Register for notification
        dwStatus = NotifyServiceStatusChange(hService, dwMask, &snServiceNotify);

        if (dwStatus != ERROR_SUCCESS)
        {
            SvcDebugOut(TEXT("NSSC failed - "), dwStatus);
            dwError = dwStatus;
            goto FnExit;
        }

        // Wait for notification to fire (or) for STOP control
        dwStatus = WaitForSingleObjectEx(g_hEvent, timeOut, TRUE);

        // Check if this was signaled due to a SERVICE_STOP control
        if (dwStatus == WAIT_OBJECT_0)
        {
            break;
        }
    }

FnExit:

    return dwError;
}

void  WinServiceHelper::CopyStrValue(std::wstring svalue, BSTR& target) {
    size_t size = svalue.size() + 1;

    if (!svalue.empty())
    {
        target = (wchar_t*)CoTaskMemAlloc(size * sizeof(wchar_t));
        wmemcpy(target, svalue.c_str(), size);
    }
}



VOID CALLBACK NotifyCallback(PVOID pParameter)
{
    HRESULT hr = S_OK;
    PSERVICE_NOTIFY         pNotify = (PSERVICE_NOTIFY)pParameter;
    PNOTIFY_CONTEXT         pContext = (PNOTIFY_CONTEXT)pNotify->pContext;
    TCHAR szStatus[1024];

    if (pNotify->ServiceStatus.dwCurrentState == SERVICE_RUNNING)
    {
        hr = StringCchPrintf(szStatus, 1024, TEXT("%s %s.\r\n"), pContext->ServiceName, TEXT("entered running state"));
    }
    else
    {
        hr = StringCchPrintf(szStatus, 1024, TEXT("%s %s.\r\n"), pContext->ServiceName, TEXT("entered stopped state"));
    }

    if (hr != S_OK)
    {
        OutputDebugString(TEXT("Error creating status msg"));
    }
    else
    {
        WriteMonitorLog(szStatus);
    }
}

VOID WriteMonitorLog(LPTSTR szStatus)
{
    HANDLE hLogFile = INVALID_HANDLE_VALUE;
    HRESULT hr = S_OK;
    LPCTSTR szFileNameSuffix = TEXT("\\MonSvc\\SvcMonitor.log");
    TCHAR szFileName[MAX_PATH];
    BOOL bRet = FALSE;
    DWORD dwSize;

    if (szStatus == NULL)
    {
        SvcDebugOut(TEXT("Invalid service status - "), ERROR_INVALID_PARAMETER);
        goto FnExit;
    }

    dwSize = ExpandEnvironmentStrings(TEXT("%ProgramFiles%"), szFileName, MAX_PATH);
    if (dwSize == 0 || dwSize > MAX_PATH)
    {
        SvcDebugOut(TEXT("File name too long - "), GetLastError());
        goto FnExit;
    }

    hr = StringCchCat(szFileName, MAX_PATH, szFileNameSuffix);
    if (hr != S_OK)
    {
        SvcDebugOut(TEXT("File name too long - "), ERROR_INSUFFICIENT_BUFFER);
        goto FnExit;
    }


    hLogFile = CreateFile(szFileName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hLogFile == INVALID_HANDLE_VALUE)
    {
        SvcDebugOut(TEXT("Cannot open monitor log - "), GetLastError());
        goto FnExit;
    }

    SetFilePointer(hLogFile, 0, NULL, FILE_END);
    bRet = WriteFile(hLogFile, szStatus, DWORD((_tcslen(szStatus)) * sizeof(TCHAR)), &dwSize, NULL);

    if (!bRet)
    {
        SvcDebugOut(TEXT("Cannot write to  log - "), GetLastError());
    }

FnExit:
    CloseHandle(hLogFile);
}

VOID SvcDebugOut(const wchar_t* String, DWORD Status)
{
    HRESULT hr = S_OK;
    TCHAR  Buffer[1024];
    hr = StringCchPrintf(Buffer, 1024, String, Status);
    if (hr == S_OK)
    {
        OutputDebugString(Buffer);
    }
    else
    {
        OutputDebugString(TEXT("Error in Dbg string"));
    }
}




