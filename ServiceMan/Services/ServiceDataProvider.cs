using ServiceMan.Model;
using ServiceMan.ViewModel;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace ServiceMan.Services
{
    class ServiceDataProvider : ServiceProviderBase
    {
        public override ServiceDataSource GetData()
        {
            ulong count = NativeAPI.ServiceCount();

            var data = new ServiceProcess[count];
            NativeAPI.ServiceList(data, count);

            var dataSource = new ServiceDataSource();

            foreach (var svcInfo in data)
            {
                string imagePath;
                string group = ParseImagePath(svcInfo.ImagePath, out imagePath);


                dataSource.Add(new ServiceViewModel(
                    pID: svcInfo.ProcessId == 0 ? string.Empty : svcInfo.ProcessId.ToString(),
                    name: svcInfo.Name,
                    description: svcInfo.Description,
                    imagePath: imagePath,
                    group: group,
                    status: svcInfo.Status
                    ));

            }

            return dataSource;

        }
    }
}
