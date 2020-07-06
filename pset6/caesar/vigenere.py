# implement vigenere cipher

import cs50
import sys
import array

if len(sys.argv) != 2:
    print("Usage: python vigenere.py k")
    sys.exit(1)

k = sys.argv[1]
for i in k:
    if not i.isalpha():
        print("Usage: python vigenere.py k")
        sys.exit(1)

# prompt user for input
uin = cs50.get_string("plaintext: ")

# declare variables
ascii = 0
caesar = 0
position = 0
v = 0
j = 0
wrap = len(k)
keys = []
out = ""
upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
lower = "abcdefghijklmnopqrstuvwxyz"

# iterate through key and find index value for each letter
for i in range(0, wrap):
    position = 0;

    while k[i] != upper[position] and k[i] != lower[position]:
        position = position + 1

    keys.append(position)

# print keys list for debugging purposes
# print(keys)

# iterate through string and convert each letter using k
for i in range(len(uin)):
    position = 0
    ascii = ord(uin[i])
    if ascii >= 65 and ascii <= 90:
        while uin[i] != upper[position]:
            position += 1

        v = j % wrap
        j = j + 1
        caesar = (position + keys[v]) % 26
        out += upper[caesar]
    elif ascii >= 97 and ascii <= 122:
        while uin[i] != lower[position]:
            position += 1

        v = j % wrap
        j = j + 1
        caesar = (position + keys[v]) % 26
        out += lower[caesar]
    else:
        out += uin[i]

# print final output
print("ciphertext:", out)