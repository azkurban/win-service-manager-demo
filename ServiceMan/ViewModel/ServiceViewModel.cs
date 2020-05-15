using ServiceMan.Model;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace ServiceMan.ViewModel
{
    internal class ServiceViewModel : INotifyPropertyChanged
    {
        private Dictionary<ServiceStatus, string> _statusMap;
        private string _pID;
        private string _status;

        public ServiceViewModel(string pID, string name, string description, string status, string group, string imagePath)
        {
            PID = pID;
            Name = name;
            Description = description;
            Status = status;
            Group = group;
            ImagePath = imagePath;

            _statusMap = new Dictionary<ServiceStatus, string>
            {
                {ServiceStatus.SERVICE_STOPPED,          "Stopped" },
                {ServiceStatus.SERVICE_RUNNING,          "Running" },
                {ServiceStatus.SERVICE_PAUSED,           "Paused" },
                {ServiceStatus.SERVICE_PAUSE_PENDING,    "Pausing..." },
                {ServiceStatus.SERVICE_START_PENDING,    "Starting ..." },
                {ServiceStatus.SERVICE_STOP_PENDING,     "Stopping ..." },
                {ServiceStatus.SERVICE_CONTINUE_PENDING, "Resuming..." },
            };
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public string PID
        {
            get => _pID;
            set
            {
                if (value != _pID)
                {
                    _pID = value;
                    this.OnPropertyChanged("PID");
                }
            }
        }

        public string Status
        {
            get => _status;
            set
            {
                if (value != _status)
                {
                    _status = value;
                    this.OnPropertyChanged("Status");
                }
            }
        }

        public string Name { get; private set; }
        public string Description { get; private set; }
        public string Group { get; private set; }
        public string ImagePath { get; private set; }

        public bool IsRunning => Status.Equals(_statusMap[ServiceStatus.SERVICE_RUNNING]);
        //public bool IsStopped => Status.Equals(_statusMap[ServiceStatus.SERVICE_STOPPED]);

        public void Update(ServiceControlState newState)
        {
            PID = newState.ProcessId == 0 ? "" : newState.ProcessId.ToString();
            Status = _statusMap[(ServiceStatus)newState.CurrentState];
        }

        protected virtual void OnPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

    };
}
