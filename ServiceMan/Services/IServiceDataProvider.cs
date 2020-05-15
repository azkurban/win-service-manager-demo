using ServiceMan.ViewModel;

namespace ServiceMan.Services
{
    interface IServiceDataProvider
    {
        ServiceListViewModel GetServiceListViewModel();
        void StopService(ServiceViewModel viewModel);
        void StartService(ServiceViewModel viewModel);
        void RestartService(ServiceViewModel viewModel);

    }
}