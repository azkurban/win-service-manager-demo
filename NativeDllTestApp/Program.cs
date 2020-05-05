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

                ServiceProcess[] services = null;
                //NativeAPI.GetServiceList(out services);
                //var count = NativeAPI.GetServiceListSize();
                Console.WriteLine($"Service Count: { services?.Length ?? -1 }");

                string[] array = new string[4] { "one", "two", "three", "four" };
                NativeAPI.SetStringArray(array);
                Console.WriteLine($"The following strings were passed: {String.Join(", ", array)}");

                string[] results = null;
                NativeAPI.GetStringArray(out results);
                Console.WriteLine($"The following strings were received: {String.Join(", ", results)}");


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
