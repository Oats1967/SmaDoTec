using Microsoft.UI;
using VirtualKeyboard.Services;
using Windows.Graphics;

namespace VirtualKeyboard
{

    public partial class App : Application
    {
        
        
        public App(ITCPService tcpService , AppShell shell)
        {
            InitializeComponent();
           
            MainPage = shell;

            

        }

        


    }
}
