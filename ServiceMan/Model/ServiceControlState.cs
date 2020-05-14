using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ServiceMan.Model
{
    [StructLayout(LayoutKind.Sequential, Pack = 1, CharSet = CharSet.Unicode)]
    public struct ServiceControlState
    {
        public uint CurrentState;
        public uint Win32ExitCode;
        public uint CheckPoint;
        public uint WaitHint;
        public uint ProcessId;
        public uint ErrorCode;
        public string ServiceName;
        public string Message;
    }
}
