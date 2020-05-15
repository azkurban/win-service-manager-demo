using ServiceMan.ViewModel;

namespace ServiceMan.Services
{
    interface IServiceDataProvider
    {
        ServiceListViewModel GetServiceListViewModel();
        void StopService(string serviceName, out ServiceViewModel viewModel);
        void StartService(string serviceName, out ServiceViewModel viewModel);
        void RestartService(string serviceName, out ServiceViewModel viewModel);

    }
}