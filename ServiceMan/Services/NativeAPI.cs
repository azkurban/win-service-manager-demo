using ServiceMan.Model;
using System.Runtime.InteropServices;

namespace ServiceMan.Services
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/aa288468(VS.71).aspx
    /// http://www.mono-project.com/docs/advanced/pinvoke/
    /// </summary>
    public static class NativeAPI
    {
        private const string DLL_LOCATION = @"x64\svcman.dll";

        [DllImport(DLL_LOCATION, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint ServiceCount();

        [DllImport(DLL_LOCATION, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ServiceList([In, Out] ServiceProcess[] services, uint count);

        [DllImport(DLL_LOCATION, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int StartWinService([MarshalAs(UnmanagedType.BStr)] string serviceName, ref ServiceControlState svcState);

        [DllImport(DLL_LOCATION, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int StopWinService([MarshalAs(UnmanagedType.BStr)] string serviceName, [In, Out] ref ServiceControlState svcState);


        [DllImport(DLL_LOCATION, CharSet = CharSet.Unicode, CallingConvention = CallingConvention.StdCall)]
        public static extern int RestartWinService([MarshalAs(UnmanagedType.BStr)] string serviceName, ref ServiceControlState svcState);


    }
}
