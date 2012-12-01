import os,sys
import re
import time
import random

parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0,parentdir)
from mappings.scan_code_maps import JS_BACKSPACE 
from math import sqrt
from scipy.stats import scoreatpercentile, percentileofscore
from numpy import array

# When filling the fly times we need to know the highest possible
# key value so we can iterate all pairs
lowest_key_code=8
highest_key_code=90

max_key_time=2000  # 2 seconds

def mean_and_stdv(data):
    """ Given a list of <data> return a tuple with the (mean,stdv) of the
    numerical data in the list.
    """
    n, mean, stdv = len(data), 0, 0

    mean = sum(data) / float(n)

    if n <= 1:
        # Just wait for Mrs.Statistician to come along and correct me on the fact
        # that the stdv of a single value is technically undefined. Can we let it
        # slide just this one time??
        return mean, 0

    for item in data:
        stdv = stdv + (item - mean)**2

    stdv = sqrt(stdv / float(n-1))

    return mean,stdv


def filterOutliers(times, irq_range=.5, rtn_range=False):
    # We need the original indexes for marking which were removed
    original_times = list(times)

    times.sort()
    np_times = array(times)
    q1 = scoreatpercentile(np_times, 25, interpolation_method='fraction')
    q3 = scoreatpercentile(np_times, 75, interpolation_method='fraction')
    irq = q3-q1
    slack = irq_range * irq

    okRange = (q1-slack, q3+slack)

    newTimes = []
    rm_indexes = []
    for index,time in enumerate(original_times):
        # If there is only one element in the list it may not be an outlier but
        # but it could still be erroneous
        if time > max_key_time:
            rm_indexes.append(index)
            continue

        if okRange[0] <= time <= okRange[1]:
            newTimes.append(time)
        else:
            rm_indexes.append(index)

    if rtn_range:
        return newTimes, rm_indexes, okRange
    else:
        return newTimes, rm_indexes

def str_to_int_list(l):
    new_list = []
    for item in l:
        new_list.append(int(item))

    return new_list

def std_reduce(user_data):
    """
    Take in the data for a raw profile and reduce the data to single values. All
    fly and press times will have their average computed and put in place of the
    previous list of values. Fly and press times will also have a stdv added with
    a key generated as follows:
    For fly times (from_key -> to_key) the stdv will be placed under the new key:
        reduced_profile['fly_times'][from_key][to_key_stdv]

    For press times you can find the stdv under this new key:
        reduced_profile['press_times'][key_stdv]

    Args:
        user_data - A complete set of user json data.
        out_dir - The name of the output directory for the reduced profile.

    Return:
        A reduced profile as specified above as a python dictionary. It will also be
        written to <out_file> as a json dump of the resulting python dictionary.
    """
    reduced_profile = {}

    wpm = 0

    random.seed(time.time())
    for section,data in user_data.items():
        print("Section: {}".format(section))
        if section == "text":
            wpm = calculateWPM(data)
        reduced_times = reduceTimes(data)
        reduced_profile[section] = reduced_times

    # Add a placeholder for the wpm
    # Note: the partitioner depends on this key being called 'wpm' so once this is actually
    #     implemented be sure to use the same key
    reduced_profile['wpm'] = wpm #random.randint(0,80)

    # Fill in missing fly and press times
    fillMissingTimes(reduced_profile)

    return reduced_profile

def reduceTimes(timeData):
    """
    Takes in times as a dictionary with two keys, fly_times and press_times. It outputs
    a dictionary with reduced times.
    """
    reduced_profile = {'fly_times':{}, 'press_times':{}}
    #print(timeData)

    # This is a running sum of the std deviation for each entry. The mean of std deviations will
    # help us in partitioning to decide between profiles with nearly identical wpm
    stdv_list = []

    # This data is used in filling in gaps later on where we are missing fly or press times
    all_fly_times = []
    all_press_times = []
    
    # When we filter later we need to know which keys to remove from the profile
    fly_time_pairs = []
    press_time_keys = []

    # Mean and std deviation for fly times
    print("Processing raw fly times")
    for from_key, to_keys in timeData['fly_times'].items():
        from_key = int(from_key)
        reduced_profile['fly_times'][from_key] = {}

        for to_key, times in to_keys.items():
            to_key = int(to_key)

            times = str_to_int_list(times)
            times,removed = filterOutliers(times)

            if len(times) == 0:
                continue

            mean, stdv = mean_and_stdv(times)

            reduced_profile['fly_times'][from_key][to_key] = mean

            stdv_list.append(stdv)
            all_fly_times.append(mean)
            fly_time_pairs.append((from_key, to_key))

    # Mean and std deviation for press times
    print("Processing raw press times")
    for key, times in timeData['press_times'].items():
        key = int(key)

        times = str_to_int_list(times)
        times,removed = filterOutliers(times)
        if len(times) == 0:
            continue

        mean, stdv = mean_and_stdv(times)

        reduced_profile['press_times'][key] = mean

        stdv_list.append(stdv)
        all_press_times.append(mean)
        press_time_keys.append(key)

    ## Add the overall data on press and fly times
    all_press_times,removed_press,rng_press = filterOutliers(all_press_times, irq_range=.4, rtn_range=True)
    all_fly_times,removed_fly,rng_fly = filterOutliers(all_fly_times, irq_range=.4, rtn_range=True)
    
    reduced_profile['filtered_fly'] = len(removed_fly)
    reduced_profile['filtered_press'] = len(removed_press)
    reduced_profile['irq_range_fly'] = rng_fly
    reduced_profile['irq_range_press'] = rng_press
    
    for index in removed_fly:
        keyPair = fly_time_pairs[index]
        from_key = keyPair[0]
        to_key = keyPair[1]

        del reduced_profile['fly_times'][from_key][to_key]
        
    for index in removed_press:
        key = press_time_keys[index]
        
        del reduced_profile['press_times'][key]

    mean, stdv = mean_and_stdv(all_press_times)
    reduced_profile['press_mean'] = mean
    reduced_profile['press_stdv'] = stdv

    mean, stdv = mean_and_stdv(all_fly_times)
    reduced_profile['fly_mean'] = mean
    reduced_profile['fly_stdv'] = stdv
    
    count = 0
    for k,v in reduced_profile['fly_times'].items():
        for k2,v2 in v.items():
            count += 1

    if count != len(all_fly_times):
        print("Post reduction fly time count is not as expected (actual: {}, expected: {})".format(count,len(all_fly_times)))
        sys.exit(2)

    reduced_profile['num_fly_times'] = len(all_fly_times)
    reduced_profile['num_press_times'] = len(all_press_times)

    return reduced_profile

def calculateWPM(data):
    totalTime = 0
    numWords = 212
    for from_key, to_keys in data['fly_times'].items():
        from_key = int(from_key)
        for to_key, times in to_keys.items():
            to_key = int(to_key)

            times = str_to_int_list(times)
            times,removed = filterOutliers(times, irq_range=1.5)
            for time in times:
                if to_key == JS_BACKSPACE:
                    totalTime += 3*time
                else:
                    totalTime += time
    return numWords / (totalTime / 60000.0) 

def removePWFields(data):
    if 'text' not in data:
        print("removePWFields: Input data not in correct format. Should have 'text' key")

    strippedData = {}

    pw_re = re.compile("pw[0-9]+")
    for key,value in data.items():
        m = pw_re.match(key)
        if m is not None:
            continue

        if key == 'text':
            strippedData['press_times'] = data['text']['press_times']
            strippedData['fly_times'] = data['text']['fly_times']
        else:
            strippedData[key] = value

    return strippedData

def fillMissingTimes(profile):

    # We only do the filling for the text part of the profile
    profile = profile['text']
    #print profile
    #print("Fly times:\n" + str(profile['fly_times']))
    #sys.exit(2)

    mean_press = profile['press_mean']
    stdv_press = profile['press_stdv']
    mean_fly = profile['fly_mean']
    stdv_fly = profile['fly_stdv']

    addedFlyTimes = 0
    addedPressTimes = 0

    # Fill missing fly times
    for keyPair in generateFlyPairs(lowest_key_code, highest_key_code):
        time = getFlyTime(profile['fly_times'], keyPair[0], keyPair[1])

        if time is None:
            # Then the fly time for that pair is missing
            addedFlyTimes += 1

            variance = -0.5 + random.random()
            time = mean_fly + (variance * stdv_fly)
            setFlyTime(profile['fly_times'], keyPair[0], keyPair[1], time)

    # Fill missing press times
    for key in range(lowest_key_code,highest_key_code+1):
        time = getPressTime(profile['press_times'], key)
        if time is None:
            addedPressTimes += 1

            variance = -1 + (random.random() * 2)
            time = mean_press + (variance * stdv_press)
            setPressTime(profile['press_times'], key, time)


    print("Added {} missing fly times".format(addedFlyTimes))
    print("Added {} missing press times".format(addedPressTimes))

    num_key_codes = (highest_key_code - lowest_key_code)
    total_combos = pow(num_key_codes, 2)
    if addedFlyTimes == total_combos or addedPressTimes == num_key_codes:
        print("Every single possible fly and/or press time entry was filled as if missing")
        print("Something is likely wrong or the profile is empty")
        sys.exit(2)

def getFlyTime(flyTimeMap, fromKey, toKey):
    if fromKey not in flyTimeMap:
        return None

    toKeyMap = flyTimeMap[fromKey]
    if toKey not in toKeyMap:
        return None

    return toKeyMap[toKey]

def setFlyTime(flyTimeMap, fromKey, toKey, time):
    if fromKey not in flyTimeMap:
        flyTimeMap[fromKey] = {}

    flyTimeMap[fromKey][toKey] = time

def getPressTime(pressTimeMap, key):
    if key not in pressTimeMap:
        return None

    return pressTimeMap[key]

def setPressTime(pressTimeMap, key, time):
    pressTimeMap[key] = time

def generateFlyPairs(start, end):
    """
    This will iterate through all pairs of integers (keys) that are within the given
    keySpaceSize. If keySpaceSize=100 this generator will produce the pairs from
    (0,0) -> (100,100) and every combination in between. Note: (10,2) != (2,10)
    """
    for from_key in range(start,end+1):
        for to_key in range(start,end+1):
            yield (from_key,to_key)

