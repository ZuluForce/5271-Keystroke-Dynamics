import os,sys
import re
import random
from math import sqrt

def mean_and_stdv(data):
    """ Given a list of <data> return a tuple with the (mean,stdv) of the
    numerical data in the list.
    """
    n, mean, stdv = len(data), 0, 0

    mean = sum(data) / float(n)

    if n <= 1:
        # Just wait for Mrs.Statistician to come along and correct me on the fact
        # that the stdv of a single value is techincally undefined. Can we let it
        # slide just this one time??
        return mean, 0

    for item in data:
        stdv = stdv + (item - mean)**2

    stdv = sqrt(stdv / float(n-1))

    return mean,stdv

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

    # Mean and std deviation for fly times
    for from_key, to_key in timeData['fly_times'].items():
        for to_key, times in to_key.items():
            times = str_to_int_list(times)
            mean, stdv = mean_and_stdv(times)

            reduced_profile['fly_times'][from_key] = {}
            reduced_profile['fly_times'][from_key][to_key] = mean
            reduced_profile['fly_times'][from_key]["{}_stdv".format(to_key)] = stdv
            
            stdv_list.append(stdv)

    # Mean and std deviation for press times
    for key, times in timeData['press_times'].items():
        times = str_to_int_list(times)
        mean, stdv = mean_and_stdv(times)

        reduced_profile['press_times'][key] = mean
        reduced_profile['press_times']["{}_stdv".format(key)] = stdv
        
        stdv_list.append(stdv)
        
    # Add the mean of all std deviations
    if len(stdv_list) != 0:
        (mean,stdv) = mean_and_stdv(times)
        reduced_profile['mean_stdv'] = mean
    else:
        print("No entires in profile. There is likely something wrong")
        reduced_profile['mean_std'] = 0

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

