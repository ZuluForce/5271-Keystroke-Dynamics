import sys
from mappings.scan_code_maps import *
from ditto_types import *

if __name__ == '__main__':
    profile = open("pySample.ditto", "wb")

    # For the sample we just use a uniform fly time between every key
    keyDelay = 1000
    if len(sys.argv) > 1:
        keyDelay = int(sys.argv[1])
        if keyDelay < 0:
            print("Invalid keyDelay value: " + str(keyDelay))
            sys.exit(-1)


    print("Generating sample profile with a uniform key delay of {} ms".format(keyDelay))

    unmapped = {}

    for jsKeyFrom in range(48,90):
        for jsKeyTo in range(48,90):
            unit = FSProfileStruct()
            unit.setTimeType(FSProfileTimeType.FLY_TIME)
            unit.setKeyTime(keyDelay)

            dittoKey = DittoSCFromJSCode(jsKeyFrom)
            if dittoKey < 0:
                dittoKey = 0
                unmapped[jsKeyFrom] = True
            unit.setFromKey(dittoKey)

            dittoKey = DittoSCFromJSCode(jsKeyTo)
            if dittoKey < 0:
                dittoKey = 0
                unmapped[jsKeyTo] = True
            unit.setToKey(dittoKey)

            # Write the binary value to the file
            writeStruct(profile, unit)

    profile.close()

    print("Unmapped Keys Encountered (default to key 0):")
    for key in unmapped:
        print("\t" + str(key))
