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

        // Global Values

        // this is currently in seconds, probably will change to some other format when needed (2000.0 = 2 seconds)
        double maxFilterTime = 2.0;

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

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
            
        }

        private void label1_Click_1(object sender, EventArgs e)
        {

        }

        private double Mahalanobis(double[] profileData, double[] collectedData) 
        {
            double[,] covM;
            double[] xMy;
            double[] xMyTranspose;
            double[,] distance;
            double[,] joinedMatrix;

            int info;
            alglib.matinvreport rep;

            joinedMatrix = joinMatrix(profileData, collectedData);

            alglib.covm(joinedMatrix, out covM);

            xMy = matrixSubtraction(profileData, collectedData);
            // Dunno if i need this
            xMyTranspose = matrixTranspose(xMy);

            alglib.rmatrixinverse(ref covM, out info, out rep);

            distance = vectorMatrixMultiply(xMyTranspose, covM);
            distance = matrixVectorMultiply(distance, xMy);


            return 0;

        }

        private double[,] joinMatrix(double[] x, double[] y)
        {
            double[,] joined = new double[x.GetLength(0), 2];

            for (int i = 0; i < x.GetLength(0); i++)
            {
                for (int j = 0; j < 1; j++)
                {
                    joined[i, j] = x[i];
                    joined[i, j + 1] = y[i];
                }
            }
            return joined;
        }

        // Filters out times based upon maxFilterTime
        private double[] FilterTimes(double[] data)
        {

            List<double> filter = new List<double>();

            foreach (double d in data) 
            {
                if (d <= maxFilterTime)
                    filter.Add(d);
                           
            }

            return filter.ToArray();
        }

        // Filters out times based upon maxFilterTime
        private double[,] FilterTimes(double[,] data)
        {

            List<double> outer = new List<double>();
            List<double> inner = new List<double>();

            for (int i = 0; i < data.GetLength(0); i++) 
            {
                if (data[i, 1] <= maxFilterTime)
                {
                    outer.Add(data[i, 0]);
                    inner.Add(data[i, 1]);
                }                 
            }

            double[,] filteredVals = new double[outer.Count, inner.Count];

            for (int i = 0; i < outer.Count; i++)
            {
                filteredVals[i, 0] = outer.ElementAt(i);
                filteredVals[i, 1] = inner.ElementAt(i);
            }
            
            return filteredVals;
        }


        private void testFilterTimes(object sender, EventArgs e)
        {
            double[] test1 = new double[] {1.0, 2.0, 1.5, 3.0};
            System.Diagnostics.Debug.WriteLine("Test a single array:");
            System.Diagnostics.Debug.WriteLine(string.Join(",", test1));

            test1 = FilterTimes(test1);

            System.Diagnostics.Debug.WriteLine("Single array new values:");
            System.Diagnostics.Debug.WriteLine(string.Join(",", test1));

            double[,] test2 = new double[,] { { 1.0, 1.0 }, { 2.0, 3.0 }, { 3.0, 1.5 } };

            System.Diagnostics.Debug.WriteLine("Test a 2D array:");
            for (int i = 0; i < test2.GetLength(0); i++)
            {
                System.Diagnostics.Debug.WriteLine(test2[i,0] + ", " + test2[i, 1]);
            }

            test2 = FilterTimes(test2);

            System.Diagnostics.Debug.WriteLine("2D new values:");
            for (int i = 0; i < test2.GetLength(0); i++)
            {
                System.Diagnostics.Debug.WriteLine(test2[i, 0] + ", " + test2[i, 1]);
            }

            double[,] covM;
            double[,] test3 = new double[,] { {1, 1}, {2, 2}, {3, 3} };
            // Calculate the covariance matrix between matrices x, y
            alglib.covm(test3, out covM);
            
            System.Diagnostics.Debug.WriteLine("CovMatrix:");
            for (int i = 0; i < covM.GetLength(0); i++)
            {
                for (int j = 0; j < covM.GetLength(1); j++)
                {
                    System.Diagnostics.Debug.WriteLine(covM[i, j] + ", " + covM[i, j]);
                }
            }

            System.Diagnostics.Debug.WriteLine("Calculating Mahalanobis distance");

            double[] d1 = new double[] { 1, 2, 3 };
            double[] d2 = new double[] { 1, 2, 3 };

            double result = Mahalanobis(d1, d2);

            System.Diagnostics.Debug.WriteLine("distance: " + result);

        }

        // Does x - y
        private double[] matrixSubtraction(double[] x, double[] y)
        {
            double[] retVal = new double[x.GetLength(0)];
            for (int i = 0; i < x.GetLength(0); i++)
            {
                retVal[i] = x[i] - y[i];
            }

            return retVal;
        }

        private double[] matrixTranspose(double[] x)
        {
            double[] transpose = new double[x.GetLength(0)];

            for (int i = 0; i < x.GetLength(0); i++)
            {
                transpose[i] = x[i];
            }
            return transpose;
        }

        private double[,] vectorMatrixMultiply(double[] x, double[,] y)
        {
            double[,] multipled = new double[x.GetLength(0), y.GetLength(1)];
            for (int i = 0; i < x.GetLength(0); i++)
            {
                for (int j = 0; j < y.GetLength(1); j++)
                {
                    multipled[i, j] += x[i] * y[i, j];
                }
            }
            return multipled;
        }

        private double[,] matrixVectorMultiply(double[,] x, double[] y)
        {
            double[,] multipled = new double[x.GetLength(0), y.GetLength(0)];
            for (int i = 0; i < x.GetLength(0); i++)
            {
                for (int j = 0; j < y.GetLength(0); j++)
                {
                    multipled[i, j] += x[i, j] * y[j];
                }
            }
            return multipled;
        }
    }
}
