using System;
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
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
            typingBoxText.Text = "Feel free to type, it will not be saved...";
        }

        public Form RefToForm1 { get; set; }

        private void logoutButton_Click(object sender, EventArgs e)
        {
            this.RefToForm1.Show();
            this.Close();
        }

        private void Form2_FormClosing(object sender, FormClosingEventArgs e)
        {
            //this.RefToForm1.Close();
            this.RefToForm1.Show();
            //this.Close();
        }

        private void typingBoxText_KeyDown(object sender, KeyEventArgs e)
        {
            //MessageBox.Show("Key: " + e.KeyValue + " pressed down", "Key Down", MessageBoxButtons.OK);
        }

        private void typingBoxText_KeyUp(object sender, KeyEventArgs e)
        {
            //MessageBox.Show("Key: " + e.ToString() + " released", "Key Up", MessageBoxButtons.OK);
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            //typingBoxLabel.Focus();
        }



    }
}
