// NativeTests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ServiceController.h"
#include "ServiceEnumerator.h"

#include <iostream>

int main()
{
   auto services = ServiceEnumerator::EnumerateServices();
   for (auto const & s : services)
   {
      std::wcout << "Name:    " << s.ServiceName << std::endl
                 << "Display: " << s.DisplayName << std::endl
                 << "Status:  " << ServiceStatusToString(static_cast<ServiceStatus>(s.Status.dwCurrentState)) << std::endl
                 << "--------------------------" << std::endl;
   }

   // open the service
   auto service = ServiceController{ L"LanmanWorkstation" };

   auto print_status = [&service]() {
      std::wcout << "Status:              " << ServiceStatusToString(service.GetStatus()) << std::endl; 
   };

   auto print_config = [](ServiceConfig const config) {
      std::wcout << "---------------------" << std::endl;
      std::wcout << "Start name:          " << config.GetStartName() << std::endl;
      std::wcout << "Display name:        " << config.GetDisplayName() << std::endl;
      std::wcout << "Description:         " << config.GetDescription() << std::endl;
      std::wcout << "Type:                " << ServiceTypeToString(config.GetType()) << std::endl;
      std::wcout << "Start type:          " << ServiceStartTypeToString(config.GetStartType()) << std::endl;
      std::wcout << "Error control:       " << ServiceErrorControlToString(config.GetErrorControl()) << std::endl;
      std::wcout << "Binary path:         " << config.GetBinaryPathName() << std::endl;
      std::wcout << "Load ordering group: " << config.GetLoadOrderingGroup() << std::endl;
      std::wcout << "Tag ID:              " << config.GetTagId() << std::endl;
      std::wcout << "Dependencies:        ";
      for (auto const & d : config.GetDependencies()) std::wcout << d << ", ";
      std::wcout << std::endl;
      std::wcout << "---------------------" << std::endl;
   };

   // read the service configuration, temporary change its description and then restore the old one
   {
      auto config = service.GetServiceConfig();

      print_config(config);

      auto oldDescription = config.GetDescription();

      auto newDescription = _T("This is a sample description.");

      config.ChangeDescription(newDescription);

      config.Refresh();

      print_config(config);

      config.ChangeDescription(oldDescription);

      config.Refresh();

      print_config(config);
   }

   // check the service status
   print_status();

   // start the service if the service is currently stopped
   if (service.GetStatus() == ServiceStatus::Stopped)
   {
      service.Start();

      print_status();

      service.WaitForStatus(ServiceStatus::Running);

      print_status();
   }

   // if the service and running and it supports pause and continue then first pause and then resume the service
   if (service.GetStatus() == ServiceStatus::Running && service.CanPauseContinue())
   {
      service.Pause();

      print_status();

      service.WaitForStatus(ServiceStatus::Paused);

      print_status();

      service.Continue();

      print_status();

      service.WaitForStatus(ServiceStatus::Running);

      print_status();
   }

   return 0;
}

