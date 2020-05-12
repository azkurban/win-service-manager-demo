using ServiceMan.ViewModel;

namespace ServiceMan.Services
{
    interface IServiceDataProvider
    {
        ServiceDataSource GetData();
    }
}