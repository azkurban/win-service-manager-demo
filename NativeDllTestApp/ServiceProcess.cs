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

        //[MarshalAs(UnmanagedType.LPWStr)]
        public string Name;

        //[MarshalAs(UnmanagedType.LPWStr)]
        public string Description;

        //[MarshalAs(UnmanagedType.LPWStr)]
        public string ImagePath;

        //[MarshalAs(UnmanagedType.LPWStr)]
        public string Status;

        //[MarshalAs(UnmanagedType.LPWStr)]
        public string GroupName;
    };

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    public struct MyStruct
    {
        public int IntValue;
        public string StringValue;
        public string StringValue2;
    }
}
