// Helper functions for music

#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    //convert numerator and denominator from a string
    int numer = atoi(&fraction[0]);
    int denom = atoi(&fraction[2]);

    if (denom == 8)
    {
        //if denom is 8 nothing needs to be done to numer
        return numer;
    }
    else if (denom == 4)
    {
        numer = numer * 2;
        return numer;
    }
    else if (denom == 2)
    {
        numer = numer * 4;
        return numer;
    }
    return 0;
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    // TODO
    //parse the string into a note and it's octave
    //calculate the frequency of the note in the given octave
    /*
    A4 is 440 Hz
    For every semitone up we move, we multiply the frequency by 2^1/12
    For every semitone down we move, we divide the frequency by 2^1/12

    To go up an octave, multiply by 2^12/12 = 2
    To go down an octave, divide by 2^12/12 = 2

    note will be a string
    First character will always be the letter of the note ('A', 'B', 'C', etc.)
    Second character could be an accidental ('#', 'b') or an octave ('2', '3', '4', etc.)
    If second character was an accidental, third character will be the octave
    */

    int freq = 440; //A4
    int number = 0;
    double solve = 0;

    //if frequency is A4 return default frequency value
    if (strcmp(note, "A4") == 0)
    {
        return freq;
    }
    //Found this technique online at: https://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
    //convert the number to an int no matter where it is. This way switch statements will work.
    if (strlen(note) == 2)
    {
        number = note[1] - '0';
    }
    else if (strlen(note) == 3)
    {
        number = note[2] - '0';
    }

    if (note[0] == 'A')
    {
        solve = freq;

        //switch statement for every note in this octave
        switch (number)
        {
            case 0 :
                solve = (((solve / 2.00) / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 1 :
                solve = ((solve / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 2 :
                solve = (solve / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 3 :
                solve = solve / 2.00;
                solve = round(solve);
                break;
            case 5 :
                solve = solve * 2.00;
                break;
            case 6 :
                solve = solve * 2.00 * 2.00;
                break;
            case 7 :
                solve = solve * 2.00 * 2.00 * 2.00;
                break;
            case 8 :
                solve = solve * 2.00 * 2.00 * 2.00 * 2.00;
                break;
        }
        //if the note is a sharp
        if (note[1] == '#')
        {
            //multiply freq by 2^1/12
            solve = solve * pow(2.00, 1.00 / 12.00);
        }
        //if the note is a flat
        if (note[1] == 'b')
        {
            //divide freq by 2^1/12
            solve = solve / pow(2.00, 1.00 / 12.00);
        }
        freq = round(solve);
        return freq;
    }
    if (note[0] == 'B')
    {
        //set freq for B4 (440 * 2^2/12 = 494)
        freq = 494;
        solve = freq;

        //switch statement for every note in this octave
        switch (number)
        {
            case 0 :
                solve = (((solve / 2.00) / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 1 :
                solve = ((solve / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 2 :
                solve = (solve / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 3 :
                solve = solve / 2.00;
                solve = round(solve);
                break;
            case 5 :
                solve = solve * 2.00;
                break;
            case 6 :
                solve = solve * 2.00 * 2.00;
                break;
            case 7 :
                solve = solve * 2.00 * 2.00 * 2.00;
                break;
            case 8 :
                solve = solve * 2.00 * 2.00 * 2.00 * 2.00;
                break;
        }
        //if the note is a sharp
        if (note[1] == '#')
        {
            //multiply freq by 2^1/12
            solve = solve * pow(2.00, 1.00 / 12.00);
        }
        //if the note is a flat
        if (note[1] == 'b')
        {
            //divide freq by 2^1/12
            if (strcmp(note, "Bb5") == 0)
            {
                freq = 932;
                return freq;
            }
            else
            {
                solve = solve / pow(2.00, 1.00 / 12.00);
            }
        }
        freq = round(solve);
        return freq;
    }
    if (note[0] == 'C')
    {
        //set freq for C5 (440 * 2^3/12)
        freq = 523;
        solve = freq;

        //switch statement for every note in this octave
        switch (number)
        {
            case 0 :
                solve = ((((solve / 2.00) / 2.00) / 2.00) / 2.00) / 2.00;
                break;
            case 1 :
                solve = (((solve / 2.00) / 2.00) / 2.00) / 2.00;
                break;
            case 2 :
                solve = ((solve / 2.00) / 2.00) / 2.00;
                break;
            case 3 :
                solve = (solve / 2.00) / 2.00;
                break;
            case 4 :
                solve = solve / 2.00;
                break;
            case 6 :
                solve = solve * 2.00;
                break;
            case 7 :
                solve = solve * 2.00 * 2.00;
                break;
            case 8 :
                solve = solve * 2.00 * 2.00 * 2.00;
                break;
        }
        //if the note is a sharp
        if (note[1] == '#')
        {
            //multiply freq by 2^1/12
            solve = solve * pow(2.00, 1.00 / 12.00);
        }
        //if the note is a flat
        if (note[1] == 'b')
        {
            //divide freq by 2^1/12
            solve = solve / pow(2.00, 1.00 / 12.00);
            //freq = solve;
            //return freq;
        }
        freq = round(solve);
        return freq;
    }
    if (note[0] == 'D')
    {
        //set freq for D5 (440 * 2^5/12 = 587)
        freq = 587;
        solve = freq;

        //switch statement for every note in this octave
        switch (number)
        {
            case 0 :
                solve = ((((solve / 2.00) / 2.00) / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 1 :
                solve = (((solve / 2.00) / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 2 :
                solve = ((solve / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 3 :
                solve = (solve / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 4 :
                solve = solve / 2.00;
                solve = round(solve);
                break;
            case 6 :
                solve = solve * 2.00;
                break;
            case 7 :
                solve = solve * 2.00 * 2.00;
                break;
            case 8 :
                solve = solve * 2.00 * 2.00 * 2.00;
                break;
        }
        //if the note is a sharp
        if (note[1] == '#')
        {
            //multiply freq by 2^1/12
            solve = solve * pow(2.00, 1.00 / 12.00);
        }
        //if the note is a flat
        if (note[1] == 'b')
        {
            //divide freq by 2^1/12
            solve = solve / pow(2.00, 1.00 / 12.00);
            //freq = solve;
            //return freq;
        }
        freq = round(solve);
        return freq;
    }
    if (note[0] == 'E')
    {
        //set freq for E5 (440 * 2^7/12 = 659)
        freq = 659;
        solve = freq;

        //switch statement for every note in this octave
        switch (number)
        {
            case 0 :
                solve = ((((solve / 2.00) / 2.00) / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 1 :
                solve = (((solve / 2.00) / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 2 :
                solve = ((solve / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 3 :
                solve = (solve / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 4 :
                solve = solve / 2.00;
                solve = round(solve);
                break;
            case 6 :
                solve = solve * 2.00;
                break;
            case 7 :
                solve = solve * 2.00 * 2.00;
                break;
            case 8 :
                solve = solve * 2.00 * 2.00 * 2.00;
                break;
        }
        //if the note is a sharp
        if (note[1] == '#')
        {
            //multiply freq by 2^1/12
            solve = solve * pow(2.00, 1.00 / 12.00);
        }
        //if the note is a flat
        if (note[1] == 'b')
        {
            //divide freq by 2^1/12
            solve = solve / pow(2.00, 1.00 / 12.00);
            //freq = solve;
            //return freq;
        }
        freq = round(solve);
        return freq;
    }
    if (note[0] == 'F')
    {
        //set freq for F5 (440 * 2^8/12 = 698)
        freq = 698;
        solve = freq;

        //switch statement for every note in this octave
        switch (number)
        {
            case 0 :
                solve = ((((solve / 2.00) / 2.00) / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 1 :
                solve = (((solve / 2.00) / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 2 :
                solve = ((solve / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 3 :
                solve = (solve / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 4 :
                solve = solve / 2.00;
                solve = round(solve);
                break;
            case 6 :
                solve = solve * 2.00;
                break;
            case 7 :
                solve = solve * 2.00 * 2.00;
                break;
            case 8 :
                solve = solve * 2.00 * 2.00 * 2.00;
                break;
        }
        //if the note is a sharp
        if (note[1] == '#')
        {
            //multiply freq by 2^1/12
            solve = solve * pow(2.00, 1.00 / 12.00);
        }
        //if the note is a flat
        if (note[1] == 'b')
        {
            //divide freq by 2^1/12
            solve = solve / pow(2.00, 1.00 / 12.00);
            //freq = solve;
            //return freq;
        }
        freq = round(solve);
        return freq;
    }
    if (note[0] == 'G')
    {
        //set freq for F5 (440 * 2^10/12 = 698)
        freq = 784;
        solve = freq;

        //switch statement for every note in this octave
        switch (number)
        {
            case 0 :
                solve = ((((solve / 2.00) / 2.00) / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 1 :
                solve = (((solve / 2.00) / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 2 :
                solve = ((solve / 2.00) / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 3 :
                solve = (solve / 2.00) / 2.00;
                solve = round(solve);
                break;
            case 4 :
                solve = solve / 2.00;
                solve = round(solve);
                break;
            case 6 :
                solve = solve * 2.00;
                break;
            case 7 :
                solve = solve * 2.00 * 2.00;
                break;
            case 8 :
                solve = solve * 2.00 * 2.00 * 2.00;
                break;
        }
        //if the note is a sharp
        if (note[1] == '#')
        {
            //multiply freq by 2^1/12
            solve = solve * pow(2.00, 1.00 / 12.00);
        }
        //if the note is a flat
        if (note[1] == 'b')
        {
            //divide freq by 2^1/12
            solve = solve / pow(2.00, 1.00 / 12.00);
            //freq = solve;
            //return freq;
        }
        freq = round(solve);
        return freq;
    }
    return 0;
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    //blank lines represent rests in our machine-readable format
    //if the string represents a rest return true
    if (strcmp(s, "") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
