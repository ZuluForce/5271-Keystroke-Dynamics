#!/usr/bin/python

# I would recommend the commented out method for importing if you
# use this for code later. This will reduce namespace clutter
# import DittoPyScancodes as sc
from DittoPyScancodes import *

print("Currently defined symbols (not all from DittoPyScancodes): ")
print("{}\n\n".format(dir()))

print(identity(SCANCODE_X))

print(SCANCODE_X)
print(SCANCODE_Y)
print(SCANCODE_V)
print("SCANCODE_X = " + str(int(SCANCODE_X)))
print("SCANCODE_Y = " + str(int(SCANCODE_Y)))
print("SCANCODE_V = " + str(int(SCANCODE_V)))

print(SCANCODE_X+2)
print("SCANCODE_X+2 == SCANCODE_V: {}".format(SCANCODE_X+2 == SCANCODE_V))
print("SCANCODE_X+2 == SCANCODE_Y: {}".format(SCANCODE_X+2 == SCANCODE_Y))
