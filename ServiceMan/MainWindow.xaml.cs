using ServiceMan.Services;
using ServiceMan.ViewModel;
using System;
using System.Collections.Generic;
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
        public MainWindow()
        {
            InitializeComponent();

            ServiceDataSource dataContext = (new ServiceDataProvider()).GetMockData();
            ServiceListView.DataContext = dataContext;
        }

        private void Close_OnClick(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {

            System.Windows.Data.CollectionViewSource serviceDataSourceViewSource = 
                ((System.Windows.Data.CollectionViewSource)(this.FindResource("serviceDataSourceViewSource")));
            // Load data by setting the CollectionViewSource.Source property:

            //ServiceDataSource dataContext = (new ServiceDataProvider()).GetMockData();
            ServiceDataSource dataContext = (new ServiceDataProvider()).GetData();
            serviceDataSourceViewSource.Source = dataContext;
            //ServiceListView.DataContext = dataContext;

        }
    }
}
