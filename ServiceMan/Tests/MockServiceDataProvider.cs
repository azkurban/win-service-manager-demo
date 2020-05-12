using ServiceMan.Services;
using ServiceMan.ViewModel;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ServiceMan.Tests
{
    class MockServiceDataProvider : ServiceProviderBase
    {
        public override ServiceDataSource GetData()
        {
            var data = new ServiceDataSource();


            string imagePath;
            string group = ParseImagePath(@"C:\WINDOWS\system32\wbem\WmiApSrv.exe", out imagePath);

            data.Add(new ServiceViewModel
            (
                pID: "",
                name: @"wmiApSrv",
                description: @"WMI Performance Adapter",
                imagePath: imagePath,
                group: group,
                status: @"Stopped"

            ));

            group = ParseImagePath(@"C:\Program Files\Windows Media Player\wmpnetwk.exe", out imagePath);

            data.Add(new ServiceViewModel
            (
                pID: "",
                name: @"WMPNetworkSvc",
                description: @"Windows Media Player Network Sharing Service",
                imagePath: imagePath,
                group: group,
                status: @"Stopped"
            ));

            group = ParseImagePath("", out imagePath);

            data.Add(new ServiceViewModel
            (
                pID: "",
                name: @"Wof",
                description: @"Windows Overlay File System Filter Driver",
                imagePath: imagePath,
                group: group,
                status: @"Running"
            ));

            group = ParseImagePath(@"C:\WINDOWS\system32\svchost.exe - k netsvcs - p", out imagePath);

            data.Add(new ServiceViewModel
            (

                pID: "4204",
                name: @"WpnService",
                description: @"Windows Push Notifications System Service",
                imagePath: imagePath,
                group: group,
                status: @"Running"
            ));

            group = ParseImagePath("", out imagePath);

            data.Add(new ServiceViewModel
            (
                pID: "5732",
                name: @"wscsvc",
                description: @"Security Center",
                imagePath: imagePath,
                group: group,
                status: @"Running"
            ));

            return data;
        }

    }
}
