using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace masada
{
    class ProfileReader
    {
        private String profileDir = Directory.GetParent(Directory.GetCurrentDirectory()).Parent.FullName + "\\profiles\\";

        private String profileSuffix = "_reduced.json";

        // Maps a profile name to a profile
        private Dictionary<String, Profile> profiles = new Dictionary<String, Profile>();

        public ProfileReader()
        {
            // Get a list of profiles from the directory
            var files = from file in Directory.EnumerateFiles(profileDir)
                        where file.EndsWith(profileSuffix)
                        select System.IO.Path.GetFileName(file);
            foreach (var f in files)
            {
                String name = f.Substring(0, f.IndexOf(profileSuffix));
                profiles.Add(name, null);
            }
        }

        public List<String> getProfiles()
        {
            return profiles.Keys.ToList<String>();
        }

        public Profile loadProfile(String name)
        {
            Profile profile;

            if (profiles.TryGetValue(name, out profile))
            {
                if (profile == null)
                {
                    String line;
                    using (StreamReader sr = new StreamReader(profileDir + name + profileSuffix))
                    {
                        line = sr.ReadToEnd();
                    }
                    JObject json;
                    json = JObject.Parse(line);
                    profile = createProfile(json);
                }
                return profile;
            }
            else
            {
                return null; // Name was not in the dictionary
            }
        }


        private Profile createProfile(JObject json)
        {
            Profile profile = new Profile();
            for (int i = 0; i < Profile.NUM_PASSWORDS; i++)
            {
                int passwordNum = i + 1;
                String query = "pw" + passwordNum;
                JToken passwordTest = json.SelectToken(query);
                TestField testField = parseTestField(passwordTest);
                profile.passwordFields[i] = testField;
            }
            profile.textField = parseTestField(json.SelectToken("text"));
            return profile;
        }

        private TestField parseTestField(JToken token)
        {
            TestField testField = new TestField();
            testField.numFlyTimes = token.Value<int>("num_fly_times");
            testField.pressMean = token.Value<double>("press_mean");
            testField.pressStdv = token.Value<double>("press_stdv");
            testField.flyMean = token.Value<double>("fly_mean");
            testField.flyStdv = token.Value<double>("fly_stdv");
            testField.meanStdv = token.Value<double>("mean_stdv");
            JToken pressTimes = token.SelectToken("press_times");
            testField.pressTimes = createKeyTimeDictionary(pressTimes.Children().ToList<JToken>());
            JToken flyTimes = token.SelectToken("fly_times");
            Dictionary<int, Dictionary<int, Time>> dict = new Dictionary<int, Dictionary<int, Time>>();
            foreach (JToken flyTime in flyTimes)
            {
                var property = flyTime as JProperty;
                int key = Convert.ToInt32(property.Name);
                dict.Add(key, createKeyTimeDictionary(property.Value.Children().ToList<JToken>()));
            }


            return new TestField();
        }

        private Dictionary<int, Time> createKeyTimeDictionary(List<JToken> tokens)
        {
            Dictionary<int, Time> dict = new Dictionary<int, Time>();
            foreach (JToken child in tokens)
            {
                var property = child as JProperty;
                JValue value = property.Value as JValue;
                string name = property.Name;
                if (name.Contains("_stdv"))
                {
                    string strippedName = name.Substring(0, name.IndexOf("_stdv"));
                    int key = Convert.ToInt32(strippedName);
                    Time time;
                    if (dict.TryGetValue(key, out time))
                    {
                        // Time is already in the dict, update its stdev
                        time.stdev = Convert.ToDouble(value.Value);
                    }
                    else // Create a new time object and put it in the dictionary
                    {
                        time = new Time();
                        time.stdev = Convert.ToDouble(value.Value);
                        dict.Add(key, time);
                    }
                }
                else // is a time property
                {
                    int key = Convert.ToInt32(name);
                    Time time;
                    if (dict.TryGetValue(key, out time))
                    {
                        // Time is already in the dict, update its time
                        time.time = Convert.ToDouble(value.Value);
                    }
                    else // Create a new time object and put it in the dictionary
                    {
                        time = new Time();
                        time.time = Convert.ToDouble(value.Value);
                        dict.Add(key, time);
                    }
                }

            }
            return dict;

        }
    }
}
