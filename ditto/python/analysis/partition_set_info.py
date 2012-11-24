'''
Created on Nov 24, 2012

@author: planeman
'''
import os
import sys

parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0,parentdir)
import util.json_handler as js

def printPartitionSetInfo(partitionDir):
    """
    Given a directory containing all the json for a partition this function will print it out.
    """
    partitions = []
    partition_to_users = {}
    
    for fname in os.listdir(partitionDir):
        (filebase, ext) = fname.rsplit('.',1)
        
        if ext == 'ditto':
            continue
        
        if fname == "user_map.json":
            data = js.load_json(os.path.join(partitionDir, fname))
            
            for user,partition in data.items():
                if partition not in partition_to_users:
                    partition_to_users[partition] = []
                    
                partition_to_users[partition].append(user)
                
            continue
        
        # Load the json
        fname = os.path.join(partitionDir,fname)
        try:
            data = js.load_json(fname)
        except Exception as e:
            print("Failed to load json: {}".format(fname))
            sys.exit(-1)
        
        pnum = int(filebase)
        if len(partitions) <= pnum:
            partitions += [None] * (pnum - len(partitions) + 1)
        
        partitions[pnum] = data
        
    # Now print all of them
    for index,partition in enumerate(partitions):
        if partition is None:
            continue

        print("Partition Number {}".format(index))
        print("\tSource profile: {}".format(partition['source']))
        print("\tFly time #: {}".format(len(partition['fly_times'])))
        print("\tPress time #: {}". format(len(partition['press_times'])))
        print("\tWPM: {}".format(partition['wpm']))
        print("\tUsers in partition: {}".format(partition_to_users[index]))

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: {} [partition directory]")
        sys.exit(-1)
        
    printPartitionSetInfo(sys.argv[1])