import os
import sys

# Adds the parentdir to the python path so we can easily import other modules
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.sys.path.insert(0,parentdir) 

from mappings.scan_code_maps import *
from ditto_types import *

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: {} [profile]".format(sys.argv[0]))
        sys.exit(1)
    
    print_all_times = True    
    if sys.argv[1] == "short":
        print_all_times = False
        sys.argv.remove("short")

    unitSize = sizeofStruct(FSProfileStruct)

    for profile_name in sys.argv[1:]:
        with open(profile_name, "rb") as profile:
            sBytes = profile.read(unitSize)
            total_fly_times = []
            total_press_times = []
            max_fly, max_fly_from, max_fly_to = 0,0,0
            
            while sBytes:
                struct = buildStructFromBytes(FSProfileStruct(), sBytes)
                if struct.time_type == FSProfileTimeType.FLY_TIME:
                    time = struct.time_in_ms
                    from_key = JSCodeFromDittoSC(struct.from_key)
                    to_key = JSCodeFromDittoSC(struct.to_key)
                    if print_all_times:
                        print(" {:^2} -> {:^2} : {} ms".format(from_key, to_key, time))
                        
                    if time > max_fly:
                        max_fly = time
                        max_fly_from = struct.from_key
                        max_fly_to = struct.to_key

                    total_fly_times.append(struct.time_in_ms)
                elif struct.time_type == FSProfileTimeType.PRESS_TIME:
                    if print_all_times:
                        print(" {0.from_key:^2} |> {0.to_key:^2} : {0.time_in_ms} ms".format(struct))
                    total_press_times.append(struct.time_in_ms)
                else:
                    print("!! ---- Unrecognized FSProfileTimeType ( {} ) ---- !!".format(struct.time_type))
    
                sBytes = profile.read(unitSize)
                
            if len(total_fly_times) > 0:
                print("# fly times: {}".format(len(total_fly_times)))
                print("Average fly time ({}): {} ms".format(profile_name, sum(total_fly_times) / len(total_fly_times)))
            
            if len(total_press_times) > 0:
                print("# press times: {}".format(len(total_press_times)))
                print("Average press time ({}): {} ms".format(profile_name, sum(total_press_times) / len(total_press_times)))
                
            max_fly_from = JSCodeFromDittoSC(max_fly_from)
            max_fly_to = JSCodeFromDittoSC(max_fly_to)
            print("Max fly time from {} -> {}: {} ms".format(max_fly_from, max_fly_to, max_fly))

