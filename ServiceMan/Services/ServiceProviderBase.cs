using ServiceMan.ViewModel;
using System;

namespace ServiceMan.Services
{
    internal abstract class ServiceProviderBase : IServiceDataProvider
    {
        public abstract ServiceListViewModel GetServiceListViewModel();
        public abstract void RestartService(string serviceName, out ServiceViewModel viewModel);
        public abstract void StartService(string serviceName, out ServiceViewModel viewModel);
        public abstract void StopService(string serviceName, out ServiceViewModel viewModel);

        protected virtual string ParseImagePath(string combinedPath, out string imagePath)
        {
            imagePath = string.Empty;

            if (string.IsNullOrEmpty(combinedPath?.Trim())) return string.Empty;

            string[] pathParts = combinedPath.Split(new[] { "-k" }, StringSplitOptions.RemoveEmptyEntries);

            imagePath = pathParts[0].Trim();
            var group = string.Empty;

            if (pathParts.Length > 1)
            {
                var remainingParts = pathParts[1].Split(new[] { "-p" }, StringSplitOptions.RemoveEmptyEntries);
                group = remainingParts[0].Trim();
            }

            return group;
        }

    }


}