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

        public struct rawTimes
        {
            public int Key;
            public uint DownTime;
            public uint UpTime;
            public rawTimes(int key, uint downTime, uint upTime)
            {
                Key = key;
                DownTime = downTime;
                UpTime = upTime;
            }
        }

        List<rawTimes> tempList = new List<rawTimes>();

        // this is currently in seconds, probably will change to some other format when needed (2000.0 = 2 seconds)
        double maxFilterTime = 2.0;
        double stdForDistance = 1.0;

        public Form2(ProfileReader profileReader)
        {
            this.profileReader = profileReader;
            InitializeComponent();
            stdBox.Text = stdForDistance.ToString();
            typingBoxText.Text = "Feel free to type, it will not be saved...";
            paragraphText.Text = "So then, his armour being furbished, his morion turned into a helmet, his hack christened, and he himself confirmed, he came to the conclusion that nothing more was needed now but to look out for a lady to be in love with; for a knight-errant without love was like a tree without leaves or fruit, or a body without a soul. As he said to himself, \"If, for my sins, or by my good fortune, I come across some giant hereabouts, a common occurrence with knights-errant, and overthrow him in one onslaught, or cleave him asunder to the waist, or, in short, vanquish and subdue him, will it not be well to have some one I may send him to as a present, that he may come in and fall on his knees before my sweet lady, and in a humble, submissive voice say, 'I am the giant Caraculiambro, lord of the island of Malindrania, vanquished in single combat by the never sufficiently extolled knight Don Quixote of La Mancha, who has commanded me to present myself before your Grace, that your Highness dispose of me at your pleasure'?\" Oh, how our good gentleman enjoyed the delivery of this speech, especially when he had thought of some one to call his Lady!";
            trackBar1.Value = (int) stdForDistance;
            trackBar1.Minimum = 0;
            trackBar1.Maximum = 20;
            trackBar1.TickFrequency = 1;
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
            tempList.Add(new rawTimes(e.KeyValue, keyDownTime, keyUpTime);
            keyRecords++;
            int index;
            keyPressTime = keyUpTime - keyDownTime;

            int tempKeyValue = -1;

            // Apparently, you can only get the uppercase value of a key in c#
            // passed in through KeyEventArgs.
            if (e.KeyValue >= 65 && e.KeyValue <= 90)
            {
                tempKeyValue = e.KeyValue + 32;
            }
            else
            {
                tempKeyValue = e.KeyValue;
            }

            // if/else for key press times
            // key exists in list, add press time to existing object's list
            if ((index = pressTimes.FindIndex(c => c.Key == tempKeyValue)) > -1)
            {
                // add time to that list
                pressTimes[index].KeyPress.Add(keyPressTime);

            }
            else // record for key does not exist. Create a new one.
            {
                // make a new object
                List<uint> temp = new List<uint>();
                temp.Add(keyPressTime);
                pressTimes.Add(new keyPressTimes(tempKeyValue, temp));
            }

            // if/else for key fly times. Make sure there is 2 keyRecords before collecting
            if (keyRecords > 1)
            {
                secondKey = tempKeyValue;
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
                firstKey = secondKey = tempKeyValue;
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

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
            this.stdBox.Text = this.getStdDev().ToString();
        }

        private double getStdDev()
        {
            int std = this.trackBar1.Value;

            double result = 0;

            result = std * 0.25;

            return result;
        }

        private void profileCheck_Click(object sender, EventArgs e)
        {
            //This is where the magic of checking the distance will happen. For now, nothing.
            double distance = 0; // <- this will be Mahalanobis(x, y);
            double distanceStd = getStdDev();
            int sizeCounter = 0;

            List<keyPressTimes> tempPressTimes = pressTimes;

            List<keyFlyTimes> tempFlyTimes = flyTimes;

            foreach (keyPressTimes temp in tempPressTimes)
            {
                if (temp.Key == 13)
                {
                    continue;
                }
                foreach (uint t in temp.KeyPress)
                {
                    sizeCounter++;
                }
            }


            Matrix collectedPressTimes = new Matrix(sizeCounter, 1);

            int i = 0;

            foreach (keyPressTimes temp in tempPressTimes)
            {
                if (i >= sizeCounter)
                {
                    break;
                }
                foreach (uint t in temp.KeyPress)
                {
                    collectedPressTimes.mat[i, 0] = t;
                    i++;
                }
            }

            double[] arrayPressTimes = profileReader.getKeyPressProfileData(tempPressTimes);
            Matrix profilePressTimes = new Matrix(arrayPressTimes.Length, 1);


            for (int j = 0; j < arrayPressTimes.Length; j++)
            {
                profilePressTimes.mat[j, 0] = arrayPressTimes[j];
            }


            Matrix testCollected = new Matrix(4, 1);

            testCollected.mat[0, 0] = 128.11;
            testCollected.mat[1, 0] = 128.11;
            testCollected.mat[2, 0] = 118.59;
            testCollected.mat[3, 0] = 120.13;

            //distance = Mahalanobis(profilePressTimes, collectedPressTimes);
            distance = Mahalanobis(profilePressTimes, testCollected);


            
            if (distance > distanceStd)
            {
                MessageBox.Show("You appear to be an intruder. Please leave. Your mahalanobis distance was: " + distance + getFacePalm(), "Ahhhh", MessageBoxButtons.OK);
            }

            else
            {
                MessageBox.Show("You are who you say you are...I trust you. Your mahalanobis distance was: " + distance, "Carry On", MessageBoxButtons.OK);
            }

            return;

        }

        private string getFacePalm()
        {
            string facepalm = @"
............................................________ 
....................................,.-'\""...................``~.,
.............................,.-\""...................................\""-.,
.........................,/...............................................\"":,
.....................,?......................................................,
.................../...........................................................,}
................./......................................................,:`^`..}
.............../...................................................,:\""........./
..............?.....__.........................................:`.........../
............./__.(.....\""~-,_..............................,:`........../
.........../(_....\""~,_........\""~,_....................,:`........_/
..........{.._$;_......\""=,_.......\""-,_.......,.-~-,},.~\"";/....}
...........((.....*~_.......\""=-._......\"";,,./`..../\""............../
...,,,___.`~,......\""~.,....................`.....}............../
............(....`=-,,.......`........................(......;_,,-\""
............/.`~,......`-...................................../
.............`~.*-,.....................................|,./.....,__
,,_..........}.>-._...................................|..............`=~-,
.....`=~-,__......`,.................................
...................`=~-,,.,...............................
................................`:,,...........................`..............__
.....................................`=-,...................,%`>--==``
........................................_..........._,-%.......`
..................................., ";

            return facepalm;

        }


    }
}
