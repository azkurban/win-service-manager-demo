using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace NativeDllTestApp
{
    [StructLayout(LayoutKind.Sequential)]
    struct ManagedAmbiguousStructSample
    {
        //[MarshalAs(UnmanagedType.LPStr)] 
        public string AnsiString;

        [MarshalAs(UnmanagedType.LPWStr)]
        public string WideString;

        //[MarshalAs(UnmanagedType.Bool)]
        public bool Win32Boolean;

        [MarshalAs(UnmanagedType.I1)]
        public bool CStyleBoolean;

        public ushort ShortInteger;
    };

}
