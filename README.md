# win-service-manager-demo
Demo hybrid WPF/C++ app using PInvoke for Windows Service Control management.

## Solution:

### WPF app project (C#, .NET 3.5): **ServiceMan**

This application can work under all Windows 7+ (x86/x64) 0S versions with .NET 3.5 Framework installed.  

### Console Test App (C#, .NET 4.0): **NativeDllTestApp**

.NET 4.0 have have been chosen for the test application due to fact that .NET 3.5 does not supported mixed debugging (from managed to unmanaged code).

### Unmanaged DLL project (C++): **svcman**

## Resources

### Interop (Platform Invoke is used)

#### Books

[.NET 2.0 Interoperability Recipes: A Problem-Solution Approach](https://books.google.ru/books?id=ZDin4axsYoEC&pg=PA597&lpg=PA597&dq=unmanaged+c%2B%2B+arrays+of+structures&source=bl&ots=x7adH6Rw00&sig=ACfU3U3M-JFofoHVS7Oo_0YDCAvCodz_lg&hl=en&sa=X&ved=2ahUKEwiChp2q6prpAhUBuIsKHfohCKsQ6AEwBXoECAoQAQ#v=onepage&q=unmanaged%20c%2B%2B%20arrays%20of%20structures&f=false) By Bruce Bukovics

#### Blogs

[BadImageFormatException, x86 i x64](https://www.codeproject.com/articles/383138/badimageformatexception-x86-i-x64)

Please, refere to this article to learn how to proper build (or even configure without rebuilding) the app for the both x86 and x64 Windows OS versions.

### C++ Unmanaged DLL

#### Microsof Documentation:

[Starting a Service](https://docs.microsoft.com/en-us/windows/win32/services/starting-a-service)

[Stopping a Service](https://docs.microsoft.com/en-us/windows/win32/services/stopping-a-service)

[NotifyServiceStatusChangeA function](https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-notifyservicestatuschangea?redirectedfrom=MSDN)

[Asynchronous Procedure Calls](https://docs.microsoft.com/en-us/windows/win32/sync/asynchronous-procedure-calls)

##### GitHub:

Using NotifyServiceStatusChange for monitoring services:

[microsoft/Windows-classic-samples/Samples/Win7Samples/winbase/monitorservices/monsvc/MonSvc.cxx](https://github.com/microsoft/Windows-classic-samples/blob/master/Samples/Win7Samples/winbase/monitorservices/monsvc/MonSvc.cxx)

### WPF

#### Microsoft Documentation

[BackgroundWorker Class](https://docs.microsoft.com/en-us/dotnet/api/system.componentmodel.backgroundworker?view=netframework-3.5)

##### GitHub

[microsoftarchive/msdn-code-gallery-community-s-z/WPF Listview Context Menu/C#-WPF Listview Context Menu/C#/WpfListViewContextMenu/MainWindow.xaml.cs](https://github.com/microsoftarchive/msdn-code-gallery-community-s-z/blob/master/WPF%20Listview%20Context%20Menu/%5BC%23%5D-WPF%20Listview%20Context%20Menu/C%23/WpfListViewContextMenu/MainWindow.xaml.cs)

[microsoftarchive/msdn-code-gallery-community-s-z/WPF Listview Context Menu/C#-WPF Listview Context Menu/C#/WpfListViewContextMenu/MainWindow.xaml](https://github.com/microsoftarchive/msdn-code-gallery-community-s-z/blob/master/WPF%20Listview%20Context%20Menu/%5BC%23%5D-WPF%20Listview%20Context%20Menu/C%23/WpfListViewContextMenu/MainWindow.xaml)

#### Tutorials

[The WPF Context Menu](https://github.com/microsoftarchive/msdn-code-gallery-community-s-z/blob/master/WPF%20Listview%20Context%20Menu/%5BC%23%5D-WPF%20Listview%20Context%20Menu/C%23/WpfListViewContextMenu/MainWindow.xaml)

[Working with Background Workers in C#](https://www.codeguru.com/columns/dotnet/working-with-background-workers-in-c.html)


