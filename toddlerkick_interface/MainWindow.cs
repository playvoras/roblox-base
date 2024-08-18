using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace toddlerkick_interface
{
    public partial class MainWindow : Form
    {
        public MainWindow() => InitializeComponent();

        private void ExecutionHandler(object sender, EventArgs e)
        {
            switch (((Button)sender).Name)
            {
                case "ExecuteButton":
                    using (Socket clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp))
                    {
                        clientSocket.Connect("127.0.0.1", 42069);

                        byte[] data = Encoding.ASCII.GetBytes(ScriptBox.Text);

                        int sent = 0;
                        while (sent < data.Length)
                            sent += clientSocket.Send(data, sent, data.Length - sent, SocketFlags.None);

                        clientSocket.Disconnect(false);
                    }
                    break;
                case "ClearButton":
                    ScriptBox.Clear();
                    break;
                case "AttachButton":
                    break;
            }
        }
    }
}
