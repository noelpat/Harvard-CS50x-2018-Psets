import pwd
import crypt
import getpass
import sys
import cs50
from hmac import compare_digest as compare_hash

# Demonstration of how we modify strings for maximum efficiency
# test = "CA"
# test = test[:1] + "Z"
# print(test)
# print_bitch = twoChar("50mjprEcqC/ts", "50")
# print(print_bitch)

def twoC(uinput, salt):
    print("Entered 2 character function!")
    # test all two character passwords
    upper = 65
    upper2 = 65
    lower = 97
    lower2 = 97
    capital = 0

    alphabet = "aa"

    while upper <= 90:
        # this loop will check every possible combo with a second lowercase letter
        lower2 = 97
        alphabet = alphabet[:1] + chr(lower2)

        while lower2 <= 122:
            brute = crypt.crypt(alphabet, salt)

            # if tested hash and input string are equal return alphabet
            if uinput == brute:
                return alphabet

            # increment lower2 to check the next lowercase letter in array place [1]
            lower2 += 1
            alphabet = alphabet[:1] + chr(lower2)

        upper2 = 65
        alphabet = alphabet[:1] + chr(upper2)

        while upper2 <= 122:
            brute = crypt.crypt(alphabet, salt)

            # if tested hash and input string are equal return alphabet
            if uinput == brute:
                return alphabet

            # increment upper2 and update alphabet
            upper2 += 1
            alphabet = alphabet[:1] + chr(upper2)

        # repeat the above while loops with the next lower case letter.
        if lower < 122:
            lower += 1
            alphabet = alphabet[:0] + chr(lower)
        elif lower == 122 and upper == 65 and capital == 0:
            alphabet = alphabet[:0] + chr(upper)
            capital += 1
        elif lower == 122 and upper <= 90 and capital == 1:
            upper += 1
            alphabet = alphabet[:0] + chr(upper)
    # if no match found return 0
    return 0

def threeC(uinput, salt):
    print("Entered 3 character function!")
    # test all three character passwords
    upper = 65
    upper2 = 65
    upper3 = 65
    lower = 97
    lower2 = 97
    lower3 = 97
    # for switching main while loop to check capital letters
    capital = 0
    capital2 = 0
    capital3 = 0

    alphabet = "aaa"

    while upper <= 90:
        # this loop will check every possible combo with a second lowercase letter and 3rd letter
        upper2 = 65
        lower2 = 97
        alphabet = alphabet[:1] + chr(lower2)

        while upper2 <= 90:
            lower3 = 97
            upper3 = 65
            capital3 = 0
            alphabet = alphabet[:2] + chr(lower3)

            while upper3 <= 90:
                # print("Testing " + alphabet)
                brute = crypt.crypt(alphabet, salt)

                # if tested hash and input string are equal return alphabet
                if uinput == brute:
                    return alphabet

                if lower3 < 122:
                    lower3 += 1
                    alphabet = alphabet[:2] + chr(lower3)
                elif lower3 == 122 and upper3 == 65 and capital3 == 0:
                    alphabet = alphabet[:2] + chr(upper3)
                    capital3 += 1
                elif lower3 == 122 and upper3 <= 90 and capital3 == 1:
                    upper3 += 1
                    alphabet = alphabet[:2] + chr(upper3)

            # repeat the above while loops with the next letter
            if lower2 < 122:
                lower2 += 1
                alphabet = alphabet[:1] + chr(lower2)
            elif lower2 == 122 and upper2 == 65 and capital2 == 0:
                alphabet = alphabet[:1] + chr(upper2)
                capital2 += 1
            elif lower == 122 and upper2 <= 90 and capital2 == 1:
                upper2 += 1
                alphabet = alphabet[:1] + chr(upper2)

        # repeat the above while loops with the next letter
        if lower < 122:
            lower += 1
            alphabet = alphabet[:0] + chr(lower)
        elif lower == 122 and upper == 65 and capital == 0:
            alphabet = alphabet[:0] + chr(upper)
            capital += 1
        elif lower == 122 and upper <= 90 and capital == 1:
            upper += 1
            alphabet = alphabet[:0] + chr(upper)
    return 0

def fourC(uinput, salt):
    # test all four character passwords
    upper = 65
    upper2 = 65
    upper3 = 65
    upper4 = 65
    lower = 97
    lower2 = 97
    lower3 = 97
    lower4 = 97
    # for switching main while loop to check capital letters
    capital = 0
    capital2 = 0
    capital3 = 0
    capital4 = 0

    alphabet = "aaaa"

    while upper <= 90:
        # this loop will check every possible combo with a second lowercase letter and 3rd letter
        upper2 = 65
        lower2 = 97
        alphabet = alphabet[:1] + chr(lower2)

        while upper2 <= 90:
            lower3 = 97
            upper3 = 65
            capital3 = 0
            alphabet = alphabet[:2] + chr(lower3)

            while upper3 <= 90:
                lower4 = 97
                upper4 = 65
                capital4 = 0
                alphabet = alphabet[:3] + chr(lower4)

                # iterate fourth letter a - z
                while upper4 <= 90:
                    brute = crypt.crypt(alphabet, salt)

                    # if tested hash and input string are equal return alphabet
                    if uinput == brute:
                        return alphabet

                    if lower4 < 122:
                        lower4 += 1
                        alphabet = alphabet[:3] + chr(lower4)
                    elif lower4 == 122 and upper4 == 65 and capital4 == 0:
                        alphabet = alphabet[:3] + chr(upper4)
                        capital4 += 1
                    elif lower4 == 122 and upper4 <= 90 and capital4 == 1:
                        upper4 += 1
                        alphabet = alphabet[:3] + chr(upper4)

                if lower3 < 122:
                    lower3 += 1
                    alphabet = alphabet[:2] + chr(lower3)
                elif lower3 == 122 and upper3 == 65 and capital3 == 0:
                    alphabet = alphabet[:2] + chr(upper3)
                    capital3 += 1
                elif lower3 == 122 and upper3 <= 90 and capital3 == 1:
                    upper3 += 1
                    alphabet = alphabet[:2] + chr(upper3)

            # repeat the above while loops with the next letter
            if lower2 < 122:
                lower2 += 1
                alphabet = alphabet[:1] + chr(lower2)
            elif lower2 == 122 and upper2 == 65 and capital2 == 0:
                alphabet = alphabet[:1] + chr(upper2)
                capital2 += 1
            elif lower == 122 and upper2 <= 90 and capital2 == 1:
                upper2 += 1
                alphabet = alphabet[:1] + chr(upper2)

        # repeat the above while loops with the next letter
        if lower < 122:
            lower += 1
            alphabet = alphabet[:0] + chr(lower)
        elif lower == 122 and upper == 65 and capital == 0:
            alphabet = alphabet[:0] + chr(upper)
            capital += 1
        elif lower == 122 and upper <= 90 and capital == 1:
            upper += 1
            alphabet = alphabet[:0] + chr(upper)
    return 0

def fiveC(uinput, salt):
    # test all four character passwords
    upper = 65
    upper2 = 65
    upper3 = 65
    upper4 = 65
    upper5 = 65
    lower = 97
    lower2 = 97
    lower3 = 97
    lower4 = 97
    lower5 = 97
    # for switching main while loop to check capital letters
    capital = 0
    capital2 = 0
    capital3 = 0
    capital4 = 0
    capital5 = 0

    alphabet = "aaaaa"

    while upper <= 90:
        # this loop will check every possible combo with a second lowercase letter and 3rd letter
        upper2 = 65
        lower2 = 97
        alphabet = alphabet[:1] + chr(lower2)

        while upper2 <= 90:
            lower3 = 97
            upper3 = 65
            capital3 = 0
            alphabet = alphabet[:2] + chr(lower3)

            while upper3 <= 90:
                lower4 = 97
                upper4 = 65
                capital4 = 0
                alphabet = alphabet[:3] + chr(lower4)

                # iterate fourth letter a - z
                while upper4 <= 90:
                    lower5 = 97
                    upper5 = 65
                    capital5 = 0
                    alphabet = alphabet[:4] + lower5
                    # iterate fifth letter a - z
                    while upper5 <= 90:
                        brute = crypt.crypt(alphabet, salt)

                        # if tested hash and input string are equal return alphabet
                        if uinput == brute:
                            return alphabet

                        if lower5 < 122:
                            lower5 += 1
                            alphabet = alphabet[:4] + chr(lower5)
                        elif lower5 == 122 and upper5 == 65 and capital5 == 0:
                            alphabet = alphabet[:4] + chr(upper5)
                            capital5 += 1
                        elif lower5 == 122 and upper5 <= 90 and capital5 == 1:
                            upper5 += 1
                            alphabet = alphabet[:4] + chr(upper5)

                    if lower4 < 122:
                        lower4 += 1
                        alphabet = alphabet[:3] + chr(lower4)
                    elif lower4 == 122 and upper4 == 65 and capital4 == 0:
                        alphabet = alphabet[:3] + chr(upper4)
                        capital4 += 1
                    elif lower4 == 122 and upper4 <= 90 and capital4 == 1:
                        upper4 += 1
                        alphabet = alphabet[:3] + chr(upper4)

                if lower3 < 122:
                    lower3 += 1
                    alphabet = alphabet[:2] + chr(lower3)
                elif lower3 == 122 and upper3 == 65 and capital3 == 0:
                    alphabet = alphabet[:2] + chr(upper3)
                    capital3 += 1
                elif lower3 == 122 and upper3 <= 90 and capital3 == 1:
                    upper3 += 1
                    alphabet = alphabet[:2] + chr(upper3)

            # repeat the above while loops with the next letter
            if lower2 < 122:
                lower2 += 1
                alphabet = alphabet[:1] + chr(lower2)
            elif lower2 == 122 and upper2 == 65 and capital2 == 0:
                alphabet = alphabet[:1] + chr(upper2)
                capital2 += 1
            elif lower == 122 and upper2 <= 90 and capital2 == 1:
                upper2 += 1
                alphabet = alphabet[:1] + chr(upper2)

        # repeat the above while loops with the next letter
        if lower < 122:
            lower += 1
            alphabet = alphabet[:0] + chr(lower)
        elif lower == 122 and upper == 65 and capital == 0:
            alphabet = alphabet[:0] + chr(upper)
            capital += 1
        elif lower == 122 and upper <= 90 and capital == 1:
            upper += 1
            alphabet = alphabet[:0] + chr(upper)
    return 0