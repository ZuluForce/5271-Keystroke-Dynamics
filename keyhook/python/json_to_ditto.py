import re
import sys

from util.json_handler import load_json
from mappings.scan_code_maps import *
from ditto_types import *


def unmapped_key_warn(key):
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

    unit = FSProfileStruct()
    unit.setTimeType(FSProfileTimeType.FLY_TIME)

    for from_key, to_key_info in json_profile['fly_times'].items():
        for key, value in to_key_info.items():
            if stdv_re.match(key):
                # We don't include any stdv info in ditto profiles right now
                continue

            mapped_key = DittoSCFromJSCode(int(from_key))
            if mapped_key < 0:
                unmapped_key_warn(from_key)
                continue

            unit.setFromKey(mapped_key)

            mapped_key = DittoSCFromJSCode(int(key))
            if mapped_key < 0:
                unmapped_key_warn(key)
                continue

            unit.setToKey(mapped_key)

            unit.setKeyTime(int(value))

            writeStruct(ditto_profile, unit)
            total_fly += 1

    ## Setup for adding press times
    unit.setTimeType(FSProfileTimeType.PRESS_TIME)

    for key, time_info in json_profile['press_times'].items():
        if stdv_re.match(key):
            # Once again, we don't include stdv right now
            continue

        # For press times Ditto just takes the 'from' key but to be safe
        # incase of future changes I just set both to be the same
        mapped_key = DittoSCFromJSCode(int(key))
        if mapped_key < 0:
            # They will already get the warning in fly times
            #unmapped_key_warn(key)
            continue

        unit.setFromKey(mapped_key)
        unit.setToKey(mapped_key)

        unit.setKeyTime(int(time_info))

        writeStruct(ditto_profile, unit)
        total_press += 1

    ditto_profile.close()

    print("\nFinished converting source profile '{}' to ditto profile '{}'".format(in_profile, out_profile))
    print("\t# Fly Times: {}".format(total_fly))
    print("\t# Press Times: {}".format(total_press))


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: {} [profile prefix]+".format(sys.argv[0]))
        print("Ex| andrewh_reduced.json; call '{} andrewh_reduced'".format(sys.argv[0]))
        print("\tThis will output a profile called andrewh_reduced.ditto")

    for prefix in sys.argv[1:]:
        in_profile = "{}.json".format(prefix)
        out_profile = "{}.ditto".format(prefix)

        print("json_to_ditto({}, {})".format(in_profile,out_profile))
        json_to_ditto(in_profile, out_profile)