using ServiceMan.Model;
using ServiceMan.ViewModel;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows;

namespace ServiceMan.Services
{
    class ServiceDataProvider : ServiceProviderBase
    {
        private ServiceListViewModel _listViewModel;

        public override ServiceListViewModel GetServiceListViewModel()
        {
            uint count = NativeAPI.ServiceCount();

            var services = new ServiceProcess[count];
            NativeAPI.ServiceList(services, count);

            _listViewModel = new ServiceListViewModel();

            foreach (var svcInfo in services
                .Where(s => !(s.ImagePath.StartsWith(@"\SystemRoot\System32\drivers\") || string.IsNullOrEmpty(s.ImagePath))))
            {
                string imagePath;
                string group = ParseImagePath(svcInfo.ImagePath, out imagePath);

                _listViewModel.Add(new ServiceViewModel(
                    pID: svcInfo.ProcessId == 0 ? string.Empty : svcInfo.ProcessId.ToString(),
                    name: svcInfo.Name,
                    description: svcInfo.Description,
                    imagePath: imagePath,
                    group: group,
                    status: svcInfo.Status
                    ));

            }

            return _listViewModel;

        }
        public override void StopService(string serviceName, out ServiceViewModel viewModel)
        {
            viewModel = _listViewModel.Single(s => s.Name == serviceName);

            var svcState = new ServiceControlState();
            int apiCallResult = NativeAPI.StopWinService(serviceName, ref svcState);

            if (apiCallResult == NativeApiCallResult.SUCCEED)
            {
                viewModel.Update(svcState);
            }
            else
            {
                string message = svcState.Message;
                string caption = $"Fail to stop service: '{serviceName}'";

                // Show message box
                ShowMessage(caption, message);
            }
        }

        public override void StartService(string serviceName, out ServiceViewModel viewModel)
        {
            viewModel = _listViewModel.Single(s => s.Name == serviceName);

            var svcState = new ServiceControlState();
            int apiCallResult = NativeAPI.StartWinService(serviceName, ref svcState);

            if (apiCallResult == NativeApiCallResult.SUCCEED)
            {
                viewModel.Update(svcState);
            }
            else
            {
                string message = svcState.Message;
                string caption = $"Fail to Start service: '{serviceName}'";

                // Show message box
                ShowMessage(caption, message);
            }
        }

        public override void RestartService(string serviceName, out ServiceViewModel viewModel)
        {
            viewModel = _listViewModel.Single(s => s.Name == serviceName);

            var svcState = new ServiceControlState();
            int apiCallResult = NativeAPI.RestartWinService(serviceName, ref svcState);

            if (apiCallResult == NativeApiCallResult.SUCCEED)
            {
                viewModel.Update(svcState);
            }
            else
            {
                string message = svcState.Message;
                string caption = $"Fail to Restart service: '{serviceName}'";

                // Show message box
                ShowMessage(caption, message);
            }
        }

        private void ShowMessage(string caption, string message)
        {
            // Configure message box
            MessageBoxButton buttons = MessageBoxButton.OK;
            MessageBoxImage icon = MessageBoxImage.Error;

            // Show message box
            MessageBoxResult result = MessageBox.Show(message, caption, buttons, icon);
        }


    }
}
