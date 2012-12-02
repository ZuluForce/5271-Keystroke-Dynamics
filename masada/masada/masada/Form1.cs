using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace masada
{
    public partial class Form1 : Form
    {
        /* Variable for the timer value */
        private uint ticks;

        private uint keyDown, keyUp, keyPress;

        /* Number of keystroke records */
        private int numRecords = 0;

        /* How often the data is sent for processing */
        private int SEND_INTERVAL = 25;

        /* Object for storing keystroke data */
        public struct keyRecord
        {
            public int Key;
            public uint DownTime;
            public uint UpTime;
            public keyRecord(int key, uint downTime, uint upTime)
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
            passwordTextBox.Text = "";
            passwordTextBox.PasswordChar = '*';
            timer1.Start();
        }

        private void logingButton_Click(object sender, EventArgs e)
        {
            if (checkUser(usernameTextBox.Text, passwordTextBox.Text))
            {
                // MessageBox.Show("Logged In", "Success", MessageBoxButtons.OK);
                Form2 form2 = new Form2();
                form2.RefToForm1 = this;
                form2.Show();
                this.Hide();
            }
            else
            {
                MessageBox.Show("Failed to Login", "Nope...", MessageBoxButtons.OK);
            }

        }

        private void logingButton_Click()
        {
            if (checkUser(usernameTextBox.Text, passwordTextBox.Text))
            {
                // MessageBox.Show("Logged In", "Success", MessageBoxButtons.OK);
                Form2 form2 = new Form2();
                form2.RefToForm1 = this;
                form2.Show();
                this.Hide();
            }
            else
            {
                MessageBox.Show("Failed to Login", "Nope...", MessageBoxButtons.OK);
            }

        }

        private bool checkUser(string _username, string _password)
        {
            string[] lines = openLoginFile();

            string username = usernameTextBox.Text;
            string password = passwordTextBox.Text;

            foreach (string line in lines)
            {
                if (line.Equals(""))
                {
                    System.Diagnostics.Debug.WriteLine("Found an empty value...continuing");
                }
                else
                {
                    string tempLine = line.Trim();
                    string[] tempLines = tempLine.Split(':');
                    if (tempLines.Length == 2)
                    {
                        if (tempLines[0].Equals(username) && tempLines[1].Equals(password))
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        private bool checkUserByUsername(string _username)
        {
            string[] lines = openLoginFile();

            string username = usernameTextBox.Text;
            string password = passwordTextBox.Text;

            foreach (string line in lines)
            {
                if (line.Equals(""))
                {
                    System.Diagnostics.Debug.WriteLine("Found an empty value...continuing");
                }
                else
                {
                    string tempLine = line.Trim();
                    string[] tempLines = tempLine.Split(':');
                    if (tempLines.Length == 2)
                    {
                        if (tempLines[0].Equals(username))
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        private void adduserButton_Click(object sender, EventArgs e)
        {
            if (!checkUserByUsername(usernameTextBox.Text))
            {
                writeToLoginFile(usernameTextBox.Text + ":" + passwordTextBox.Text);
                MessageBox.Show("Successfully added " + usernameTextBox.Text + " to the list of users! Proceed to login.", "Success", MessageBoxButtons.OK);
                usernameTextBox.Text = "";
                passwordTextBox.Text = "";
            }
            else
            {
                MessageBox.Show("User " + usernameTextBox.Text + " appears to already exist", "Failure", MessageBoxButtons.OK);
                usernameTextBox.Text = "";
                passwordTextBox.Text = "";
            }
        }

        private string[] openLoginFile()
        {
            if (!File.Exists(Directory.GetCurrentDirectory() + "\\login.txt"))
            {
                FileStream fs = File.Create(Directory.GetCurrentDirectory() + "\\login.txt");
                fs.Close();
                StreamWriter sw = File.AppendText(Directory.GetCurrentDirectory() + "\\login.txt");
                sw.WriteLine("admin:123456");
                sw.Close();
            }
            string text = System.IO.File.ReadAllText(Directory.GetCurrentDirectory() + "\\login.txt");
            string[] lines = text.Split('\r');
            return lines;
        }

        private void writeToLoginFile(string _write)
        {
            string path = Directory.GetCurrentDirectory() + "\\login.txt";

            using (StreamWriter sw = File.AppendText(path))
            {
                sw.WriteLine(_write);
            }
        }


        private void passwordTextBox_KeyPress(object sender, KeyPressEventArgs e)
        {

        }

        private void passwordTextBox_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                logingButton_Click();
            }
            else
            {
                keyDown = ticks;
            }
        }

        private void passwordTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            keyUp = ticks;

            records.Add(new keyRecord(e.KeyValue, keyDown, keyUp));
            numRecords++;

            if ((numRecords % SEND_INTERVAL) == 0)
            {
                //Code for sending data...
                MessageBox.Show("Sending data...");
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            ticks++;
        }

    }
}
