#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

//problem set 1

int main(void)
{
    int h = 0; //height
    char max[25] = ""; //holds the maximum amount of characters

    //do while loop that asks user for height here
    do
    {
        h = get_int("Height: ");
    }
    while (h < 0 || h > 23); //avoids negative numbers and anything over 23

    if (h > 1) //avoid this for loop for 1 row height
    {
        //create first row
        string hash = "#"; //string for adding hashes
        strcat(max, hash);
        strcat(max, hash);

        int spaces = h;
        for (int i = 0; i < h; i++)
        {
            spaces--;
            //print spaces
            for (int j = 0; j < spaces; j++)
            {
                printf(" ");
            }

            printf("%s\n", max);
            strcat(max, hash);
        }
    }
    else if (h == 1) //if the height is one just print one line ##
    {
        printf("##\n");
    }
    else if (h == 0)
    {
        //do nothing
    }
}