#if WINDOWS
using Microsoft.UI.Windowing;
using Windows.Graphics;
using Windows.UI.WindowManagement;
#endif

namespace VirtualKeyboard.Services
{
    internal static class WindowSizeService
    {
        
            public static void ResizeWindow(int x, int y, int width, int height)
            {

#if WINDOWS
            var window = Application.Current?.Windows.First(uiWindow =>
                {
                    if (uiWindow.Handler?.PlatformView is MauiWinUIWindow winUIWindow)
                    {

                        return true;
                    }
                    return false;
                });
            var platformView = window!.Handler?.PlatformView as MauiWinUIWindow;
            var winUiWindow = platformView!.AppWindow;
            if (winUiWindow.Presenter is OverlappedPresenter presenter)
            {
                winUiWindow.SetPresenter(AppWindowPresenterKind.Overlapped);
                presenter.IsAlwaysOnTop = true;

            }
            winUiWindow.MoveAndResize(new RectInt32(x, y, width, height));

#endif
        }

        



    }
}
