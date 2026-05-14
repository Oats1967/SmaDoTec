using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VirtualKeyboard.Commands
{
    public class TKSetShow
    {
        public TKSetShow(Layouts layout)
        {
            Layout = layout;
        }
        public Layouts Layout { get; private set; }
    }
}
