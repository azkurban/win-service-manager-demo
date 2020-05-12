using ServiceMan.Model;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ServiceMan.ViewModel
{
    internal class ServiceViewModel
    {
        public ServiceViewModel(string pID, string name, string description, string status, string group, string imagePath)
        {
            PID = pID;
            Name = name;
            Description = description;
            Status = status;
            Group = group;
            ImagePath = imagePath;
        }

        public string PID { get; private set; }
        public string Name { get; private set; }
        public string Description { get; private set; }
        public string Status { get; private set; }
        public string Group { get; private set; }
        public string ImagePath { get; private set; }

        public bool IsRunning => Status.Equals(ServiceStatus.Running.ToString());
    };
}
