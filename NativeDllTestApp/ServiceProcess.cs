using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace NativeDllTestApp
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct ServiceProcess
    {
        public int PID;
        public string Name;
        public string Description;
        public string GroupName;
        public string ImagePath;
        public string Status;
    };
}
