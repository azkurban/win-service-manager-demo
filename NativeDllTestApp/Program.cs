using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NativeDllTestApp
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {

                //ServiceProcess[] services = null;
                ////NativeAPI.GetServiceList(out services);
                ////var count = NativeAPI.GetServiceListSize();
                //Console.WriteLine($"Service Count: { services?.Length ?? -1 }");

                //string[] array = new string[] { "one", "two", "three", "four", "Раз", "два", "три" };
                //NativeAPI.SetStringArray(array);
                //Console.WriteLine($"The following strings were passed: {String.Join(", ", array)}");

                //string[] results = null;
                //NativeAPI.GetStringArray(out results);
                //Console.WriteLine($"The following strings were received: {String.Join(", ", results)}");

                //MyStruct[] myStructs = new MyStruct[7];
                //int size = NativeAPI.SendArray(myStructs, 7);
                //Console.WriteLine($"myStruct sould bo of size: {size}");

                //if (myStructs == null)
                //    throw new NullReferenceException("myStructs IS NULL!");

                //foreach(var myStruct in myStructs)
                //{
                //    Console.WriteLine($"myStruct.IntValue = {myStruct.IntValue}");
                //    Console.WriteLine($"myStruct.StringValue = {myStruct.StringValue}");
                //    Console.WriteLine($"myStruct.StringValue = {myStruct.StringValue2}");
                //}

                ulong svcCount = NativeAPI.ServiceCount();

                var services = new ServiceProcess[svcCount];
                Console.WriteLine($"ServiceProcess should be of size: {svcCount}");
                Console.WriteLine();

                NativeAPI.ServiceList(services, svcCount);

                int count = (int)(svcCount > 10 ? svcCount : svcCount);

                for (int i = 0; i < count; i++)
                {
                    var svc = services[i];

                    Console.WriteLine($"ServiceProcess.ProcessId = {svc.ProcessId}");
                    Console.WriteLine($"ServiceProcess.Name = {svc.Name}");
                    Console.WriteLine($"ServiceProcess.Description = {svc.Description}");
                    Console.WriteLine($"ServiceProcess.ImagePath = {svc.ImagePath}");
                    Console.WriteLine($"ServiceProcess.Status = {svc.Status}");
                    //Console.WriteLine($"ServiceProcess.GroupName = {svc.GroupName}");
                    Console.WriteLine();
                }

                //string[] wstrings = null;
                //NativeAPI.GetWStringArray(out wstrings);
                //Console.WriteLine($"The following wide strings were received: {String.Join(", ", wstrings)}");

                /**
                foreach (var s in services)
                {
                    //Console.WriteLine($"Service '{s.Name}':");
                    Console.WriteLine("----------------------------");
                    //Console.WriteLine($" {s.PID}");
                    //Console.WriteLine($" {s.Name}");
                    //Console.WriteLine($" {s.Description}");
                    //Console.WriteLine($" {s.Status}");
                    //Console.WriteLine($" {s.ImagePath}");
                    //Console.WriteLine($" {s.GroupName}");
                    Console.WriteLine("----------------------------");
                }
                **/
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            Console.ReadKey();

        }
    }
}
