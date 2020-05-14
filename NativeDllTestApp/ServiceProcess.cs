﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace NativeDllTestApp
{
    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Unicode)]
    public struct ServiceProcess
    {
        public uint ProcessId { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public string ImagePath { get; set; }
        public string Status { get; set; }
    };

    //[StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    //public struct MyStruct
    //{
    //    public int IntValue;
    //    public string StringValue;
    //    public string StringValue2;
    //}
}
