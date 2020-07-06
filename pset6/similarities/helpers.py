import cs50
from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""
    # Given two strings a and b this function should return lines that
    # are identical in both a and b. Avoid duplicates! Check for the string before
    # inserting it into your list.
    # lines in a and b will be separated by \n

    # Split each string into lines
    stra = []
    strb = []
    stra = a.splitlines()
    strb = b.splitlines()
    # Compute a list of all lines that appear in both a and b
    match = []
    cta = 0

    # algorithm checks for strings in b that are in a. Boolean to avoid duplicates.
    for i in stra:
        ctb = 0
        for j in strb:
            dupe = False
            if stra[cta] == strb[ctb]:
                dupecheck = 0
                for z in match:
                    if stra[cta] == match[dupecheck]:
                        dupe = True
                        break
                    dupecheck += 1
                if dupe == False:
                    match.append(stra[cta])
            ctb += 1
        cta += 1

    # return the list that contains identical lines
    return match


def sentences(a, b):
    """Return sentences in both a and b"""
    # Returns unique English sentences that are identical in both a and b.
    # No duplicates. Use sent_tokenize to separate each string into a list of sentences.
    stra = sent_tokenize(a)
    strb = sent_tokenize(b)

    # algorithm checks for strings in b that are in a. Boolean to avoid duplicates.
    cta = 0
    match = []

    for i in stra:
        ctb = 0
        for j in strb:
            dupe = False
            if stra[cta] == strb[ctb]:
                dupecheck = 0
                for z in match:
                    if stra[cta] == match[dupecheck]:
                        dupe = True
                        break
                    dupecheck += 1
                if dupe == False:
                    match.append(stra[cta])
            ctb += 1
        cta += 1

    return match


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    # returns a list of all substrings of length n that are in a and b. No duplicates
    stra = trimSub(a, n)
    strb = trimSub(b, n)

    # algorithm checks for strings in b that are in a. Boolean to avoid duplicates.
    cta = 0
    match = []

    for i in stra:
        ctb = 0
        for j in strb:
            dupe = False
            if stra[cta] == strb[ctb]:
                dupecheck = 0
                for z in match:
                    if stra[cta] == match[dupecheck]:
                        dupe = True
                        break
                    dupecheck += 1
                if dupe == False:
                    match.append(stra[cta])
            ctb += 1
        cta += 1

    return match

# Reference: https://stackoverflow.com/questions/22469997/how-to-get-all-the-contiguous-substrings-of-a-string-in-python#22470047
# I modified the function in the link above to work for this assignment.
# See the modified function below:


def trimSub(string, n):
    l = len(string)
    sub = [string[i:j+1] for i in range(l) for j in range(i, l)]
    trim = []
    count = 0
    for i in sub:
        if len(sub[count]) == n:
            trim.append(sub[count])
        count += 1
    return trim
