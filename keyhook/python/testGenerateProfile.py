from ScanCodeMaps import *
from pyDittoTypes import *

if __name__ == '__main__':
    profile = open("pySample.ditto", "wb")

    unmapped = {}

    for jsKeyFrom in range(48,90):
        for jsKeyTo in range(48,90):
            unit = FSProfileStruct()
            unit.setTimeType(FSProfileTimeType.FLY_TIME)
            unit.setKeyTime(1000)  # 1 second delay on every key

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
