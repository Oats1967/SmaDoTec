using CommunityToolkit.Mvvm.Input;
using MetroLog;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using VirtualKeyboard.Services;

namespace VirtualKeyboard.ViewModels
{
  
    public partial class NumericKeyboardViewModel : KeyboardViewModel
    {

      
        public NumericKeyboardViewModel(IKeyboardService keyboardService, ILogger<NumericKeyboardViewModel> logger) : base(keyboardService, logger)
        {
        }

        protected override int InitWidth => (int)(DeviceDisplay.Current.MainDisplayInfo.Height / 2);

        protected override int InitHeight => (int)(Width * ResolutionConfig.ResolutionToKeyboardRatio[(DeviceDisplay.Current.MainDisplayInfo.Width, DeviceDisplay.Current.MainDisplayInfo.Height)].numericRatio);

        protected override int InitX => (int)((DeviceDisplay.Current.MainDisplayInfo.Width - Width) / 2);
        protected override int InitY => (int)((DeviceDisplay.Current.MainDisplayInfo.Height - Height) / 2);


        protected override (double current, double max) SizeRef => (Application.Current!.Windows[0].Height, DeviceDisplay.Current.MainDisplayInfo.Height);

        [RelayCommand]
        public override void BackspacePressed()
        {
            _keyboardService.SendKey(0x08);
        }
        [RelayCommand]
        public override void KeyPressed(string key)
        {
            _keyboardService.SendKey(Convert.ToChar(key));
        }

        public override void SizeChanged(object? sender, EventArgs e)
        {
            _logger.LogInformation("SizeChanged");
            var fontSize = ResolutionConfig.ResolutionToFontSize[(DeviceDisplay.Current.MainDisplayInfo.Width, DeviceDisplay.Current.MainDisplayInfo.Height)];
            var keySpacing = ResolutionConfig.ResolutionToSpacing[(DeviceDisplay.Current.MainDisplayInfo.Width, DeviceDisplay.Current.MainDisplayInfo.Height)];
            FontSize = ValueScaler.MapLinear(SizeRef.current, 0, SizeRef.max, fontSize.numericMin, fontSize.numericMax);
            KeySpacing = ValueScaler.MapLinear(SizeRef.current, 0, SizeRef.max, keySpacing.numericMin, keySpacing.numericMax);
        }
    }
}
