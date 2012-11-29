'''
Created on Nov 29, 2012

@author: Planeman
'''
import os
import sys

# Adds the parentdir to the python path so we can easily import other modules
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.sys.path.insert(0,parentdir) 

import util.json_handler as js
from mappings.scan_code_maps import *
from ditto_types import *
from profile_compare import compareProfiles

def compareDittoAndJson(ditto, profile):
    # First read in the ditto dprofile and create a python
    # dictionary
    ditto_as_map = {'fly_times':{}, 'press_times':{}}
    
    unitSize = sizeofStruct(FSProfileStruct)

    with open(ditto, "rb") as dprofile:
        sBytes = dprofile.read(unitSize)
        total_fly_times = []
        total_press_times = []
        while sBytes:
            struct = buildStructFromBytes(FSProfileStruct(), sBytes)
            if struct.time_type == FSProfileTimeType.FLY_TIME:
                from_key = JSCodeFromDittoSC(struct.from_key)
                to_key = JSCodeFromDittoSC(struct.to_key)
                if from_key not in ditto_as_map['fly_times']:
                    ditto_as_map['fly_times'][from_key] = {}

                ditto_as_map['fly_times'][from_key][to_key] = struct.time_in_ms
            elif struct.time_type == FSProfileTimeType.PRESS_TIME:
                key = JSCodeFromDittoSC(struct.from_key)
                ditto_as_map['press_times'][key] = struct.time_in_ms
            else:
                print("!! ---- Unrecognized FSProfileTimeType ( {} ) ---- !!".format(struct.time_type))

            sBytes = dprofile.read(unitSize)
                
    # Load the regular dprofile
    profile_data = js.load_json(profile)
    if profile_data is None:
        print("Failed to load dprofile " + profile)
        sys.exit(2)
        
    # Do the comparison
    compareProfiles(ditto_as_map, profile_data, timeToInt=True)
    
    
if __name__ == '__main__':
    if len(sys.argv) != 3 or not sys.argv[1].endswith(".ditto"):
        print("Usage: {} [.ditto file] [json reduced profile]".format(sys.argv[0]))
        sys.exit(1)
        
    compareDittoAndJson(sys.argv[1],sys.argv[2])