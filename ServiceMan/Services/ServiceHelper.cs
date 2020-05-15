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
    class ServiceHelper : ServiceProviderBase
    {
        private ServiceListViewModel _listViewModel;

        public override ServiceListViewModel GetServiceListViewModel()
        {
            uint count = NativeAPI.ServiceCount();

            var services = new ServiceProcess[count];
            NativeAPI.ServiceList(services, count);

            _listViewModel = new ServiceListViewModel();

            foreach (var svcInfo in services
                .Where(s => !(string.IsNullOrEmpty(s.ImagePath) ||
                            s.ImagePath.Contains(@"SystemRoot\")||
                            s.ImagePath.StartsWith(@"system32\")))
                .OrderBy(s => s.Name))
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
        public override void StopService(ServiceViewModel viewModel)
        {
            var svcState = new ServiceControlState();
            int apiCallResult = NativeAPI.StopWinService(viewModel.Name, ref svcState);

            if (apiCallResult == NativeApiCallResult.SUCCEED)
            {
                viewModel.Update(svcState);
            }
            else
            {
                string message = svcState.Message;
                string caption = $"Fail to stop service: '{viewModel.Name}'";

                // Show message box
                ShowErrorMessage(caption, message);
            }
        }

        public override void StartService(ServiceViewModel viewModel)
        {
            var svcState = new ServiceControlState();
            int apiCallResult = NativeAPI.StartWinService(viewModel.Name, ref svcState);

            if (apiCallResult == NativeApiCallResult.SUCCEED)
            {
                viewModel.Update(svcState);
            }
            else
            {
                string message = svcState.Message;
                string caption = $"Fail to Start service: '{viewModel.Name}'";

                // Show message box
                ShowErrorMessage(caption, message);
            }
        }

        public override void RestartService(ServiceViewModel viewModel)
        {
            //viewModel = _listViewModel.Single(s => s.Name == service.Name);

            var svcState = new ServiceControlState();
            int apiCallResult = NativeAPI.RestartWinService(viewModel.Name, ref svcState);

            if (apiCallResult == NativeApiCallResult.SUCCEED)
            {
                viewModel.Update(svcState);
            }
            else
            {
                string message = svcState.Message;
                string caption = $"Fail to Restart service: '{viewModel.Name}'";

                // Show message box
                ShowErrorMessage(caption, message);
            }
        }

        public void ShowErrorMessage(string caption, string message)
        {
            // Configure message box
            MessageBoxButton buttons = MessageBoxButton.OK;
            MessageBoxImage icon = MessageBoxImage.Error;

            // Show message box
            MessageBoxResult result = MessageBox.Show(message, caption, buttons, icon);
        }


    }
}
