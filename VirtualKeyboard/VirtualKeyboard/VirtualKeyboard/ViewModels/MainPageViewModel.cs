using CommunityToolkit.Mvvm.Messaging;
using Microsoft.Extensions.Logging;
using VirtualKeyboard.Commands;
using VirtualKeyboard.Pages;
using VirtualKeyboard.Services;




namespace VirtualKeyboard.ViewModels
{
    public class MainPageViewModel : BaseViewModel, IRecipient<TKSetShow>, IRecipient<TKSetHide>, IRecipient<TKSetShowPoint>
    { 
      
        public MainPageViewModel( ILogger<MainPageViewModel> logger) : base(logger)
        {
            WeakReferenceMessenger.Default.RegisterAll(this);
        }

        public void Appearing(object? sender, EventArgs e)
        {
            var x = 0; var y = 0; var width = 0; var height = 0;    
            WindowSizeService.ResizeWindow(x, y, width, height);
            _logger.LogInformation($"Window was resized to {x} {y} {width} {height}");
        }


        async void IRecipient<TKSetShow>.Receive(TKSetShow message)
        {
            _logger.LogInformation($"TKSetShow received");
            
            switch (message.Layout)
            {
                case Layouts.Numeric:
                    await OpenNumericKeyboardAsync();
                    break;
                case Layouts.German:
                    await OpenGermanKeyboardAsync();
                    break;
                default: return;
            }
        }

        async void IRecipient<TKSetHide>.Receive(TKSetHide message)
        {
            _logger.LogInformation($"TKSetHide received");
            await CloseKeyboardAsync();

        }

        async void IRecipient<TKSetShowPoint>.Receive(TKSetShowPoint message)
        {
            _logger.LogInformation($"TKSetShowPoint received");
           
            switch (message.Layout)
            {
                case Layouts.Numeric:
                    await OpenNumericKeyboardAsync();
                    break;
                case Layouts.German:
                    await OpenGermanKeyboardAsync();
                    break;
                default: return;
            }
        }


       
        private async Task OpenNumericKeyboardAsync()
        {
            
            _logger.LogInformation($"Opening {nameof(NumericKeyboardPage)}");
            await Shell.Current.GoToAsync($"//{nameof(NumericKeyboardPage)}");
            
        }
        private async Task CloseKeyboardAsync()
        {
           
            _logger.LogInformation($"Opening {nameof(MainPage)}");
            await Shell.Current.GoToAsync($"//{nameof(MainPage)}");
        }
        private async Task OpenGermanKeyboardAsync()
        {
          
            _logger.LogInformation($"Opening {nameof(GermanKeyboardPage)}");
            await Shell.Current.GoToAsync($"//{nameof(GermanKeyboardPage)}");
            
        }
    }
}
