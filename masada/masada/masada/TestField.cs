using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace masada
{
    class TestField
    {
        public int numFlyTimes { get; set; }
        public int numPressTimes { get; set; }
        public double pressMean { get; set; }
        public double pressStdv { get; set; }
        public double flyMean { get; set; }
        public double flyStdv { get; set; }
        public double meanStdv { get; set; }
        public Dictionary<int, Time> pressTimes = new Dictionary<int, Time>();
        public Dictionary<int, Dictionary<int, Time>> flyTimes = new Dictionary<int, Dictionary<int, Time>>();
    }
}
