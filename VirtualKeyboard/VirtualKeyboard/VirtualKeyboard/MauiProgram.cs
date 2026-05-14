using CommunityToolkit.Maui;
using MetroLog.MicrosoftExtensions;
using MetroLog.Operators;
using Microsoft.Extensions.Logging;
using System.Runtime.InteropServices;
using VirtualKeyboard.Pages;
using VirtualKeyboard.Services;
using VirtualKeyboard.ViewModels;


#if WINDOWS
using Microsoft.Maui.LifecycleEvents;
using Microsoft.UI;
using Microsoft.UI.Windowing;
using Windows.Graphics;
#endif

namespace VirtualKeyboard
{
    public static class MauiProgram
    {
        public static MauiApp CreateMauiApp()
        {
            var builder = MauiApp.CreateBuilder();
            builder
                .UseMauiApp<App>()
                .UseMauiCommunityToolkit()
                .ConfigureFonts(fonts =>
                {
                    fonts.AddFont("OpenSans-Regular.ttf", "OpenSansRegular");
                    fonts.AddFont("OpenSans-Semibold.ttf", "OpenSansSemibold");
                    fonts.AddFont("MaterialIcons-Regular.ttf", "MaterialIcon");
                })
                .RegisterServices()
                .RegisterViewModels()
                .RegisterPages();

#if WINDOWS
            builder.ConfigureLifecycleEvents(events =>
            {
                events.AddWindows(wndLifeCycleBuilder =>
                {
                    wndLifeCycleBuilder.OnWindowCreated(window =>
                    {
                        window.ExtendsContentIntoTitleBar = false; /*This is important to prevent your app content extends into the title bar area.*/
                        IntPtr nativeWindowHandle = WinRT.Interop.WindowNative.GetWindowHandle(window);
                        WindowId win32WindowsId = Win32Interop.GetWindowIdFromWindow(nativeWindowHandle);
                        AppWindow winuiAppWindow = AppWindow.GetFromWindowId(win32WindowsId);
                        winuiAppWindow.SetPresenter(AppWindowPresenterKind.Overlapped);
                        if (winuiAppWindow.Presenter is OverlappedPresenter p)
                        {
                            p.IsMinimizable = false;
                            p.IsMaximizable = false;
                            p.IsResizable = false;
                            p.IsAlwaysOnTop = true;
                            p.SetBorderAndTitleBar(false, false);   
                        }
                        SetWindowLong(nativeWindowHandle, GWL_EXSTYLE, GetWindowLong(nativeWindowHandle, GWL_EXSTYLE) | WS_EX_NOACTIVATE);
                        
                       
                       WindowSizeService.ResizeWindow(0,0,0,0);

                    });
                   

                });
            });

#endif

            builder.Logging
#if DEBUG
                        .AddTraceLogger(
                            options =>
                            {
                                options.MinLevel = LogLevel.Trace;
                                options.MaxLevel = LogLevel.Critical;
                            }) // Will write to the Debug Output
                        .AddStreamingFileLogger(
                            options =>
                            {
                                options.RetainDays = 2;
                                options.FolderPath = Path.Combine(
                                Environment.GetFolderPath(Environment.SpecialFolder.Desktop),
                                "MetroLogs");
                            });
#endif

#if RELEASE


                       
                        .AddConsoleLogger(
                            options =>
                            {
                                options.MinLevel = LogLevel.Information;
                                options.MaxLevel = LogLevel.Critical;
                            }); // Will write to the Console Output (logcat for android)
#endif
            builder.Services.AddSingleton(LogOperatorRetriever.Instance);
            return builder.Build();
          
        }

        
        
        private static MauiAppBuilder RegisterPages(this MauiAppBuilder builder)
        {
            builder.Services.AddSingleton<AppShell>();
            builder.Services.AddSingleton<MainPage>();
            builder.Services.AddSingleton<NumericKeyboardPage>();
            builder.Services.AddSingleton<GermanKeyboardPage>();
            return builder;
        }

        private static MauiAppBuilder RegisterViewModels(this MauiAppBuilder builder)
        {
            builder.Services.AddSingleton<MainPageViewModel>();
            builder.Services.AddSingleton<NumericKeyboardViewModel>();
            builder.Services.AddSingleton<AlphabeticKeyboardViewModel>();
            return builder;
        }

        private static MauiAppBuilder RegisterServices(this MauiAppBuilder builder)
        {
            builder.Services.AddSingleton<ITCPService,TCPService>();
            builder.Services.AddSingleton<IKeyboardService, KeyboardService>();
            builder.Services.AddSingleton<IProcessMessageService, ProcessMessageService>();
            return builder;
        }

        //Invoke declarations for setting window styles
        private const int GWL_EXSTYLE = -20;
        private const int WS_EX_NOACTIVATE = 0x08000000;

        [DllImport("user32.dll")]
        private static extern int GetWindowLong(IntPtr hwnd, int index);

        [DllImport("user32.dll")]
        private static extern int SetWindowLong(IntPtr hwnd, int index, int newStyle);
    }

   
}
