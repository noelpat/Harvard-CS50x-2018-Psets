# implement caesar cipher

import cs50
import sys

if len(sys.argv) != 2:
    print("Usage: python caesar.py k")
    sys.exit(1)
else:
    # prompt user for input
    uin = cs50.get_string("plaintext: ")

    # declare variables
    k = int(sys.argv[1])
    ascii = 0
    caesar = 0
    position = 0
    out = ""
    upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    lower = "abcdefghijklmnopqrstuvwxyz"

    # iterate through string and convert each letter using k
    for i in range(len(uin)):
        position = 0
        ascii = ord(uin[i])
        if ascii >= 65 and ascii <= 90:
            while uin[i] != upper[position]:
                position += 1
            caesar = (position + k) % 26
            out += upper[caesar]
        elif ascii >= 97 and ascii <= 122:
            while uin[i] != lower[position]:
                position += 1
            caesar = (position + k) % 26
            out += lower[caesar]
        else:
            out += uin[i]
    print("ciphertext:", out)