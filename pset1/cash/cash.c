//problem set 1 - cash

#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float input = 0;
    int coins = 0;
    int count = 0;

    do
    {
        input = get_float("Change owed: ");
    }
    while (input < 0);

    //find minimum number of coins to return
    //convert float to int to avoid rounding errors etc.
    input = input * 100.00;
    count = round(input); //round input to the nearest penny

    //while quarters can be used increase count
    while (count / 25 >= 1)
    {
        //decrease amount by .25
        count = count - 25;
        coins++;
    }
    //while dimes can be used increase count
    //decrease amount by .10
    while (count / 10 >= 1)
    {
        //decrease amount by .25
        count = count - 10;
        coins++;
    }

    //while nickels can be used increase count
    //decrease amount by .05
    while (count / 5 >= 1)
    {
        //decrease amount by .25
        count = count - 5;
        coins++;
    }

    //while pennies can be used increase count
    //decrease amount by .01
    while (count / 1 >= 1)
    {
        //decrease amount by .25
        count = count - 1;
        coins++;
    }

    //print number of coins used
    printf("%d\n", coins);
}