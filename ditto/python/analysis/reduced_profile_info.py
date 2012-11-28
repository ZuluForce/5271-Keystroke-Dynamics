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
        all_data = js.load_json(profile)

        pdata = all_data
        if 'text' in all_data:
            pdata = all_data['text']
        
        print("\n=======================\n")
        print("Profile Name: {}".format(profile))
        if 'username' in all_data and all_data['username'] is not None:
            print("User name: {}".format(all_data['username']))

        print("# collected fly: {} (unique pairs)".format(pdata['num_fly_times']))
        print("# collected press: {} (unique keys)".format(pdata['num_press_times']))
        print("Mean fly time: {} ms".format(pdata['fly_mean']))
        print("Standard Dev. fly time: {} ms".format(pdata['fly_stdv']))
        print("Mean press time: {} ms".format(pdata['press_mean']))
        print("Standard Dev. press time: {} ms".format(pdata['press_stdv']))
        print("Overall profile variance: {}".format(pdata['mean_stdv']))

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: {} [profile]+")
        
    printReducedProfileInfo(*sys.argv[1:])