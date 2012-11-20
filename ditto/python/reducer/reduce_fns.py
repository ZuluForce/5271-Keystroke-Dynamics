import os,sys
import re
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

    return reduced_profile

def reduceTimes(timeData):
    """
    Takes in times as a dictionary with two keys, fly_times and press_times. It outputs
    a dictionary with reduced times.
    """
    reduced_profile = {'fly_times':{}, 'press_times':{}}
    print(timeData)
    for from_key, to_key in timeData['fly_times'].items():
        for to_key, times in to_key.items():
            times = str_to_int_list(times)
            mean, stdv = mean_and_stdv(times)

            reduced_profile['fly_times'][from_key] = {}
            reduced_profile['fly_times'][from_key][to_key] = mean
            reduced_profile['fly_times'][from_key]["{}_stdv".format(to_key)] = stdv

    for key, times in timeData['press_times'].items():
        times = str_to_int_list(times)
        mean, stdv = mean_and_stdv(times)

        reduced_profile['press_times'][key] = mean
        reduced_profile['press_times']["{}_stdv".format(key)] = stdv

    return reduced_profile

