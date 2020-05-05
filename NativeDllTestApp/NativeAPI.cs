using System.Runtime.InteropServices;

namespace NativeDllTestApp
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/aa288468(VS.71).aspx
    /// http://www.mono-project.com/docs/advanced/pinvoke/
    /// </summary>
    internal static class NativeAPI
    {
        private const string DLL_LOCATION = "svcman.dll";

        [DllImport(DLL_LOCATION/*, CallingConvention = CallingConvention.Cdecl*/)]
        internal static extern void GetServiceList(out ServiceProcess[] serviceProcesses);
        
        //[DllImport(DLL_LOCATION/*, CallingConvention = CallingConvention.Cdecl*/)]
        //internal static extern int GetServiceListSize();

        [DllImport(DLL_LOCATION, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern void SetStringArray([MarshalAs(UnmanagedType.SafeArray)] string[] array);

        [DllImport(DLL_LOCATION, CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern void GetStringArray([MarshalAs(UnmanagedType.SafeArray)] out string[] array);

    }
}
