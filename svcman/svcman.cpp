#include "pch.h"
#include "svcman.h"

#include "ServiceController.h"
#include "ServiceEnumerator.h"


SVCMAN_C_FUNCTION
int GetServiceList() {
    auto services = ServiceEnumerator::EnumerateServices();

    for (auto const& s : services)
    {
        //std::wcout << "Name:    " << s.ServiceName << std::endl
        //    << "Display: " << s.DisplayName << std::endl
        //    << "Status:  " << ServiceStatusToString(static_cast<ServiceStatus>(s.Status.dwCurrentState)) << std::endl
        //    << "--------------------------" << std::endl;
    }

    return 123;

}

