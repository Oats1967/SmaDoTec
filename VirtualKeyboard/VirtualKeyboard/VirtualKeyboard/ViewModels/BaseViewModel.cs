using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using Microsoft.Extensions.Logging;
using VirtualKeyboard.Services;

namespace VirtualKeyboard.ViewModels
{
    public class BaseViewModel : ObservableObject
    {
        protected ILogger _logger;
        public BaseViewModel(ILogger<BaseViewModel> logger)
        {

            _logger = logger;

        }
    }
}
