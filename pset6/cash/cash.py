# implement Cash

import cs50
import math

uin = 0.00
coins = 0
count = 0

while True:
    uin = cs50.get_float("Change owed: ")
    if (uin < 0):
        pass
    else:
        # find minimum number of coins to return using modulo
        # convert float to int to avoid rounding errors
        uin = uin * 100.00
        count = round(uin)
        # calculate coins
        coins = count // 25
        count = count % 25

        coins += count // 10
        count = count % 10

        coins += count // 5
        count = count % 5

        coins += count // 1
        print(coins)
        break