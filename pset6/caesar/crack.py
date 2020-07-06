import pwd
import crypt
import getpass
import sys
import cs50
import argparse
from hmac import compare_digest as compare_hash
from helpers import twoC, threeC, fourC, fiveC

if len(sys.argv) != 2:
    print("Usage: python crack.py hash")
    sys.exit(1)

if len(sys.argv[1]) < 2:
    print("Usage: python crack.py hash")
    sys.exit(1)

# Example uncomment below
# hashed = crypt.crypt("y", "50")
# print(hashed)

# Grab the salt
salt = ""
salt += sys.argv[1][0]
salt += sys.argv[1][1]

uinput = sys.argv[1]

# print(salt)

# Optional Dictionary Attack


# Check all possible 1 character passwords
letter = 65
one = False

for i in range(0,52):
    brute = crypt.crypt(chr(letter), salt)

    # Compare hashes and print letter if it matches
    if compare_hash(uinput, brute):
        print(chr(letter))
        one = True
        sys.exit()

    # Jump to lowercase letters when the loop hits 90 (the last uppercase letter Z)
    if letter == 90:
        letter = 96
    letter += 1

print("No one character hash matched!")

# Check all possible 2 character passwords
if one == False:
    found = twoC(uinput, salt)
    if found != 0:
        # return template with found (the alphabet)
        print(found)
        sys.exit()

    found = threeC(uinput, salt)
    if found != 0:
        # return template with found (the alphabet)
        print(found)
        sys.exit()

    found = fourC(uinput, salt)
    if found != 0:
        # return template with found (the alphabet)
        print(found)
        sys.exit()

    found = fiveC(uinput, salt)
    if found != 0:
        # return template with found (the alphabet)
        print(found)
        sys.exit()