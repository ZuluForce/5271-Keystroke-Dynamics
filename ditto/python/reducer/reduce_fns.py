import os,sys
import re
import random
from math import sqrt, ceil

# When filling the fly times we need to know the highest possible
# key value so we can iterate all pairs
highest_key_code=220

max_press_time=3  # seconds
max_fly_time=4  # seconds

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


def filterOutliers(times):
    (median, q1, q3) = findQuartiles(times)
    irq = q3-q1
    slack = 1.5 * irq
    
    okRange = (q1-slack, q3+slack)
    
    newTimes = []
    for time in times:
        if okRange[0] <= time <= okRange[1]:
            newTimes.append(time)

    return newTimes

def findQuartiles(times):
    if len(times) == 1:
        return (times[0],0,times[0])

    (median, low, high) = findMedian(times)
    
    (q1, q1_low, q1_high) = findMedian(low)
    (q3, q3_low, q3_high) = findMedian(high)
    
    return (median, q1, q3)
    
def findMedian(times):
    if len(times) == 0:
        return (0,0,0)

    if len(times) == 1:
        return (times[0], 0,times[0])

    times.sort()
    
    m_index_low = m_index_high = -1
    low_half = high_half = None
    median = 0
    
    # Find the median value first
    if len(times) % 2 == 0:
        m_index_low = (len(times) / 2) - 1
        m_index_high = m_index_low + 1
        median = (times[m_index_low] + times[m_index_high]) / 2
        low_half = times[:m_index_low]
        high_half = times[m_index_high:]
    else:
        m_index_low = m_index_high = len(times) / 2
        median = times[m_index_low]
        low_half = times[:m_index_low]
        high_half = times[m_index_low+1:]
        
    #print("(median, low, high) = ({},{},{})".format(median, low_half, high_half))
    return (median, low_half, high_half)
        
    

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

    for section,data in user_data.items():
        print("Section: {}".format(section))
        reduced_times = reduceTimes(data)
        reduced_profile[section] = reduced_times
        
    # Add a placeholder for the wpm
    # Note: the reducer depends on this key being called 'wpm' so once this is actually
    #     implemented be sure to use the same key
    reduced_profile['wpm'] = random.randint(0,80)
    
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

    # Mean and std deviation for fly times
    for from_key, to_key in timeData['fly_times'].items():
        from_key = int(from_key)
        reduced_profile['fly_times'][from_key] = {}
        
        for to_key, times in to_key.items():
            to_key = int(to_key)
            
            times = str_to_int_list(times)
            times = filterOutliers(times)
            mean, stdv = mean_and_stdv(times)

            reduced_profile['fly_times'][from_key][to_key] = mean
            reduced_profile['fly_times'][from_key]["{}_stdv".format(to_key)] = stdv
            
            stdv_list.append(stdv)
            all_fly_times.append(mean)

    # Mean and std deviation for press times
    for key, times in timeData['press_times'].items():
        key = int(key)

        times = str_to_int_list(times)
        times = filterOutliers(times)
        mean, stdv = mean_and_stdv(times)

        reduced_profile['press_times'][key] = mean
        reduced_profile['press_times']["{}_stdv".format(key)] = stdv
        
        stdv_list.append(stdv)
        all_press_times.append(mean)

    # Add the mean of all std deviations
    if len(stdv_list) != 0:
        (mean,stdv) = mean_and_stdv(times)
        reduced_profile['mean_stdv'] = mean
    else:
        print("No entires in profile. There is likely something wrong")
        reduced_profile['mean_stdv'] = 0
        
    ## Add the overall data on press and fly times
    all_press_times = filterOutliers(all_press_times)
    all_fly_times = filterOutliers(all_fly_times)
    
    mean, stdv = mean_and_stdv(all_press_times)
    reduced_profile['press_mean'] = mean
    reduced_profile['press_stdv'] = stdv
    
    mean, stdv = mean_and_stdv(all_fly_times)
    reduced_profile['fly_mean'] = mean
    reduced_profile['fly_stdv'] = stdv

    return reduced_profile

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
    
    mean_press = profile['press_mean']
    stdv_press = profile['press_stdv']
    mean_fly = profile['fly_mean']
    stdv_fly = profile['fly_stdv']
    
    random.seed()
    
    addedFlyTimes = 0
    addedPressTimes = 0

    # Fill missing fly times
    for keyPair in generateFlyPairs(highest_key_code):
        time = getFlyTime(profile['fly_times'], keyPair[0], keyPair[1])
 
        if time is None:
            # Then the fly time for that pair is missing
            addedFlyTimes += 1
            
            variance = -1 + (random.random() * 2)
            time = mean_fly + (variance * stdv_fly)
            setFlyTime(profile['fly_times'], keyPair[0], keyPair[1], time)
     
    # Fill missing press times
    for key in range(highest_key_code):
        time = getPressTime(profile['press_times'], key)
        if time is None:
            addedPressTimes += 1
            
            variance = -1 + (random.random() * 2)
            time = mean_press + (variance * stdv_press)
            setPressTime(profile['press_times'], key, time)
        
    
    print("Added {} missing fly times".format(addedFlyTimes))
    print("Added {} missing press times".format(addedPressTimes))
    
    total_combos = pow(highest_key_code, 2)
    if addedFlyTimes == total_combos or addedPressTimes == highest_key_code:
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

def generateFlyPairs(keySpaceSize):
    """
    This will iterate through all pairs of integers (keys) that are within the given
    keySpaceSize. If keySpaceSize=100 this generator will produce the pairs from
    (0,0) -> (100,100) and every combination in between. Note: (10,2) != (2,10)
    """
    for from_key in range(keySpaceSize):
        for to_key in range(keySpaceSize):
            yield (from_key,to_key)

