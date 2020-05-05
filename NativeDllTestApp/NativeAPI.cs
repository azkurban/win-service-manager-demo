using System.Runtime.InteropServices;

namespace NativeDllTestApp
{
    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/aa288468(VS.71).aspx
    /// http://www.mono-project.com/docs/advanced/pinvoke/
    /// </summary>
    internal static class NativeAPI
    {
        private const string nativeDll = "svcman.dll";

        [DllImport(nativeDll/*, CallingConvention = CallingConvention.Cdecl*/)]
        internal static extern void GetServiceList([In, Out] ServiceProcess[] serviceProcesses, int size);
    }
}
