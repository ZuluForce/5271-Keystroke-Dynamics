'''
Created on Nov 26, 2012

@author: Planeman
'''

import os
import sys

parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0,parentdir)
import util.json_handler as js

def printReducedProfileInfo(*profiles):
    for profile in profiles:
        if "user_map.json" in profile:
            continue

        all_data = js.load_json(profile)

        pdata = all_data
        wpm = 0
        if 'text' in all_data:
            if 'wpm' in pdata:
                wpm = pdata['wpm']

            pdata = all_data['text']
        
        fly_total_float = 0.0
        fly_total = num_fly = 0
        max_fly = -1
        (max_fly_from, max_fly_to) = (-1,-1)
        for from_key, to_keys in pdata['fly_times'].items():
            for to_key, time in to_keys.items():
                #if type(to_key) in (str,unicode) and to_key.endswith("_stdv"):
                #    continue
                if time > max_fly:
                    max_fly = time
                    max_fly_from = from_key
                    max_fly_to = to_key

                num_fly += 1
                fly_total_float += time
                fly_total += int(time)

                
        
        print("\n=======================\n")
        print("Profile Name: {}".format(profile))
        if 'username' in all_data and all_data['username'] is not None:
            print("User name: {}".format(all_data['username']))

        print("# collected fly: {} (unique pairs)".format(pdata['num_fly_times']))
        print("# collected press: {} (unique keys)".format(pdata['num_press_times']))
        print("# filtered fly: {} (range: {})".format(pdata['filtered_fly'], pdata['irq_range_fly']))
        print("# filtered press: {} (range: {})".format(pdata['filtered_press'], pdata['irq_range_press']))
        print("Mean fly time: {} ms".format(pdata['fly_mean']))
        print("Standard Dev. fly time: {} ms".format(pdata['fly_stdv']))
        print("Mean press time: {} ms".format(pdata['press_mean']))
        print("Standard Dev. press time: {} ms".format(pdata['press_stdv']))
        if wpm > 0:
            print("Words Per Minute: {}".format(wpm))
        print("Max fly time from {} -> {} = {} ms".format(max_fly_from, max_fly_to, max_fly))
        print("Recalculated mean fly: {} ms".format(fly_total / num_fly))
        print("Recalculated mean fly (float): {} ms".format(fly_total_float / num_fly))

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: {} [profile]+")
        
    printReducedProfileInfo(*sys.argv[1:])