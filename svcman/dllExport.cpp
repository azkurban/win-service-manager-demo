#pragma once
#include "pch.h"
#include "dllExport.h"

std::vector<std::wstring> s_strings;
MyStruct arr[];

WinServiceHelper* _serviceHelper;

DLL_EXPORT_API
size_t ServiceCount() 
{
    if (_serviceHelper)
    {
        delete _serviceHelper;
    }
    
    _serviceHelper = new WinServiceHelper();
    return _serviceHelper->ServiceCount();
}

DLL_EXPORT_API
void ServiceList(ServiceProcess* services, size_t count) 
{
    _serviceHelper->ServiceList(services, count);
    //delete _serviceHelper;
}


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

    for (int i = 0; i < size; i++)
    {
        arr[i].IntValue = i;

        auto wstr = s_strings[i];
        auto strSize = wstr.size() + 1;

        arr[i].StringValue = (wchar_t*)CoTaskMemAlloc(strSize * sizeof(wchar_t));
        //swprintf_s(arr[i].StringValue, strSize, wstr.c_str());
        wmemcpy(arr[i].StringValue, wstr.c_str(), strSize);


        arr[i].StringValue2 = (wchar_t*)CoTaskMemAlloc(stringSize);
        swprintf_s(arr[i].StringValue2, 255, L"NextVal%i", i);
        //wmemcpy(arr[i].StringValue2, L"NextVal%i", (L"NextVal%i").size());

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


