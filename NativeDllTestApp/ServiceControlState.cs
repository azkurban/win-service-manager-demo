﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace NativeDllTestApp
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
    struct ServiceControlState
    {
        public uint CurrentState;
        public uint Win32ExitCode;
        public uint CheckPoint;
        public uint WaitHint;
        public uint ProcessId;
        public uint ErrorCode;
        public string Message;
    }
}
