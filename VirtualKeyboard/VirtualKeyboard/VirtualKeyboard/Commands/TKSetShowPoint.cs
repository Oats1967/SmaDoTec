using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VirtualKeyboard.Commands
{
    public class TKSetShowPoint
    {
        public TKSetShowPoint(Layouts layout, int x, int y)
        {
            Layout = layout;
            X =  Math.Max(0, Math.Min(x, (int)DeviceDisplay.MainDisplayInfo.Width));
            Y = Math.Max(0, Math.Min(y, (int)DeviceDisplay.MainDisplayInfo.Height));
        }
        public int X { get; private set; }
        public int Y { get; private set; }
        public Layouts Layout { get; private set; }
    }
}
