using System.Net;
using System.Net.Sockets;


namespace TCPTest;

public class Program
{
    
    private static readonly List<(ArraySegment<byte> byteStream, string commandMessage)> Commands = new()
     {
        #region TKSetSize
          ( new ArraySegment<byte>([0x02, 7, 0x11, 1, 0x64, 0, 0, 0, 0x03]), "TKSetSize Numeric 100%" ) ,
           ( new ArraySegment<byte>([0x02, 7, 0x11, 1, 0x4b, 0, 0, 0, 0x03]), "TKSetSize Numeric 75%" ) ,
          ( new ArraySegment<byte>([0x02, 7, 0x11, 1, 0x32, 0, 0, 0, 0x03]), "TKSetSize Numeric 50%" ) ,
         
          ( new ArraySegment<byte>([0x02, 7, 0x11, 2, 0x64, 0, 0, 0, 0x03]), "TKSetSize German 100%" ) ,
           ( new ArraySegment<byte>([0x02, 7, 0x11, 2, 0x4b, 0, 0, 0, 0x03]), "TKSetSize German 75%" ) ,
          ( new ArraySegment<byte>([0x02, 7, 0x11, 2, 0x32, 0, 0, 0, 0x03]), "TKSetSize German 50%" ) ,
         
          ( new ArraySegment<byte>([0x02, 7, 0x11, 3, 0x64, 0, 0, 0, 0x03]), "TKSetSize Not used 100%" ) ,
          ( new ArraySegment<byte>([0x02, 7, 0x11, 3, 0x4b, 0, 0, 0, 0x03]), "TKSetSize Not used 75%" ) ,
          ( new ArraySegment<byte>([0x02, 7, 0x11, 3, 0x32, 0, 0, 0, 0x03]), "TKSetSize Not used 50%" ) ,
           
        #endregion



        #region TKSetShowPoint
           ( new ArraySegment<byte>([0x02, 7, 0x13, 1, 0, 0, 0, 0, 0x03]), "TKSetShowPoint Numeric 0 0" ) ,
          ( new ArraySegment<byte>([0x02, 7, 0x13, 1, 0x90, 0x01, 0x90, 0x01, 0x03]), "TKSetShowPoint Numeric 400 400" ) ,

          ( new ArraySegment<byte>([0x02, 7, 0x13, 2, 0, 0, 0, 0, 0x03]), "TKSetShowPoint German 0 0" ) ,
          ( new ArraySegment<byte>([0x02, 7, 0x13, 2, 0x90, 0x01, 0x90, 0x01, 0x03]), "TKSetShowPoint German 400 400" ) ,

          ( new ArraySegment<byte>([0x02, 7, 0x13, 3, 0, 0, 0, 0, 0x03]), "TKSetShowPoint Not used 0 0" ) ,
          ( new ArraySegment<byte>([0x02, 7, 0x13, 3, 0x90, 0x01, 0x90, 0x01, 0x03]), "TKSetShowPoint Not used 400 400" ) ,
         

        #endregion


        #region TKSetShow
        // serious
        ( new ArraySegment<byte>([0x02, 3, 0x14, 1, 0x03]), "TKSetShow Numeric" ) ,
         ( new ArraySegment<byte>([0x02, 3, 0x14, 2, 0x03]), "TKSetShow German" ) ,
         ( new ArraySegment<byte>([0x02, 3, 0x14, 3, 0x03]), "TKSetShow Not used" ) ,

        // // bullshit
        #endregion


        #region TKSetHide
           ( new ArraySegment<byte>([0x02, 2, 0x15,  0x03]), "TKSetHide" ) ,
       #endregion
    };

    public async static Task Main(string[] args)
    {
        
        var logger = new ProcessLogger( "VirtualKeyboard", Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Desktop), "LogFile.txt"), TimeSpan.FromMinutes(30)); 
        logger.Start();
        var ip = IPAddress.Loopback; // Use loopback address for local host
        var iPEndPoint = new IPEndPoint(ip, 2000);

        // client socket
        using var client = new Socket(ip.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

        await client.ConnectAsync(iPEndPoint);
        var random = new Random();
        DateTime startTime = DateTime.Now;
        DateTime endTime = startTime.AddHours(24);
        while (DateTime.Now < endTime)
        {

            var command = Commands[random.Next(0, Commands.Count)];
            Console.WriteLine($"{DateTime.Now}: {command.commandMessage}");
            await client.SendAsync(command.byteStream, SocketFlags.None);
            Thread.Sleep(1000);


        }
        logger.Stop();
    }

}