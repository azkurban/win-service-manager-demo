using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ServiceMan.Model
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    internal struct ServiceProcess
    {
        public int ProcessId;
        public string Name;
        public string Description;
        public string ImagePath;
        public string Status;
    };
}
