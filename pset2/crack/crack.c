/*problem set 2: Crack
Accepts a single command line argument.
Assume each password has been hashed with C's DES-based crypt function.
No password is longer than 5 characters.
Each password only contains alpabetical characters uppercase or lowercase.
2 arguments passed to crypt: key, salt.
The salt for most of the passwords is "50" in the example hashes.

ASCII Values for lowercase letters are from 097 - 122
ASCII Values for uppercase letters are from 065 - 090

This program reads from the following files to check hashes against common passwords:
dictionary.txt
names.txt
university.txt

These files should be in the same folder/directory as the program when it is run.
*/

//includes

#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cs50.h>

//prototypes
char *crypt(const char *key, const char *salt);
//declare functions for each step in the cracking process
int oneC(string input, char salt[]);
int twoC(string input, char salt[]);
int threeC(string input, char salt[]);
int fourC(string input, char salt[]);
int fourC(string input, char salt[]);
int easyMoney(string input, char salt[]);
int fiveC(string input, char salt[]);

int main(int argc, string argv[])
{
    //if the program is ran without an input exit
    if (argc <= 1)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    //test against whatever hash you want by creating them here with print statements and running the program once.
    printf("Value of 'test' hash: ");
    printf("%s\n", crypt("test", "1234"));
    //printf("%s\n", crypt("zzzzz", "50"));

    int found = 0;
    //create string equal to input
    string input = argv[1];

    //create char array to hold salt
    char salt[3] = "";
    //pull salt(first two characters) from input
    salt[0] = input[0];
    salt[1] = input[1];
    salt[2] = '\0';

    //call functions to crack password and print it
    //printf("Checking all possible 1 character passwords...\n");
    found = oneC(input, salt);
    //if oneC finds the password return 0 and so on...
    if (found == 1)
    {
        return 0;
    }
    //printf("Checking all possible 2 character passwords...\n");
    found = twoC(input, salt);
    if (found == 1)
    {
        return 0;
    }
    //printf("Checking all possible 3 character passwords...\n");
    found = threeC(input, salt);
    if (found == 1)
    {
        return 0;
    }
    //printf("Attempting dictionary attack...\n");
    found = easyMoney(input, salt);
    if (found == 1)
    {
        return 0;
    }
    //printf("Checking all possible 4 character passwords...\n");
    found = fourC(input, salt);
    if (found == 1)
    {
        return 0;
    }
    //printf("Checking all possible 5 character passwords...\n");
    found = fiveC(input, salt);
    if (found == 1)
    {
        return 0;
    }
    //if the program makes it this far no match was found.
    printf("No match found.\n");
    return 0;
}

//this function checks all possible one character passwords
int oneC(string input, char salt[])
{
    //create ints representing ascii values for upper and lower case letters
    int lower = 97;
    int upper = 65;
    int capital = 0;
    string brute = ""; //string for comparing hashes

    //char array for holding ALL possible passwords
    char alphabet[6] = "";
    alphabet[0] = lower;
    alphabet[5] = '\0';

    //while loops for cracking 1 character passwords in this scenario
    //the first loop checks all possible lowercase 1 letter passwords
    while (upper <= 90)
    {
        brute = crypt(alphabet, salt);

        //if the tested hash and the input string are equal print the alphabet array and exit
        if (strcmp(brute, input) == 0)
        {
            printf("%s\n", alphabet);
            return 1;
        }
        //if the letter hash did not match try the next letter by incrementing the ASCII value by one.
        if (lower < 122)
        {
            alphabet[0] = ++lower;
        }
        else if (lower == 122 && upper == 65 && capital == 0)
        {
            alphabet[0] = upper;
            ++capital;
        }
        else if (lower == 122 && upper <= 90 && capital == 1)
        {
            alphabet[0] = ++upper;
        }
    }
    return 0;
}

//this function checks all possible two character passwords
int twoC(string input, char salt[])
{
    string brute = "";
    int upper = 65;
    int upper2 = 65;
    int lower = 97;
    int lower2 = 97;
    int capital = 0;

    //char array for holding ALL possible passwords
    char alphabet[6] = "";
    alphabet[0] = lower;
    alphabet[5] = '\0';

    //set alphabet char array to check for the first possible string "aa"
    alphabet[1] = lower2;

    //checks all possible combinations with lowercase first letter
    while (upper <= 90)
    {
        //this loop will check every possible combination with a second lowercase letter
        lower2 = 97;
        alphabet[1] = lower2;

        while (lower2 <= 122)
        {
            brute = crypt(alphabet, salt);

            //if the tested hash and the input string are equal print the alphabet array and exit
            if (strcmp(brute, input) == 0)
            {
                printf("%s\n", alphabet);
                return 1;
            }
            //increment lower2 to check the next lowercase letter in array place [1]
            alphabet[1] = ++lower2;
        }

        upper2 = 65;
        alphabet[1] = upper2; //set second letter to capital A and iterate to Z with loop below

        //this loop will check every possible combination with the second letter being capital
        while (upper2 <= 122)
        {
            brute = crypt(alphabet, salt);

            //if the tested hash and the input string are equal print the alphabet array and exit
            if (strcmp(brute, input) == 0)
            {
                printf("%s\n", alphabet);
                return 1;
            }

            alphabet[1] = ++upper2;
        }

        //repeat the above while loops with the next lower case letter.
        if (lower < 122)
        {
            alphabet[0] = ++lower;
        }
        else if (lower == 122 && upper == 65 && capital == 0)
        {
            alphabet[0] = upper;
            ++capital;
        }
        else if (lower == 122 && upper <= 90 && capital == 1)
        {
            alphabet[0] = ++upper;
        }
    }
    return 0;
}

//function to check all 3 character passwords
int threeC(string input, char salt[])
{
    //declare variables/ASCII values
    int lower = 97;
    int lower2 = 97;
    int lower3 = 97;
    int upper = 65;
    int upper2 = 65;
    int upper3 = 65;
    int capital = 0; //for switching main while loop to check capital letters
    int capital2 = 0;
    int capital3 = 0;

    string brute = "";

    //char array for holding ALL possible passwords
    char alphabet[6] = "";
    alphabet[0] = lower;
    alphabet[5] = '\0';

    //checks all possible combinations with lowercase first letter in 3 letter password
    while (upper <= 90)
    {
        lower2 = 97;
        upper2 = 65;
        capital2 = 0;
        alphabet[1] = lower2;
        //this loop will check every possible combination with a second lowercase letter and a third letter
        while (upper2 <= 90)
        {
            //this loop will iterate the third letter to a - z
            lower3 = 97;
            upper3 = 65;
            capital3 = 0;
            alphabet[2] = lower3;

            while (upper3 <= 90)
            {
                brute = crypt(alphabet, salt);

                //if the tested hash and the input string are equal print the alphabet array and exit
                if (strcmp(brute, input) == 0)
                {
                    printf("%s\n", alphabet);
                    return 1;
                }
                if (lower3 < 122)
                {
                    alphabet[2] = ++lower3;
                }
                else if (lower3 == 122 && upper3 == 65 && capital3 == 0)
                {
                    alphabet[2] = upper3;
                    ++capital3;
                }
                else if (lower3 == 122 && upper3 <= 90 && capital3 == 1)
                {
                    alphabet[2] = ++upper3;
                }
            }
            //repeat above while loops with next letter
            if (lower2 < 122)
            {
                alphabet[1] = ++lower2;
            }
            else if (lower2 == 122 && upper2 == 65 && capital2 == 0)
            {
                alphabet[1] = upper2;
                ++capital2;
            }
            else if (lower2 == 122 && upper2 <= 90 && capital2 == 1)
            {
                alphabet[1] = ++upper2;
            }
        }

        //repeat the above while loops with the next letter.
        if (lower < 122)
        {
            alphabet[0] = ++lower;
        }
        else if (lower == 122 && upper == 65 && capital == 0)
        {
            alphabet[0] = upper;
            ++capital;
        }
        else if (lower == 122 && upper <= 90 && capital == 1)
        {
            alphabet[0] = ++upper;
        }
    }
    return 0;
}

int fourC(string input, char salt[])
{
    //declare variables/ASCII values
    int lower = 97;
    int lower2 = 97;
    int lower3 = 97;
    int lower4 = 97;
    int upper = 65;
    int upper2 = 65;
    int upper3 = 65;
    int upper4 = 65;
    int capital = 0; //for switching the main while loop to capital letters
    int capital2 = 0;
    int capital3 = 0;
    int capital4 = 0;

    string brute = "";

    //char array for holding ALL possible passwords
    char alphabet[6] = "";
    alphabet[0] = lower;
    alphabet[5] = '\0';

    //checks all possible 4 letter combinations with lowercase first letter in 4 letter password
    while (upper <= 90)
    {
        lower2 = 97;
        upper2 = 65;
        capital2 = 0;
        alphabet[1] = lower2;
        //this loop will check every possible combination with a second lowercase letter
        while (upper2 <= 90)
        {
            //this loop will iterate the third letter to a - z
            lower3 = 97;
            upper3 = 65;
            capital3 = 0;
            alphabet[2] = lower3;

            while (upper3 <= 90)
            {
                lower4 = 97;
                upper4 = 65;
                capital4 = 0;
                alphabet[3] = lower4;
                //iterate fourth letter a - z
                while (upper4 <= 90)
                {
                    brute = crypt(alphabet, salt);

                    //if the tested hash and the input string are equal print the alphabet array and exit
                    if (strcmp(brute, input) == 0)
                    {
                        printf("%s\n", alphabet);
                        return 1;
                    }
                    if (lower4 < 122)
                    {
                        alphabet[3] = ++lower4;
                    }
                    else if (lower4 == 122 && upper4 == 65 && capital4 == 0)
                    {
                        alphabet[3] = upper4;
                        ++capital4;
                    }
                    else if (lower4 == 122 && upper4 <= 90 && capital4 == 1)
                    {
                        alphabet[3] = ++upper4;
                    }
                }
                //repeat above while loops with next letter
                if (lower3 < 122)
                {
                    alphabet[2] = ++lower3;
                }
                else if (lower3 == 122 && upper3 == 65 && capital3 == 0)
                {
                    alphabet[2] = upper3;
                    ++capital3;
                }
                else if (lower3 == 122 && upper3 <= 90 && capital3 == 1)
                {
                    alphabet[2] = ++upper3;
                }
            }
            //repeat above while loops with next letter
            if (lower2 < 122)
            {
                alphabet[1] = ++lower2;
            }
            else if (lower2 == 122 && upper2 == 65 && capital2 == 0)
            {
                alphabet[1] = upper2;
                ++capital2;
            }
            else if (lower2 == 122 && upper2 <= 90 && capital2 == 1)
            {
                alphabet[1] = ++upper2;
            }
        }
        //repeat the above while loops with the next letter.
        if (lower < 122)
        {
            alphabet[0] = ++lower;
        }
        else if (lower == 122 && upper == 65 && capital == 0)
        {
            alphabet[0] = upper;
            ++capital;
        }
        else if (lower == 122 && upper <= 90 && capital == 1)
        {
            alphabet[0] = ++upper;
        }
    }
    return 0;
}

int easyMoney(string input, char salt[])
{
    /*
    This function will check the hash against a word list of:
    -4 letter words
    -5 letter words
    -5 letter names (The most common according to: https://www.ssa.gov/OACT/babynames/decades/century.html)
    -5 letter University names in the united states (Source: https://en.wikipedia.org/wiki/List_of_colloquial_names_for_universities_and_colleges_in_the_United_States)
    -Common acronyms on the internet such as "rofl" or "tgif".
    All from two text files named "dictionary4.txt" and "dictionary.txt".
    */

    //declare variables
    string brute = "";
    char word[50];

    //open and read from dictionary4.txt (contains 4 letter words)
    FILE *easy = fopen("dictionary4.txt", "r");
    if (easy != NULL)
    {
        //printf("dictionary4.txt opened!\n");
        //read every line/string in the file
        while (fgets(word, 5, easy) != NULL)
        {
            brute = crypt(word, salt);

            //if the tested hash and the input string are equal print the word array and exit
            if (strcmp(brute, input) == 0)
            {
                printf("%s\n", word);
                return 1;
            }
        }
        fclose(easy);
    }

    //open and read from dictionary.txt
    FILE *money = fopen("dictionary.txt", "r");
    if (money != NULL)
    {
        //printf("dictionary.txt opened!\n");
        //read every line/string in the file
        while (fgets(word, 6, money) != NULL)
        {
            brute = crypt(word, salt);

            //if the tested hash and the input string are equal print the word array and exit
            if (strcmp(brute, input) == 0)
            {
                printf("%s\n", word);
                return 1;
            }
        }
        fclose(money);
    }
    //printf("Dictionary attack failed!\n");
    return 0;
}

int fiveC(string input, char salt[])
{
    //declare variables/ASCII values
    int lower = 97;
    int lower2 = 97;
    int lower3 = 97;
    int lower4 = 97;
    int lower5 = 97;
    int upper = 65;
    int upper2 = 65;
    int upper3 = 65;
    int upper4 = 65;
    int upper5 = 65;
    int capital = 0;
    int capital2 = 0;
    int capital3 = 0;
    int capital4 = 0;
    int capital5 = 0;

    string brute = "";

    //char array for holding ALL possible passwords
    char alphabet[6] = "";
    alphabet[0] = lower;
    alphabet[5] = '\0';

    //check a few worst-case scenarios before entering the final brute force loop
    brute = crypt("ZZZZZ", salt);
    if (strcmp(brute, input) == 0)
    {
        printf("ZZZZZ\n");
        return 1;
    }
    brute = crypt("zzzzz", salt);
    if (strcmp(brute, input) == 0)
    {
        printf("zzzzz\n");
        return 1;
    }
    brute = crypt("YYYYY", salt);
    if (strcmp(brute, input) == 0)
    {
        printf("YYYYY\n");
        return 1;
    }
    brute = crypt("yyyyy", salt);
    if (strcmp(brute, input) == 0)
    {
        printf("yyyyy\n");
        return 1;
    }


    //checks all possible 4 letter combinations with lowercase first letter in 4 letter password
    while (upper <= 90)
    {
        lower2 = 97;
        upper2 = 65;
        capital2 = 0;
        alphabet[1] = lower2;
        //this loop will check every possible combination with a second lowercase letter
        while (upper2 <= 90)
        {
            //this loop will iterate the third letter to a - z
            lower3 = 97;
            upper3 = 65;
            capital3 = 0;
            alphabet[2] = lower3;

            while (upper3 <= 90)
            {
                lower4 = 97;
                upper4 = 65;
                capital4 = 0;
                alphabet[3] = lower4;
                //iterate fourth letter a - z
                while (upper4 <= 90)
                {
                    //printf("Checking with third letter: %c\n", alphabet[2]);
                    lower5 = 97;
                    upper5 = 65;
                    capital5 = 0;
                    alphabet[4] = lower5;
                    //iterate fifth letter a - z
                    while (upper5 <= 90)
                    {
                        //printf("5th letter %c\n", alphabet[4]);
                        brute = crypt(alphabet, salt);

                        //if the tested hash and the input string are equal print the alphabet array and exit
                        if (strcmp(brute, input) == 0)
                        {
                            printf("%s\n", alphabet);
                            return 1;
                        }
                        //repeat above while loops with next letter
                        if (lower5 < 122)
                        {
                            alphabet[4] = ++lower5;
                        }
                        else if (lower5 == 122 && upper5 == 65 && capital5 == 0)
                        {
                            alphabet[4] = upper5;
                            ++capital5;
                        }
                        else if (lower5 == 122 && upper5 <= 90 && capital5 == 1)
                        {
                            alphabet[4] = ++upper5;
                        }
                    }
                    //repeat above while loops with next letter
                    if (lower4 < 122)
                    {
                        alphabet[3] = ++lower4;
                    }
                    else if (lower4 == 122 && upper4 == 65 && capital4 == 0)
                    {
                        alphabet[3] = upper4;
                        ++capital4;
                    }
                    else if (lower4 == 122 && upper4 <= 90 && capital4 == 1)
                    {
                        alphabet[3] = ++upper4;
                    }
                }
                //repeat above while loops with next letter
                if (lower3 < 122)
                {
                    alphabet[2] = ++lower3;
                }
                else if (lower3 == 122 && upper3 == 65 && capital3 == 0)
                {
                    alphabet[2] = upper3;
                    ++capital3;
                }
                else if (lower3 == 122 && upper3 <= 90 && capital3 == 1)
                {
                    alphabet[2] = ++upper3;
                }
            }
            //repeat above while loops with next letter
            if (lower2 < 122)
            {
                alphabet[1] = ++lower2;
            }
            else if (lower2 == 122 && upper2 == 65 && capital2 == 0)
            {
                alphabet[1] = upper2;
                ++capital2;
            }
            else if (lower2 == 122 && upper2 <= 90 && capital2 == 1)
            {
                alphabet[1] = ++upper2;
            }
        }
        //repeat the above while loops with the next letter.
        if (lower < 122)
        {
            alphabet[0] = ++lower;
        }
        else if (lower == 122 && upper == 65 && capital == 0)
        {
            alphabet[0] = upper;
            ++capital;
        }
        else if (lower == 122 && upper <= 90 && capital == 1)
        {
            alphabet[0] = ++upper;
        }
        //printf("Checking with first letter: %c\n", alphabet[0]);
    }
    return 0;
}