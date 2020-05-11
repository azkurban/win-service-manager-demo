using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ServiceMan.ViewModel
{
    internal class ServiceModel
    {
        public int PID { get; set; }
        public string Name { get; set; }
        public string Description { get; set; }
        public string Status { get; set; }
        public string Group { get; set; }
        public string ImagePath { get; set; }
    };
}
