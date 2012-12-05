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

        /* Variable for the timer value. (milliseconds) */
        private uint ticks;

        private uint keyDownTime, keyUpTime, keyPressTime;

        private int firstKey = -1;
        private int secondKey = -1;

        private uint firstKeyUp;
        private uint secondKeyDown;
        private uint keyFlyTime;

        private uint prevKeyUp;

        /* Number of recorded fly times */
        private int flyRecords = 0;

        /* Number of recorded press times */
        private int pressRecords = 0;

        private int keyRecords = 0;

        /* How often the data is sent for processing */
        private int SEND_INTERVAL = 25;

        private ProfileReader profileReader;

        /* Object for storing key press times */
        public struct keyPressTimes
        {
            public int Key;
            public List<uint> KeyPress;
            public keyPressTimes(int key, List<uint> keyPress)
            {
                Key = key;
                KeyPress = keyPress;
            }
        }

        List<keyPressTimes> pressTimes = new List<keyPressTimes>();

        public struct keyFlyTimes
        {
            public int FirstKey;
            public int SecondKey;
            public List<uint> KeyFly;
            public keyFlyTimes(int firstKey, int secondKey, List<uint> keyFly)
            {
                FirstKey = firstKey;
                SecondKey = secondKey;
                KeyFly = keyFly;
            }
        }

        List<keyFlyTimes> flyTimes = new List<keyFlyTimes>();

        // this is currently in seconds, probably will change to some other format when needed (2000.0 = 2 seconds)
        double maxFilterTime = 2.0;

        public Form2(ProfileReader profileReader)
        {
            this.profileReader = profileReader;
            InitializeComponent();
            typingBoxText.Text = "Feel free to type, it will not be saved...";
            timer1.Start();
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
            keyDownTime = ticks;
            if (keyRecords > 0)
            {
                prevKeyUp = keyUpTime;
            }
        }

        private void typingBoxText_KeyUp(object sender, KeyEventArgs e)
        {
            keyUpTime = ticks;
            keyRecords++;
            int index;
            keyPressTime = keyUpTime - keyDownTime;

            // if/else for key press times
            // key exists in list, add press time to existing object's list
            if ((index = pressTimes.FindIndex(c => c.Key == e.KeyValue)) > -1)
            {
                // add time to that list
                pressTimes[index].KeyPress.Add(keyPressTime);

            }
            else // record for key does not exist. Create a new one.
            {
                // make a new object
                List<uint> temp = new List<uint>();
                temp.Add(keyPressTime);
                pressTimes.Add(new keyPressTimes(e.KeyValue, temp));
            }

            // if/else for key fly times. Make sure there is 2 keyRecords before collecting
            if (keyRecords > 1)
            {
                // This is not going to work correctly. Some variables are not updated as they should be. Going to finish when I have more time.
                secondKey = e.KeyValue;
                secondKeyDown = keyDownTime;
                firstKeyUp = prevKeyUp;
                keyFlyTime = secondKeyDown - firstKeyUp;
                if ((index = flyTimes.FindIndex(c => c.FirstKey == firstKey && c.SecondKey == secondKey)) > -1)
                {
                    flyTimes[index].KeyFly.Add(keyFlyTime);
                }
                else
                {
                    List<uint> temp = new List<uint>();
                    temp.Add(keyFlyTime);
                    flyTimes.Add(new keyFlyTimes(firstKey, secondKey, temp));
                }
                firstKey = secondKey;
            }
            else // We only have 1 record. Cannot collect fly times yet.
            {
                firstKey = secondKey = e.KeyValue;
            }
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

        /*
         * Calculates the distance between two n x 1 vectors.
         */
        private double Mahalanobis(Matrix profileData, Matrix collectedData)
        {
            // Average of the profile data
            double meanProfileData = calculateMean(profileData);

            // Average of the collected data
            double meanCollectedData = calculateMean(collectedData);

            // New matrix to hold the adjusted profile and collected data matrices
            Matrix B = new Matrix(profileData.rows, 2);
            B.SetCol(matrixSubtraction(profileData, meanProfileData), 0);
            B.SetCol(matrixSubtraction(collectedData, meanCollectedData), 1);

            // Covariance of the data
            double covM = matrixCov(B);

            // Calculates the distance (before taking the square root)
            Matrix result = Matrix.Transpose(profileData - collectedData) * ((1 / covM) * (profileData - collectedData));

            // Final distance
            double retVal = Math.Sqrt(result[0, 0]);

            return retVal;

        }

        //Calculate the covariance of a matrix
        private double matrixCov(Matrix x)
        {
            double total = 0;
            double count = x.rows;

            for (int i = 0; i < x.rows; i++)
            {
                total += x[i, 0] * x[i, 1];
            }

            return total / (count - 1);
        }

        // Calculate the mean of a n x 1 matrix
        private double calculateMean(Matrix data)
        {
            double size = data.rows;
            double total = 0;


            for (int i = 0; i < size; i++)
            {
                total += data[i, 0];
            }

            return total / size;
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
            double[] test1 = new double[] { 1.0, 2.0, 1.5, 3.0 };
            System.Diagnostics.Debug.WriteLine("Test a single array:");
            System.Diagnostics.Debug.WriteLine(string.Join(",", test1));

            test1 = FilterTimes(test1);

            System.Diagnostics.Debug.WriteLine("Single array new values:");
            System.Diagnostics.Debug.WriteLine(string.Join(",", test1));

            double[,] test2 = new double[,] { { 1.0, 1.0 }, { 2.0, 3.0 }, { 3.0, 1.5 } };

            System.Diagnostics.Debug.WriteLine("Test a 2D array:");
            for (int i = 0; i < test2.GetLength(0); i++)
            {
                System.Diagnostics.Debug.WriteLine(test2[i, 0] + ", " + test2[i, 1]);
            }

            test2 = FilterTimes(test2);

            System.Diagnostics.Debug.WriteLine("2D new values:");
            for (int i = 0; i < test2.GetLength(0); i++)
            {
                System.Diagnostics.Debug.WriteLine(test2[i, 0] + ", " + test2[i, 1]);
            }

            Matrix a = new Matrix(4, 1);
            Matrix b = new Matrix(4, 1);
            a.mat[0, 0] = 1;
            a.mat[1, 0] = 2;
            a.mat[2, 0] = 3;
            a.mat[3, 0] = 4;
            b.mat[0, 0] = 1.2;
            b.mat[1, 0] = 2.01;
            b.mat[2, 0] = 2.95;
            b.mat[3, 0] = 4.02;

            System.Diagnostics.Debug.WriteLine("Distance is: " + Mahalanobis(a, b));
        }

        // Does (Matrix) x - (double) y
        private Matrix matrixSubtraction(Matrix x, double y)
        {
            Matrix retVal = new Matrix(x.rows, 1);
            for (int i = 0; i < x.rows; i++)
            {
                retVal[i, 0] = x[i, 0] - y;
            }

            return retVal;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            ticks++;
        }
    }
}
