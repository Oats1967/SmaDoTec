using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VirtualKeyboard.Services
{
    public interface IProcessMessageService
    {
        void Process(byte[] buffer);
    }
}
