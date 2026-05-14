using Microsoft.Extensions.Logging;
using System.Collections;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace VirtualKeyboard.Services
{
   
    public class TCPService : ITCPService
    {
        ILogger _logger;
        IProcessMessageService _messageService;
        public TCPService(ILogger<TCPService> logger, IProcessMessageService messageService)
        {
           _logger = logger;
            _messageService = messageService;
            StartAsync();
        }



        private async void StartAsync()
        {
             
             IPAddress ip = IPAddress.Loopback; // Use loopback address for local host
             IPEndPoint ipEndPoint = new IPEndPoint(ip, 2000);
             using Socket server = new(ipEndPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
             server.Bind(ipEndPoint);
             server.Listen();
            _logger.LogInformation("Server started listening");

            var handler = await server.AcceptAsync();
            try
            {
                var buffer = new byte[1024];
                while (true)
                {
                   
                    var received = await handler.ReceiveAsync(buffer, SocketFlags.None);
                    if (received > 0)
                    {
                        // Process the received data in the buffer (up to 'received' bytes)
                        byte[] receivedData = new byte[received];
                        Array.Copy(buffer, receivedData, received);
                        string hexRepresentation = BitConverter.ToString(receivedData).Replace("-", " ");
                        _logger.LogInformation($"Server received message: {hexRepresentation}");
                        _messageService.Process(receivedData);
                        Array.Clear(receivedData, 0, receivedData.Length);
                       
                    }
                    else 
                    { 
                         _logger.LogInformation($"Server received message: shutdown");
                        break;
                    }
                }
            }
            catch(SocketException ex)
            {
                _logger.LogError($"Client disconnected: {ex.Message}");
            }
            finally
            {
                // Additional cleanup or logging if needed
                handler.Close();
                Application.Current?.CloseWindow(Application.Current!.MainPage!.Window);
                _logger.LogInformation($"Application shutdown forced by client");
            }
        }

       
    }
}
