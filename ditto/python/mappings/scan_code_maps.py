
# This map is generated from what I can see in keystroke.js
#
# If the enum in profile_enforcer.h or the javascript keycodes get updated
# so too does this map.
JSCodeToDitto = {
    8  : 0x0E,  # backspace

    13 : 0x1C,  # return

    16 : 0x2A,  # left shift (ditto distinguishes left and right but not js)

    32 : 0x39,  # space

    48 : 0x0B,  # 0
    49 : 0x02,  # 1
    50 : 0x03,  # 2
    51 : 0x04,  # 3
    52 : 0x05,  # 4
    53 : 0x06,  # 5
    54 : 0x07,  # 6
    55 : 0x08,  # 7
    56 : 0x09,  # 8
    57 : 0x0A,  # 9

    65 : 0x1E,  # a
    66 : 0x30,  # b
    67 : 0x2E,  # c
    68 : 0x20,  # d
    69 : 0x12,  # e
    70 : 0x21,  # f
    71 : 0x22,  # g
    72 : 0x23,  # h
    73 : 0x17,  # i
    74 : 0x24,  # j
    75 : 0x25,  # k
    76 : 0x26,  # l
    77 : 0x32,  # m
    78 : 0x31,  # n
    79 : 0x18,  # o
    80 : 0x19,  # p
    81 : 0x10,  # q
    82 : 0x13,  # r
    83 : 0x1F,  # s
    84 : 0x14,  # t
    85 : 0x16,  # u
    86 : 0x2F,  # v
    87 : 0x11,  # w
    88 : 0x2D,  # x
    89 : 0x15,  # y
    90 : 0x2C,  # z
    
    186 : 0x27, # ;
    188 : 0x33, # ,
    189 : 0x0C, # -
    190 : 0x34, # .
    191 : 0x35, # /
    219 : 0x1A, # [
    220 : 0x2B, # \
    221 : 0x1B, # ]
    222 : 0x28, # '
}


DittoSCToChar = {
    0x0E: "backspace",

    0x1C: "return",

    0x2A: "lShift",

    0x39: "space",

    0x0B: "0",
    0x02: "1",
    0x03: "2",
    0x04: "3",
    0x05: "4",
    0x06: "5",
    0x07: "6",
    0x08: "7",
    0x09: "8",
    0x0A: "9",

    0x1E:  "a",
    0x30:  "b",
    0x2E:  'c',
    0x20:  'd',
    0x12:  'e',
    0x21:  'f',
    0x22:  'g',
    0x23:  'h',
    0x17:  'i',
    0x24:  'j',
    0x25:  'k',
    0x26:  'l',
    0x32:  'm',
    0x31:  'n',
    0x18:  'o',
    0x19:  'p',
    0x10:  'q',
    0x13:  'r',
    0x1F:  's',
    0x14:  't',
    0x16:  'u',
    0x2F:  'v',
    0x11:  'w',
    0x2D:  'x',
    0x15:  'y',
    0x2C:  'z',
    
    0x1A:  '[',
    0x1B:  ']',
    0x27:  ';',
    0x33:  ',',
    0x34:  '.',
    0x35:  '/',
    0x2B:  '\\',
    0x28:  '\'',
}

def DittoSCFromJSCode(jscode):
    """
    Takes in a javascript key code and returns the scancode used to represent
    that key in ditto. If the given key is not mapped to a a ditto scancode, -1
    is returned.
    """
    try:
        return JSCodeToDitto[jscode]
    except:
        return -1


if __name__ == '__main__':
    print("JS(67) = Ditto({})".format(DittoSCFromJSCode(67)))
