'''
Created on Nov 29, 2012

@author: Planeman
'''
import os,sys

parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0,parentdir)
import util.json_handler as js

def loadAndCompareProfiles(prof1, prof2):
    prof1_data = js.load_json(prof1)
    if prof1_data is None:
        print("Failed to load profile: {}".format(prof1))

    prof2_data = js.load_json(prof2)
    if prof2_data is None:
        print("Failed to load profile: {}".format(prof2))

def compareProfiles(prof1_data, prof2_data, timeToInt=False):
    if 'text' in prof1_data:
        prof1_data = prof1_data['text']
        
    if 'text' in prof2_data:
        prof2_data = prof2_data['text']
        
    prof1_fly = prof1_data['fly_times']
    prof1_press = prof1_data['press_times']
    
    prof2_fly = prof2_data['fly_times']
    prof2_press = prof2_data['press_times']
    
    # Compare fly times
    for from_key, times in prof1_fly.items():
        from_key_str = str(from_key)
        from_key_int = int(from_key)

        for to_key, time in times.items():
            if type(to_key) == str and to_key.endswith("_stdv"):
                continue
            
            to_key_str = str(to_key)
            to_key_int = int(to_key)

            if from_key_str in prof2_fly:
                from_key = from_key_str
            elif from_key_int in prof2_fly:
                from_key = from_key_int
            else:
                print("Missing from_key {}".format(from_key))
                break
                
            if to_key_str in prof2_fly[from_key]:
                to_key = to_key_str 
            elif to_key_int in prof2_fly[from_key]:
                to_key = to_key_int
            else:
                print("Missing to_key {} for from_key {}".format(from_key, to_key))
                continue

            prof2_time = prof2_fly[from_key][to_key]
            if timeToInt:
                time = int(time)
                prof2_time = int(prof2_time)

            if prof2_time != time:
                print("flytime {} -> {} not equal (1:{}, 2:{})".format(from_key,to_key,time,prof2_time))
                
    for key, time in prof1_press.items():
        if type(key) == str and key.endswith("_stdv"):
            continue

        key_str = str(key)
        key_int = int(key)
        if key_str in prof2_press:
            key = key_str
        elif key_int in prof2_press:
            key = key_int
        else:
            print("Missing press key {}".format(key))
            continue

        prof2_time = prof2_press[key]
        if timeToInt:
            time = int(time)
            prof2_time = int(prof2_time)

        if time != prof2_time :
            print("presstime {} not equal (1:{}, 2:{})".format(key, time, prof2_time))
            
    print("Finished comparing profiles")
                
                
if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: {} [profile1] [profile2]".format(sys.argv[0]))
        sys.exit(2)
        
    compareProfiles(sys.argv[1], sys.argv[2])
        