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
        public int ProcessId;

        //[MarshalAs(UnmanagedType.LPWStr)]
        public string Name;
        public string Description;
        public string ImagePath;
        public string Status;
        //public string GroupName;
    };

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct MyStruct
    {
        public int IntValue;
        public string StringValue;
        public string StringValue2;
    }
}
