using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TCPTest
{
    internal class ProcessLogger
    {
        private Timer _logTimer;
        private string LogFilePath { get; init; }
        private string ProcessName {  get; init; }
        private TimeSpan Intervall { get; init; }

        

        public ProcessLogger(string processName, string logFilePath, TimeSpan intervall)
        {
            ProcessName = processName;
            LogFilePath = logFilePath;
            Intervall = intervall;
        }
        public void Start()
        {
           ClearFile();

          
            _logTimer = new Timer( LogInformation, null, 0, (int) Intervall.TotalMilliseconds);
        }

        public void Stop()
        {
            _logTimer.Dispose();
        }



        private  void LogInformation(object state)
        {
            string logMessage;
            var processes = Process.GetProcessesByName(ProcessName);
            if (processes.Length == 0)
            {
                logMessage = $"{DateTime.Now}:\n" +
                    $"\t-Process not found\n";
                LogToFile(logMessage);
            }

            foreach (var process in processes)
            {
                logMessage = $"{DateTime.Now}:\n" +
                    $"\t-CPU Usage: {process.TotalProcessorTime}" +
                    $"\n\t-Memory Usage: {process.PrivateMemorySize64} bytes\n";
                LogToFile(logMessage);
            }
        }

        private  void LogToFile(string message)
        {
            try
            {
                // Append the log message to the file
                File.AppendAllText(LogFilePath, message + Environment.NewLine);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error writing to the log file: {ex.Message}");
            }
        }

        private void ClearFile()
        {
            try
            {
                // Create an empty file to clear its contents
                File.WriteAllText(LogFilePath, string.Empty);

            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error clearing the log file: {ex.Message}");
            }
        }


       


    }
}
