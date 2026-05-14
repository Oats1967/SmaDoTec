using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VirtualKeyboard.Services
{
    public interface IKeyboardService
    {
        public void SendKey(ushort key);
        public void SendKey(char key);

    }
}
