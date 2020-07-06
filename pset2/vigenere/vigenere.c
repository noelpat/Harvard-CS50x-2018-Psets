/*
CS50 - Problem Set 2 - Vigenere
This program encrypts messages using Vigenere's ciper.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <cs50.h>

int main(int argc, string argv[])
{
    //if the program is ran without an input OR more than one input exit
    if (argc != 2)
    {
        printf("Usage: ./vigenere k\n");
        return 1;
    }
    //check for non-alphabetical characters
    string k = argv[1];
    for (int i = 0; i < strlen(k); i++)
    {
        //check each character from argv[1] for non-alpha characters
        if (isalpha(k[i]) == false)
        {
            printf("Usage: ./vigenere k\n");
            return 1;
        }
    }

    //declare variables
    int ascii = 0;
    int caesar = 0;
    int position = 0;
    int v = 0;
    int j = 0;
    int wrap = strlen(k);
    int keys[wrap];

    //prompt user for plaintext to be encrypted
    string plain = get_string("plaintext:  ");

    //declare array just big enough to hold encrypted text
    char output[strlen(plain) + 1];
    output[strlen(plain)] = '\0';

    //encrypt text (could call a function to do this)
    //declare arrays for holding alphabet A-z
    char upper[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char lower[26] = "abcdefghijklmnopqrstuvwxyz";

    //iterate through input and find index value for each letter
    for (int i = 0; i < wrap; i++)
    {
        position = 0;
        //k = key/input
        while (k[i] != upper[position] && k[i] != lower[position])
        {
            position++;
        }
        keys[i] = position; //add letter index value to keys array
    }

    /*for (int i = 0; i < wrap; i++)
    {
        printf("%d\n", keys[i]);
    }*/

    //iterate through the string and convert each letter using k
    for (int i = 0; i < strlen(plain); i++)
    {
        position = 0;
        ascii = plain[i]; //get ascii value of letter
        //printf("ascii value of letter: %d\n", ascii);
        //verify that it is indeed a letter before changing
        if (ascii >= 65 && ascii <= 90) //uppercase letters
        {
            //get position of letter
            while (plain[i] != upper[position])
            {
                position++;
            }
            //printf("Position set to: %d\n", position);
            v = j++ % wrap;
            caesar = (position + keys[v]) % 26;
            //printf("caesar shift: %d\n", caesar);
            //printf("current letter to add to output: %c\n", upper[caesar]);
            output[i] = upper[caesar];
        }
        else if (ascii >= 97 && ascii <= 122) //lowercase letters
        {
            //get position of letter
            while (plain[i] != lower[position])
            {
                position++;
            }
            //printf("Position set to: %d\n", position);
            v = j++ % wrap;
            caesar = (position + keys[v]) % 26;
            //printf("caesar shift: %d\n", caesar);
            //printf("current letter to add to output: %c\n", upper[caesar]);
            output[i] = lower[caesar];
        }
        else
        {
            output[i] = plain[i];
        }
    }
    //print encrypted text
    printf("ciphertext: %s\n", output);
    return 0;
}