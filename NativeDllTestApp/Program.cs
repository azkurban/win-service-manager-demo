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
                var count = 5;
                var services = new ServiceProcess[count];
                NativeAPI.GetServiceList(services, count);

                
                foreach(var s in services)
                {
                    Console.WriteLine($"Service '{s.Name}':");
                    Console.WriteLine("----------------------------");
                    Console.WriteLine($" {s.PID}");
                    Console.WriteLine($" {s.Name}");
                    Console.WriteLine($" {s.Description}");
                    Console.WriteLine($" {s.Status}");
                    Console.WriteLine($" {s.ImagePath}");
                    Console.WriteLine($" {s.GroupName}");
                    Console.WriteLine("----------------------------");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            Console.ReadKey();

        }
    }
}
