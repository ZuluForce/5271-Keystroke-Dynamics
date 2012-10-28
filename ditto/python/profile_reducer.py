import sys
from math import sqrt

from util import json_handler


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


def std_reduce(fly_file, press_file, out_file):
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
        fly_file - Name of the file containing the json data for the raw profile's
                fly times.
        press_file - Name of the file containing the json data for the raw profile's
                press times.
        out_file - The name of the output file for the reduced profile.

    Return:
        A reduced profile as specified above as a python dictionary. It will also be
        written to <out_file> as a json dump of the resulting python dictionary.
    """
    raw_fly_profile = json_handler.load_json(fly_file)
    raw_press_profile = json_handler.load_json(press_file)
    reduced_profile = {'fly_times':{}, 'press_times':{}}

    for from_key, to_key in raw_fly_profile.items():
        for to_key, times in to_key.items():
            mean, stdv = mean_and_stdv(times)

            reduced_profile['fly_times'][from_key] = {}  # Add the new key:dictionary
            reduced_profile['fly_times'][from_key][to_key] = mean
            reduced_profile['fly_times'][from_key]["{}_stdv".format(to_key)] = stdv

    for key, times in raw_press_profile.items():
        mean, stdv = mean_and_stdv(times)

        reduced_profile['press_times'][key] = mean
        reduced_profile['press_times']["{}_stdv".format(key)] = stdv

    with open(out_file, "w") as f:
        json_handler.write_json(reduced_profile, fd=f)

    return reduced_profile


if __name__ == '__main__':
    # Note that the standalone program functionality provided below is merely for testing
    # and it is suspected that when actually used this will be imported elsewhere and
    # the reduce method used

    if len(sys.argv) < 2:
        print("Usage: {} [profile prefix]+".format(sys.argv[0]))
        print("Using the prefix this program will look for <prefix>_fly.json and <prefix>_press.json")
        print("Reduced profile/s will be outputed with 'reduced.json' appended to the filename")
        sys.exit(1)

    for prefix in sys.argv[1:]:
        fly_data = "{}_fly.json".format(prefix)
        press_data = "{}_press.json".format(prefix)
        out_file = "{}_reduced.json".format(prefix)

        print("std_reduce({},{},{})".format(fly_data, press_data, out_file))
        std_reduce(fly_data, press_data, out_file)
