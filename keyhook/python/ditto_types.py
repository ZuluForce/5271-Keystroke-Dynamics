from ctypes import *
from io import BytesIO

# Example:
#  new_enum = enum(alpha=0, bravo=1, charlie='2')
#  new_enum.alpha
#   > 0
#  new_enum.charlie
#   > '2'
def enum(**enums):
    return type('Enum', (), enums)

# new_enum = auto_enum('alpha', 'bravo', 'charlie')
# new_enum.alpha
# > 0
def auto_enum(*sequential, **named):
    enums = dict(zip(sequential, range(len(sequential))), **named)
    return type('Enum', (), enums)

FSProfileTimeType = auto_enum('FLY_TIME', 'PRESS_TIME')

class FSProfileStruct(Structure):
    _pack_ = 0
    _fields_ = [
        ("time_type", c_int),
        ("time_in_ms", c_uint),
        ("from_key", c_int),
        ("to_key", c_int)
        ]

    def setTimeType(self, t):
        self.time_type = t

    def setKeyTime(self, t):
        self.time_in_ms = t

    def setFromKey(self, val):
        self.from_key = val

    def setToKey(self, val):
        self.to_key = val

def sizeofStruct(struct):
    return sizeof(struct)

def buildStructFromBytes(struct, encoded):
    """
    Build a struct from the encoded data given. The <struct> parameter should
    be an instance of the Structure class you want to fill.
    """
    bytefile = BytesIO(encoded)
    bytefile.readinto(struct)
    return struct

def getStructBytes(struct):
    if not isinstance(struct, Structure):
        raise "getStructBytes: struct is not a instance of ctypes.Structure"
        
    bytefile = BytesIO()
    bytefile.write(struct)

    return bytefile.getvalue()

def writeStruct(file, struct):
    file.write(getStructBytes(struct))

if __name__ == '__main__':
    unit = FSProfileStruct()
    print(dir(unit))
    print("sizeof FSProfileStruct: {}".format(sizeof(unit)))

    unit.setToKey(10)
    unit.setFromKey(20)
    unit.setTimeType(FSProfileTimeType.FLY_TIME)
    unit.setKeyTime(200) # 200ms fly time from key(20) -> key(10)

    fakefile = BytesIO()
    fakefile.write(unit)

    print(fakefile.getvalue())
    print(getStructBytes(unit))
    

    
