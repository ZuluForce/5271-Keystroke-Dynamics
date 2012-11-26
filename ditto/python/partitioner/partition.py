import os
import re
import sys
import errno
import shutil
import argparse

# Adds the parentdir to the python path so we can easily import other modules
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.sys.path.insert(0,parentdir) 

from all_partitioners import partitioners
import util.json_handler as js


def expectedJsonFormat(data):  
    # Check for pw fields
    for i in range(1,21):
        pw_field = "pw" + str(i)
        if pw_field not in data:
            print("Missing {} key in profile data".format(pw_field))
            return False
        
        if 'fly_times' not in data[pw_field]:
            print("Missing fly_times for {}".format(pw_field))
            return False
        
        if 'press_times' not in data[pw_field]:
            print("Missing press_times for {}".format(pw_field))
            return False
    
    # Check the paragraph text field data
    if 'text' not in data:
        print("Missing text key in profile data")
        return False
    
    if 'fly_times' not in data['text']:
        print("Missing fly_times for main text data")
        return False
    
    if 'press_times' not in data['text']:
        print("Missing press_times for main text data")
        return False
    
    if 'wpm' not in data:
        print("Missing wpm for profile")
        return False
    
    return True


def loadProfiles(*filepaths):
    profiles = {}

    user_name_re = re.compile("(.+?)_reduced.json")

    for path in filepaths:
        fname = os.path.basename(path)

        # Load the profile's json data
        try:
            prof_data = js.load_json(path)
        except Exception as e:
            print("Failed to load json: {}".format(e))
            sys.exit(-1)

        # Verify it has already been reduced
        print("Doing json format pre-check on {}".format(path))
        if not expectedJsonFormat(prof_data):
            print("Bad profile format: '{}'".format(path))
            print("The profile data likely hasn't been reduced yet")
            sys.exit(1)

        if 'prof_name' in prof_data:
            fname = prof_data['prof_name']
        else:
            m = user_name_re.search(fname)
            if m is None:
                print("Cannot create internal name for profile. Either use the format of the reducer or provide a 'prof_name' key in the profile")
                sys.exit(1)
            
            fname = m.group(1)
            
        profiles[fname] = prof_data

    return profiles


if __name__ == '__main__':
    pt_types = list(partitioners.keys())
    parser = argparse.ArgumentParser(description="Partion a set of profiles")
    parser.add_argument('profiles', metavar='P', type=str, nargs='+',
            help='profiles to partition')
    parser.add_argument('-pt', '--partitioner', type=str, choices=pt_types, default=pt_types[0],
            help="name of the partioner to use")
    parser.add_argument('-o','--outdir', type=str, required=True, help="Output directory for partitioning information")
    parser.add_argument('--pname', type=str, required=True, help="The name of the set of partitions produced. A directory by this name will be created in outdir.")
    parser.add_argument('-ptarg', action='append', help="arguments for the partitioner")

    args = parser.parse_args()

    partitioner = None
    try:
        partitioner = partitioners[args.partitioner]()
    except KeyError as e:
        print("Unknown partitioner '{}'".format(args.partitioner))
        sys.exit(1)

    profiles = loadProfiles(*args.profiles)

    (partitions, user_to_partition) = partitioner.partitionProfiles(profiles)
    
    print("Users -> partitions:\n{}".format(user_to_partition))
    
    try:
        outdir = os.path.join(args.outdir, args.pname);
        os.makedirs(outdir)
    except os.error as e:
        if e.errno != errno.EEXIST:
            print("makedirs failed on: {}".format(outdir))
            print("error: {}".format(e))
        else:
            print("Removing previous partition output directory: {}".format(outdir))
            shutil.rmtree(outdir)
            os.makedirs(outdir)
    
    print("Writing partition information to {}".format(outdir))
    # Write all partition profiles to disk
    for pnum, profile in partitions.items():
        fname = os.path.join(outdir, str(pnum) + ".json")
        js.write_json(profile, filename=fname)
        
    user_map = os.path.join(outdir, 'user_map.json')
    js.write_json(user_to_partition, filename=user_map)
