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

        //std::wstring str = ws.ServiceName;
        //LPWSTR str = const_cast<LPWSTR>(str.c_str());

        sp.Name = const_cast<LPWSTR>(ws.ServiceName.c_str());

        //sp.PID = 0;
        sp.Name = const_cast<LPWSTR>(ws.ServiceName.c_str());
        sp.Description = const_cast<LPWSTR>(ws.DisplayName.c_str());

        std::wstring status = ServiceStatusToString(static_cast<ServiceStatus>(ws.Status.dwCurrentState));
        sp.Status = const_cast<LPWSTR>(status.c_str());

        std::wstring imagePath = config.GetBinaryPathName();
        sp.ImagePath = const_cast<LPWSTR>(imagePath.c_str());

        services[i] = sp;
    }
}

//int WinServiceHelper::ServiceCount()
//{
//    return _winServices.size();
//}

//DLL_EXPORT_API
//int GetServiceListSize() {
//    auto helper = new WinServiceHelper();
//    return helper->ServiceCount();
//}

DLL_EXPORT_API
void GetServiceList(SAFEARRAY& services) {
    auto helper = new WinServiceHelper();
    //helper->GetServiceList(services);
}

std::vector<std::wstring> s_strings;
MyStruct arr[];

DLL_EXPORT_API
void __stdcall SetStringArray(SAFEARRAY& safeArray)
{
    s_strings.clear();
    if (safeArray.cDims == 1)
    {
        if ((safeArray.fFeatures & FADF_BSTR) == FADF_BSTR)
        {
            BSTR* bstrArray;
            HRESULT hr = SafeArrayAccessData(&safeArray, (void**)&bstrArray);

            long iMin;
            SafeArrayGetLBound(&safeArray, 1, &iMin);
            long iMax;
            SafeArrayGetUBound(&safeArray, 1, &iMax);

            for (long i = iMin; i <= iMax; ++i)
            {
                s_strings.push_back(std::wstring(bstrArray[i]));
            }
        }
    }
}

DLL_EXPORT_API
int SendArray(MyStruct* arr, int recordsCount) {
    size_t size = s_strings.size();
    
    //arr = new MyStruct[recordsCount];
    int stringSize = 255 * sizeof(wchar_t);

    for (int i = 0; i < recordsCount; i++)
    {
        //BSTR item = SysAllocString(s_strings[i].c_str());

        //arr[i].IntValue = i;
        //arr[i].StringValue = SysAllocString(s_strings[i].c_str());

         arr[i].StringValue = (wchar_t*)CoTaskMemAlloc(stringSize);
         swprintf_s(arr[i].StringValue, 255, s_strings[i].c_str());
    }

    return size;
}


DLL_EXPORT_API
void GetStringArray(SAFEARRAY*& pSafeArray)
{
    if (s_strings.size() > 0)
    {
        SAFEARRAYBOUND  Bound;
        Bound.lLbound = 0;
        Bound.cElements = s_strings.size();

        pSafeArray = SafeArrayCreate(VT_BSTR, 1, &Bound);

        BSTR* pData;
        HRESULT hr = SafeArrayAccessData(pSafeArray, (void**)&pData);
        if (SUCCEEDED(hr))
        {
            for (DWORD i = 0; i < s_strings.size(); i++)
            {
                *pData++ = SysAllocString(s_strings[i].c_str());
            }
            SafeArrayUnaccessData(pSafeArray);
        }
    }
    else
    {
        pSafeArray = nullptr;
    }
}

/**/


