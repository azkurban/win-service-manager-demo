using ServiceMan.Services;
using ServiceMan.ViewModel;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ServiceMan
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Stopwatch _watch;
        private BackgroundWorker _backgroundWorker;
        private ServiceListViewModel _dataContext;
        private CollectionViewSource _serviceDataSourceViewSource;
        private ServiceHelper _serviceHelper;

        private ServiceViewModel _currentServiceToManage;

        private DoWorkEventHandler _doWorkEventHandler;

        public MainWindow()
        {
            InitializeComponent();
            _watch = new Stopwatch();
            _backgroundWorker = new BackgroundWorker();
            _serviceHelper = new ServiceHelper();
        }


        private void Close_OnClick(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            beginLoadServiceList();
        }

        private void Start_Click(object sender, RoutedEventArgs e)
        {

        }

        private void ServiceListView_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {

        }

        private void mnuItem_Start_Click(object sender, RoutedEventArgs e)
        {
            if (serviceListView.SelectedIndex > -1)
            {
                var service = (ServiceViewModel)serviceListView.SelectedItem; // casting the list view
                //MessageBox.Show($"Request to Start service:'{service.Name}'", "Service Item Context Menu Test", MessageBoxButton.OK, MessageBoxImage.Information);
                beginStartService(service);

            }
        }

        private void mnuItem_Stop_Click(object sender, RoutedEventArgs e)
        {
            if (serviceListView.SelectedIndex > -1)
            {
                var service = (ServiceViewModel)serviceListView.SelectedItem; // casting the list view
                //MessageBox.Show($"Request to Stop service:'{service.Name}'", "Service Item Context Menu Test", MessageBoxButton.OK, MessageBoxImage.Information);
                beginStopService(service);
            }
        }

        private void mnuItem_Restart_Click(object sender, RoutedEventArgs e)
        {
            if (serviceListView.SelectedIndex > -1)
            {
                var service = (ServiceViewModel)serviceListView.SelectedItem; // casting the list view
                //MessageBox.Show($"Request to Restart service:'{service.Name}'", "Service Item Context Menu Test", MessageBoxButton.OK, MessageBoxImage.Information);
                beginRestartService(service);
            }
        }

        private void serviceListView_ContextMenuOpening(object sender, ContextMenuEventArgs e)
        {
            if (serviceListView.SelectedIndex > -1)
            {
                var service = (ServiceViewModel)serviceListView.SelectedItem; // casting the list view

                if (service.IsRunning)
                {
                    mnuItem_Start.IsEnabled = false;
                    mnuItem_Stop.IsEnabled = true;
                    mnuItem_Restart.IsEnabled = true;
                }
                else
                {
                    mnuItem_Start.IsEnabled = true;
                    mnuItem_Stop.IsEnabled = false;
                    mnuItem_Restart.IsEnabled = false;
                }
            }
        }

        #region Asyncronous Tasks
        private void beginLoadServiceList()
        {
            if (_backgroundWorker.IsBusy)
            {
                _serviceHelper.ShowErrorMessage("Load data Error", "Background Worker is busy.");
                return;
            }

            _doWorkEventHandler += (_, args) =>
            {
                _dataContext = _serviceHelper.GetServiceListViewModel();
            };

            _backgroundWorker.DoWork += _doWorkEventHandler;
            _backgroundWorker.RunWorkerCompleted += EndLoadServiceList;

            #region Before loading service data
            _watch.Start();
            ServiceProgress.Visibility = Visibility.Visible;
            ServiceProgress.IsIndeterminate = true;
            #endregion

            _serviceDataSourceViewSource =
                ((System.Windows.Data.CollectionViewSource)(this.FindResource("serviceDataSourceViewSource")));

            StocksStatus.Text = "Loading data, please wait...";
            _backgroundWorker.RunWorkerAsync();
        }

        void EndLoadServiceList(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error == null)
            {
                // Load data by setting the CollectionViewSource.Source property:

                _serviceDataSourceViewSource.Source = _dataContext;
                serviceListView.Visibility = Visibility.Visible;
                StocksStatus.Text = $"Loaded data in {_watch.ElapsedMilliseconds}ms";
                ServiceProgress.Visibility = Visibility.Hidden;
            }
            else
            {
                _serviceHelper.ShowErrorMessage("Load data Error", e.Error.Message);
            }

            _backgroundWorker.DoWork -= _doWorkEventHandler;
            _backgroundWorker.RunWorkerCompleted -= EndLoadServiceList;
        }

        private void beginStopService(ServiceViewModel service)
        {
            if (_backgroundWorker.IsBusy)
            {
                _serviceHelper.ShowErrorMessage("Stop Service Error", "Background Worker is busy.");
                return;
            }

            _doWorkEventHandler = (_, args) =>
            {
                _serviceHelper.StopService(service);
            };

            _backgroundWorker.DoWork += _doWorkEventHandler;
            _backgroundWorker.RunWorkerCompleted += EndStopService;

            #region Before stopping service

            _watch.Reset();
            _watch.Start();
            ServiceProgress.Visibility = Visibility.Visible;
            ServiceProgress.IsIndeterminate = true;
            #endregion

            StocksStatus.Text = "Trying to stop service, please wait...";
            _backgroundWorker.RunWorkerAsync();
        }

        void EndStopService(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error == null)
            {
                serviceListView.Visibility = Visibility.Visible;
                StocksStatus.Text = $"Stopped service in {_watch.ElapsedMilliseconds}ms";
                ServiceProgress.Visibility = Visibility.Hidden;
            }
            else
            {
                _serviceHelper.ShowErrorMessage("Stop Service Error", e.Error.Message);
            }

            _backgroundWorker.DoWork -= _doWorkEventHandler;
            _backgroundWorker.RunWorkerCompleted -= EndStopService;
        }

        private void beginStartService(ServiceViewModel service)
        {
            if (_backgroundWorker.IsBusy)
            {
                _serviceHelper.ShowErrorMessage("Start Service Error", "Background Worker is busy.");
                return;
            }

            _doWorkEventHandler = (_, args) =>
            {
                _serviceHelper.StartService(service);
            };

            _backgroundWorker.DoWork += _doWorkEventHandler;
            _backgroundWorker.RunWorkerCompleted += EndStartService;

            #region Before starting service
            _watch.Reset();
            _watch.Start();
            ServiceProgress.Visibility = Visibility.Visible;
            ServiceProgress.IsIndeterminate = true;
            #endregion

            StocksStatus.Text = "Trying to start service, please wait...";
            _backgroundWorker.RunWorkerAsync();
        }

        void EndStartService(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error == null)
            {
                serviceListView.Visibility = Visibility.Visible;
                StocksStatus.Text = $"Started service in {_watch.ElapsedMilliseconds}ms";
                ServiceProgress.Visibility = Visibility.Hidden;
            }
            else
            {
                _serviceHelper.ShowErrorMessage("Start Service Error", e.Error.Message);
            }

            _backgroundWorker.DoWork -= _doWorkEventHandler;
            _backgroundWorker.RunWorkerCompleted -= EndStartService;
        }

        private void beginRestartService(ServiceViewModel service)
        {
            if (_backgroundWorker.IsBusy)
            {
                _serviceHelper.ShowErrorMessage("Start Service Error", "Background Worker is busy.");
                return;
            }

            _doWorkEventHandler = (_, args) =>
             {
                 _serviceHelper.RestartService(service);
             };

            _backgroundWorker.DoWork += _doWorkEventHandler;
            _backgroundWorker.RunWorkerCompleted += EndRestartService;

            #region Before reseting service
            _watch.Reset();
            _watch.Start();
            ServiceProgress.Visibility = Visibility.Visible;
            ServiceProgress.IsIndeterminate = true;
            #endregion

            StocksStatus.Text = "Trying to restart service, please wait...";
            _backgroundWorker.RunWorkerAsync();
        }

        void EndRestartService(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error == null)
            {
                serviceListView.Visibility = Visibility.Visible;
                StocksStatus.Text = $"Restarted service in {_watch.ElapsedMilliseconds}ms";
                ServiceProgress.Visibility = Visibility.Hidden;
            }
            else
            {
                _serviceHelper.ShowErrorMessage("Restart Service Error", e.Error.Message);
            }

            _backgroundWorker.DoWork -= _doWorkEventHandler;
            _backgroundWorker.RunWorkerCompleted -= EndRestartService;
        }



        #endregion
    }
}
