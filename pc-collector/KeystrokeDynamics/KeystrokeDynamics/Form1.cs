using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace KeystrokeDynamics
{
    public partial class Form1 : Form
    {
        private uint ticks;
        private uint keyDown, keyUp, keyPress;
        private int numRecords = 0;

        public struct keyRecord
        {
            public string Key;
            public uint DownTime;
            public uint UpTime;
            public keyRecord(string key, uint downTime, uint upTime)
            {
                Key = key;
                DownTime = downTime;
                UpTime = upTime;
            }
        }


        List<keyRecord> records = new List<keyRecord>();


        public Form1()
        {
            InitializeComponent();
            timer1.Start();
        }

        private void textBox1_KeyDown(object sender, KeyEventArgs e)
        {
            keyDown = ticks;
            textBox3.Text = e.KeyCode.ToString();
            textBox4.Text = keyDown.ToString();
        }

        private void textBox1_KeyPress(object sender, KeyPressEventArgs e)
        {

        }

        private void textBox1_KeyUp(object sender, KeyEventArgs e)
        {
            keyUp = ticks;
            keyPress = keyUp - keyDown;
            textBox5.Text = keyPress.ToString();
            textBox6.Text = keyUp.ToString();

            records.Add(new keyRecord(e.KeyCode.ToString(), keyDown, keyUp));

            numRecords++;
            textBox7.Text = numRecords.ToString();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            ticks++;
            textBox2.Text = ticks.ToString();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            timer1.Stop();
            string pathname = "C:\\Users\\" + Environment.UserName + "\\test.txt";
            System.IO.StreamWriter file = new System.IO.StreamWriter(pathname);
            file.WriteLine("{");
            foreach (keyRecord key in records) {
                file.WriteLine("\t\"key\": \"" + key.Key + "\",");
                file.WriteLine("\t\"keyDown\": \"" + key.DownTime + "\",");
                if (key.Equals(records.Last()))
                {
                    file.WriteLine("\t\"keyUp\": \"" + key.UpTime + "\"");
                }
                else
                {
                    file.WriteLine("\t\"keyUp\": \"" + key.UpTime + "\",");
                }
                file.WriteLine();
            }
            file.WriteLine("}");
            file.Close();
            var result = MessageBox.Show("Wrote JSON results to file: " + pathname, "Collection Complete", MessageBoxButtons.OK, MessageBoxIcon.Question);
            if (result == DialogResult.OK)
            {
                Application.Exit();
            }
        }
    }

}
