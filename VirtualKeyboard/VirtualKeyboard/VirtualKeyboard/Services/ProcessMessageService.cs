using CommunityToolkit.Mvvm.Messaging;
using VirtualKeyboard.Commands;


namespace VirtualKeyboard.Services
{
    public class ProcessMessageService : IProcessMessageService
    {
        private const byte STX = 0x02;
        private const byte ETX = 0x03;

        private const byte TKSetSize = 0x11;
        private const byte TKSetShowPoint = 0x13;
        private const byte TKSetShow = 0x14;
        private const byte TKHide = 0x15;
        public void Process(byte[] buffer)
        {
            if (buffer == null) return;
            if (buffer.Length < 4) return;
            if (buffer[0]!= STX) return;
            if (buffer[buffer.Length-1]!= ETX) return;
            if (buffer[1] != buffer.Length - 2) return;

            switch (buffer[2])
            {
                case TKSetSize: 
                    NotifyTKSetSize(buffer);
                    break;
                case TKSetShowPoint: 
                    NotifyTKSetShowPoint(buffer);
                    break;
                case TKSetShow: 
                    NotifyTKSetShow(buffer);
                    break;
                case TKHide: 
                    NotifyTKSetHide(buffer);
                    break;
                default: 
                    throw new NotImplementedException();
            }
            
        }

        private void NotifyTKSetHide(byte[] buffer)
        {
            WeakReferenceMessenger.Default.Send(new TKSetHide());
        }

        private void NotifyTKSetShow(byte[] buffer)
        {
            var layout = (Layouts)buffer[3];
            WeakReferenceMessenger.Default.Send(new TKSetShow(layout));
        }

        private void NotifyTKSetShowPoint(byte[] buffer)
        {
            var layout = (Layouts)buffer[3];
            var x = (buffer[5] << 8) | buffer[4];
            var y = (buffer[7] << 8) | buffer[6];
            WeakReferenceMessenger.Default.Send(new TKSetShowPoint(layout, x, y));
        }

        private void NotifyTKSetSize(byte[] buffer)
        
        {
            var layout = (Layouts)buffer[3];
            var percentage = buffer[4];
            WeakReferenceMessenger.Default.Send(new TKSetSize(layout, percentage));
        }
    }

    
}
