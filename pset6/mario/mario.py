# implement mario in python

import cs50

test = True

while test:
    # prompt user
    h = cs50.get_int("Height: ")
    if h == 0:
        # do nothing
        print("", end="")
        break
    elif h < 0 or h > 23:
        # do nothing
        pass
    elif h == 1:
        print("##")
        break
    elif h > 1:
        # create rows
        spaces = h
        j = 2
        for i in range(h):
            spaces -= 1
            print(" " * spaces, end="")
            print("#" * j)
            j += 1
        break