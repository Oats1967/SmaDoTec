using System.Runtime.InteropServices;
using System.Text;
using VirtualKeyboard.Services;
using CommunityToolkit.Mvvm.Input;
using CommunityToolkit.Mvvm.ComponentModel;
using VirtualKeyboard.Controls;
using Microsoft.Extensions.Logging;


namespace VirtualKeyboard.ViewModels
{
    public partial class AlphabeticKeyboardViewModel : KeyboardViewModel
    {


        public AlphabeticKeyboardViewModel(IKeyboardService keyboardService, ILogger<AlphabeticKeyboardViewModel> logger) : base(keyboardService,logger)
        {
            Locked = false;
            CapsLock = false;
        }

        [ObservableProperty]
        private bool locked;

        [ObservableProperty]
        private bool capsLock;


        [RelayCommand]
        public void Lock()
        {

            CapsLock = !CapsLock;
            Locked = CapsLock;
        }

        [RelayCommand]
        public void CapsLockPressed()
        {
            Locked = false;
            CapsLock = !CapsLock;
        }

        [RelayCommand]
        public override void KeyPressed(string key)
        {
            if (!Locked)
            {
                CapsLock = false;
            }
            _keyboardService.SendKey(Convert.ToChar(key));
        }

        [RelayCommand]
        public override void BackspacePressed()
        {
            if (!Locked)
            {
                CapsLock = false;
            }
            _keyboardService.SendKey(0x08);
        }

        public override void SizeChanged(object? sender, EventArgs e)
        {
            _logger.LogInformation("SizeChanged");
            var fontSize = ResolutionConfig.ResolutionToFontSize[(DeviceDisplay.Current.MainDisplayInfo.Width, DeviceDisplay.Current.MainDisplayInfo.Height)];
            var keySpacing = ResolutionConfig.ResolutionToSpacing[(DeviceDisplay.Current.MainDisplayInfo.Width, DeviceDisplay.Current.MainDisplayInfo.Height)];
            FontSize = ValueScaler.MapLinear(SizeRef.current, 0, SizeRef.max, fontSize.alphaMin, fontSize.alphaMax);
            KeySpacing = ValueScaler.MapLinear(SizeRef.current, 0, SizeRef.max, keySpacing.alphaMin, keySpacing.alphaMax);
        }

        protected override int InitWidth => (int)(DeviceDisplay.Current.MainDisplayInfo.Width / 2);

       
        protected override int InitHeight => (int)(Width * ResolutionConfig.ResolutionToKeyboardRatio[(DeviceDisplay.Current.MainDisplayInfo.Width, DeviceDisplay.Current.MainDisplayInfo.Height)].alphaRatio);
        

        protected override int InitX => (int)((DeviceDisplay.Current.MainDisplayInfo.Width - Width) / 2);

        

        protected override int InitY => (int)((DeviceDisplay.Current.MainDisplayInfo.Height - Height) / 2);
        

        protected override (double current, double max) SizeRef => (Application.Current!.Windows[0].Width, DeviceDisplay.Current.MainDisplayInfo.Width);
        

       
        
        
    }
}