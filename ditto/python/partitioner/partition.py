import os
import sys
import argparse

# Adds the parentdir to the python path so we can easily import other modules
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.sys.path.insert(0,parentdir) 

from all_partitioners import partitioners
import util.json_handler as js


def expectedJsonFormat(data):
    ok = True
    ok = ok if 'fly_times' in data else False
    ok = ok if 'press_times' in data else False
    return ok


def loadProfiles(*filepaths):
    profiles = {}

    for path in filepaths:
        fname = os.path.basename(path)
        saved_fname = fname

        # Make sure we have a unique identifier for this profile
        counter = 0
        while fname in profiles:
            fname = saved_fname + str(counter)

        # Load the profile's json data
        try:
            prof_data = js.load_json(path)
        except Exception as e:
            print("Failed to load json: {}".format(e))
            sys.exit(-1)

        # Verify it has already been reduced
        if not expectedJsonFormat(prof_data):
            print("Bad profile format: '{}'".format(path))
            print("The profile data likely hasn't been reduced yet")
            sys.exit(-1)

        profiles[fname] = prof_data

    return profiles


if __name__ == '__main__':
    pt_types = list(partitioners.keys())
    parser = argparse.ArgumentParser(description="Partion a set of profiles")
    parser.add_argument('profiles', metavar='P', type=str, nargs='+',
            help='profiles to partition')
    parser.add_argument('-pt', '--partitioner', type=str, choices=pt_types, default=pt_types[0],
            help="name of the partioner to use")
    parser.add_argument('-ptarg', action='append', help="arguments for the partitioner")

    args = parser.parse_args()

    partitioner = None
    try:
        partitioner = partitioners[args.partitioner]()
    except KeyError as e:
        print("Unknown partitioner '{}'".format(args.partitioner))
        sys.exit(-1)

    profiles = loadProfiles(*args.profiles)
    print(profiles)

    partitioner.partitionProfiles({},{})
