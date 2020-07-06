#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //ensure proper usage
    if (argc <= 1)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    //open input image
    FILE *file = fopen(argv[1], "r");
    //check is file is found or not.
    if (file == NULL)
    {
        fprintf(stderr, "File not found!\n");
        return 2;
    }

    //declare variables
    char filename[10];
    int jname = 0; //keep track of jpg names as created

    FILE *img; //declare variable name for file here for multiple if statements

    //int track = 1; //track while loop iterations
    bool open = false; //set this to true when first jpg header is found
    BYTE buffer[512]; //array for holding bytes for writing

    while (fread(buffer, 512, 1, file) == 1)
    {
        //printf("While loop Iteration: %d\n", track++);
        //check for a header of a new jpeg
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            //printf("Header found!\n");
            if (open == true) //check if previous file still open
            {
                //printf("Closing a jpg out!\n");
                fclose(img);
                //break;
            }
            //open new jpg file for writing
            sprintf(filename, "%03d.jpg", jname++);
            //printf("created name for jpg with sprintf!\n");
            img = fopen(filename, "w");
            //printf("opened the new jpg!\n");
            open = true;
        }

        if (open == true)
        {
            fwrite(buffer, 512, 1, img);
            //printf("Wrote bytes!\n");
        }
        //check if not writing a jpg??
        //printf("Ending loop iteration.\n");
    }

    //printf("Reached end of program!\n");
    return 0;
}