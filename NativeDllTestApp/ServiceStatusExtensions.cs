using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ServiceMan.Model;
using ServiceMan.Services;

namespace NativeDllTestApp
{
    static class ServiceStatusExtensions
    {
        public static bool IsRunning(this ServiceProcess service) => service.Status.Equals(ServiceStatus.SERVICE_RUNNING.ToString());
    }
}
