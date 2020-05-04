using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace NativeDllTestApp
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                var result = UnsafeNativeMethods.GetServiceList();
                Console.WriteLine(result);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            Console.ReadKey();

        }
    }

    internal static class Import
    {
        public const string lib = "svcman.dll";
    }

    /// <summary>
    /// http://msdn.microsoft.com/en-us/library/aa288468(VS.71).aspx
    /// http://www.mono-project.com/docs/advanced/pinvoke/
    /// </summary>
    internal static class UnsafeNativeMethods
    {
        [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int GetServiceList();
    }

}
