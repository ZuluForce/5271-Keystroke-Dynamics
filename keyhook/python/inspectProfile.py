import sys
from ScanCodeMaps import *
from pyDittoTypes import *

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: {} [profile]".format(sys.argv[0]))
        sys.exit(1)
    
    unitSize = sizeofStruct(FSProfileStruct)

    with open(sys.argv[1], "rb") as profile:
        sBytes = profile.read(unitSize)
        while sBytes:
            struct = buildStructFromBytes(FSProfileStruct(), sBytes)
            if struct.time_type == FSProfileTimeType.FLY_TIME:
                print(" {0.from_key:^2} -> {0.to_key:^2} : {0.time_in_ms} ms".format(struct))
            elif struct.time_type == FSProfileTimeType.PRESS_TIME:
                print(" {0.from_key:^2} |> {0.to_key:^2} : {0.time_in_ms} ms".format(struct))
            else:
                print("!! ---- Unrecognized FSProfileTimeType ( {} ) ---- !!".format(struct.time_type))

            sBytes = profile.read(unitSize)
        
