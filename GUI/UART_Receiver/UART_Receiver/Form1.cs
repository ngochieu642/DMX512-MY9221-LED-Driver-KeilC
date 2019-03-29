using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace UART_Receiver
{
    public partial class Form1 : Form
    {
        public string value = "0";
        public char[] valueCharArr;

        public Form1()
        {
            InitializeComponent();
        }

        private void btnOpen_Click(object sender, EventArgs e)
        {
            btnOpen.Enabled = false;
            btnClose.Enabled = true;

            try
            {
                serialPort1.PortName = cboxPort.Text;
                serialPort1.BaudRate = 19200;
                serialPort1.Parity = System.IO.Ports.Parity.None;
                serialPort1.DataBits = 8;
                serialPort1.StopBits = System.IO.Ports.StopBits.One;
                serialPort1.Encoding = Encoding.GetEncoding(28591); //Encoding 8 bit
                serialPort1.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Message", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            //Delete myPos.txt
            System.IO.File.Delete(@"dmx.txt");
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            btnOpen.Enabled = true;
            btnClose.Enabled = false;

            try
            {
                serialPort1.Close();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Message", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string[] ports = System.IO.Ports.SerialPort.GetPortNames();
            cboxPort.Items.AddRange(ports);
            cboxPort.SelectedIndex = 0;
            btnClose.Enabled = false;
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {           
            this.Invoke(new EventHandler(WriteEvent));
        }

        private void WriteEvent(object sender, EventArgs e)
        {
            value = (serialPort1.IsOpen) ? serialPort1.ReadLine() : 0.ToString();
            using (StreamWriter w = File.AppendText("dmx.txt"))
            {
                valueCharArr = value.ToCharArray();
                foreach (char ch in valueCharArr)
                {
                    w.WriteLine(ch - 48);
                }                
            }
        }
    }
}
