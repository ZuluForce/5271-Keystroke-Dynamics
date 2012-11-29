import re
import sys
import argparse

from util.json_handler import load_json
from mappings.scan_code_maps import *
from ditto_types import *

ignore_umapped=False

def unmapped_key_warn(key):
    if not ignore_umapped:
        print("Warning: Input profile contains key not mapped to Ditto. key={}".format(key))


def json_to_ditto(in_profile, out_profile):
    if not out_profile.endswith(".ditto"):
        print("Warning: ditto profiles usually end with .ditto")

    json_profile = load_json(in_profile)
    ditto_profile = open(out_profile, "wb")

    if (json_profile is None) or (ditto_profile is None):
        print("Failed to load required data(json: {}, out: {})".format(json_profile,ditto_profile))
        return False

    stdv_re = re.compile("^[0-9]+_stdv$")

    total_fly = total_press = 0
    fly_time_total = press_time_total = 0
    
    # When we can't map from the js to the ditto code for a key we throw it out. This can
    # have an impact on the overall fly and press times for a profile
    thrown_fly = []
    thrown_press = []
    
    num_stdv = 0

    unit = FSProfileStruct()
    unit.setTimeType(FSProfileTimeType.FLY_TIME)

    for from_key, to_key_info in json_profile['fly_times'].items():
        for to_key, time in to_key_info.items():
            if stdv_re.match(to_key):
                # We don't include any stdv info in ditto profiles right now
                num_stdv += 1
                continue

            mapped_key = DittoSCFromJSCode(int(from_key))
            if mapped_key < 0:
                unmapped_key_warn(from_key)
                thrown_fly.append(int(time))
                continue

            unit.setFromKey(mapped_key)

            mapped_key = DittoSCFromJSCode(int(to_key))
            if mapped_key < 0:
                unmapped_key_warn(to_key)
                thrown_fly.append(int(time))
                continue

            unit.setToKey(mapped_key)

            unit.setKeyTimeMs(int(time))

            writeStruct(ditto_profile, unit)
            total_fly += 1
            fly_time_total += int(time)

    ## Setup for adding press times
    unit.setTimeType(FSProfileTimeType.PRESS_TIME)

    for key, time in json_profile['press_times'].items():
        if stdv_re.match(key):
            # Once again, we don't include stdv right now
            num_stdv += 1
            continue

        # For press times Ditto just takes the 'from' key but to be safe
        # incase of future changes I just set both to be the same
        mapped_key = DittoSCFromJSCode(int(key))
        if mapped_key < 0:
            # They will already get the warning in fly times
            #unmapped_key_warn(key)
            thrown_press.append(int(time))
            continue

        unit.setFromKey(mapped_key)
        unit.setToKey(mapped_key)

        unit.setKeyTimeMs(int(time))

        writeStruct(ditto_profile, unit)
        total_press += 1
        press_time_total += int(time)

    ditto_profile.close()

    print("\nFinished converting source profile '{}' to ditto profile '{}'".format(in_profile, out_profile))
    print("\t# Fly Times: {}".format(total_fly))
    print("\t# Press Times: {}".format(total_press))
    print("\tFly time average: {}".format(fly_time_total / total_fly))
    print("\tPress time average: {}".format(press_time_total / total_press))
    if len(thrown_fly) > 0:
        print("\tAverage of unmapped fly times: {}".format(sum(thrown_fly) / len(thrown_fly)))
    if len(thrown_press) > 0:
        print("\tAverage of unmapped press times: {}".format(sum(thrown_press) / len(thrown_press)))
    print("\tIgnored {} standard deviation entries".format(num_stdv))

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Convet a set of json profiles to ditto binary profiles")
    parser.add_argument('profiles', metavar='P', type=str, nargs='+',
            help='profiles to convert')
    parser.add_argument('-o','--outdir', type=str, required=True, help="Output directory for partitioning information")
    parser.add_argument('--ignore_unmapped', action='store_true', default=False, help="Ignore warnings about unmapped keys")
    
    args = parser.parse_args()

    ignore_umapped = args.ignore_unmapped

    for profile in args.profiles:
        if 'user_map.json' in profile:
            print("Ignoring 'user_map.json'")
            continue

        # Take the basename for the profile to make the ditto profile name
        out_profile = "{}.ditto".format(profile.rsplit('.',1)[0])

        print("json_to_ditto({}, {})".format(profile, out_profile))
        json_to_ditto(profile, out_profile)
