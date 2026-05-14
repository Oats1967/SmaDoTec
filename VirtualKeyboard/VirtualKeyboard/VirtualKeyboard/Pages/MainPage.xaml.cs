using CommunityToolkit.Mvvm.Messaging;
using VirtualKeyboard.Commands;
using VirtualKeyboard.Pages;
using VirtualKeyboard.Services;
using VirtualKeyboard.ViewModels;

namespace VirtualKeyboard.Pages
{
    public partial class MainPage : ContentPage
    {


       // for dependency injection Pages have to be initialized through constructor
        public MainPage(MainPageViewModel viewModel, NumericKeyboardPage p1, GermanKeyboardPage p2)
        {
            InitializeComponent();
            BindingContext = viewModel;
            Appearing += viewModel.Appearing;
        }

        

      
        


        
    }

}
