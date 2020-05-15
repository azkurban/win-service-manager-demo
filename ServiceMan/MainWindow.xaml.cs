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

        public MainWindow()
        {
            InitializeComponent();
            _watch = new Stopwatch();
            _backgroundWorker = new BackgroundWorker();

            _backgroundWorker.DoWork += (_, args) =>
            {
                //ServiceDataSource dataContext = (new ServiceDataProvider()).GetMockData();
                _dataContext = (new ServiceDataProvider()).GetServiceListViewModel();
            };

            _backgroundWorker.RunWorkerCompleted += BackgroundWorkerRunWorkerCompleted;
        }

        private void Close_OnClick(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            #region Before loading service data
            _watch.Start();
            ServiceProgress.Visibility = Visibility.Visible;
            ServiceProgress.IsIndeterminate = true;
            #endregion

            _serviceDataSourceViewSource =
                ((System.Windows.Data.CollectionViewSource)(this.FindResource("serviceDataSourceViewSource")));

            if (!_backgroundWorker.IsBusy)
            {
                StocksStatus.Text = "Loading data, please wait...";
                _backgroundWorker.RunWorkerAsync();
            }
        }

        void BackgroundWorkerRunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
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
                // Configure message box
                string message = e.Error.Message;
                string caption = "Load data Error";
                MessageBoxButton buttons = MessageBoxButton.OK;
                MessageBoxImage icon = MessageBoxImage.Error;

                // Show message box
                MessageBoxResult result = MessageBox.Show(message, caption, buttons, icon);
            }
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
                MessageBox.Show($"Request to Start service:'{service.Name}'", "Service Item Context Menu Test", MessageBoxButton.OK, MessageBoxImage.Information);

            }
        }

        private void mnuItem_Stop_Click(object sender, RoutedEventArgs e)
        {
            if (serviceListView.SelectedIndex > -1)
            {
                var service = (ServiceViewModel)serviceListView.SelectedItem; // casting the list view
                MessageBox.Show($"Request to Stop service:'{service.Name}'", "Service Item Context Menu Test", MessageBoxButton.OK, MessageBoxImage.Information);

            }
        }

        private void mnuItem_Restart_Click(object sender, RoutedEventArgs e)
        {
            if (serviceListView.SelectedIndex > -1)
            {
                var service = (ServiceViewModel)serviceListView.SelectedItem; // casting the list view
                MessageBox.Show($"Request to Restart service:'{service.Name}'", "Service Item Context Menu Test", MessageBoxButton.OK, MessageBoxImage.Information);

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
    }
}
