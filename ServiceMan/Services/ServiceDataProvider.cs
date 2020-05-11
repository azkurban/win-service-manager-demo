using ServiceMan.Model;
using ServiceMan.ViewModel;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace ServiceMan.Services
{
    class ServiceDataProvider
    {
        public ServiceDataSource GetData()
        {
            ulong count = NativeAPI.ServiceCount();

            var data = new ServiceProcess[count];
            NativeAPI.ServiceList(data, count);

            var dataSource = new ServiceDataSource();

            foreach(var svcInfo in data)
            {
                string imagePath;
                string group = ParseImagePath(svcInfo.ImagePath, out imagePath);


                dataSource.Add(new ServiceViewModel
                {
                    PID = svcInfo.ProcessId == 0 ? string.Empty : svcInfo.ProcessId.ToString(),
                    Name = svcInfo.Name,
                    Description = svcInfo.Description,
                    ImagePath = imagePath,
                    Group = group,
                    Status = svcInfo.Status
                });

            }

            return dataSource;

        }

        private string ParseImagePath(string combinedPath, out string imagePath)
        {
            imagePath = string.Empty;

            if (string.IsNullOrEmpty(combinedPath?.Trim())) return string.Empty;

            string[] pathParts = combinedPath.Split(new[] { "-k" }, StringSplitOptions.RemoveEmptyEntries);

            imagePath = pathParts[0].Trim();
            var group = string.Empty;

            if(pathParts.Length > 1)
            {
                var remainingParts = pathParts[1].Split(new[] { "-p" }, StringSplitOptions.RemoveEmptyEntries);
                group = remainingParts[0].Trim();
            }

            return group;
        }

        public ServiceDataSource GetMockData()
        {
            var data = new ServiceDataSource();

            data.Add(new ServiceViewModel
            {
                PID = "",
                Name = @"wmiApSrv",
                Description = @"WMI Performance Adapter",
                ImagePath = @"C:\WINDOWS\system32\wbem\WmiApSrv.exe",
                Status = @"Stopped"

            });

            data.Add(new ServiceViewModel
            {
                PID = "",
                Name = @"WMPNetworkSvc",
                Description = @"Windows Media Player Network Sharing Service",
                ImagePath = @"C:\Program Files\Windows Media Player\wmpnetwk.exe",
                Status = @"Stopped"
            });

            data.Add(new ServiceViewModel
            {
                PID = "",
                Name = @"Wof",
                Description = @"Windows Overlay File System Filter Driver",
                ImagePath = @"",
                Status = @"Running"
            });

            data.Add(new ServiceViewModel
            {

                PID = "4204",
                Name = @"WpnService",
                Description = @"Windows Push Notifications System Service",
                ImagePath = @"C:\WINDOWS\system32\svchost.exe - k netsvcs - p",
                Status = @"Running"
            });

            data.Add(new ServiceViewModel
            {
                PID = "5732",
                Name = @"wscsvc",
                Description = @"Security Center",
                ImagePath = "",
                Status = @"Running"

            });



            return data;
        }
    }
}
