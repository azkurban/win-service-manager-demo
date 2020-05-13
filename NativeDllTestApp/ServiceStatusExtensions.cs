using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NativeDllTestApp
{
    static class ServiceStatusExtensions
    {
        public static bool IsRunning(this ServiceProcess service) => service.Status.Equals(ServiceStatus.Running.ToString());
    }
}
