#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    char *infile = argv[2];
    char *outfile = argv[3];

    // declare variables
    //int* pixels = malloc(n * sizeof(int));

    if (n > 100 || n <= 0)
    {
        fprintf(stderr, "Must be a positive integer less than or equal to 100.\n");
        return 1;
    }

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    if (n == 1)
    {
        // write outfile's BITMAPFILEHEADER
        fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

        // write outfile's BITMAPINFOHEADER
        fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

        // iterate over infile's scanlines
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }

            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
        }

        // close infile
        fclose(inptr);

        // close outfile
        fclose(outptr);
        return 0; //success
    }
    else
    {
        // determine padding for scanlines
        //int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

        // create outfile's BITMAPFILEHEADER
        BITMAPFILEHEADER outbf;
        outbf = bf; //set them equal to input file before making changes
        // create outfile's BITMAPINFOHEADER
        BITMAPINFOHEADER outbi;
        outbi = bi;

        //update values in headers
        outbi.biWidth = bi.biWidth * n;
        outbi.biHeight = bi.biHeight * n;

        // determine padding for output file
        int padding2 = (4 - (outbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

        //Formula from video: bi.biSizeImage = ((sizeof(RGBTRIPLE) * bi.biWidth) + padding) * abs(bi.biHeight);
        outbi.biSizeImage = ((sizeof(RGBTRIPLE) * outbi.biWidth) + padding2) * abs(outbi.biHeight); //type DWORD aka uint32_t
        outbf.bfSize = outbi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER); //type DWORD aka uint32_t
        // write outfile's BITMAPFILEHEADER and BITMAPINFOHEADER
        fwrite(&outbf, sizeof(BITMAPFILEHEADER), 1, outptr);
        fwrite(&outbi, sizeof(BITMAPINFOHEADER), 1, outptr);

        //declare variables for nested loops
        int aSize = outbi.biWidth; //declare int to use for array size

        int lines = 1;
        int aPlace = 0;
        RGBTRIPLE tripA[aSize];

        // iterate over infile's scanlines //for each row
        for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
        {
            aPlace = 0;
            //printf("Current scanline: %d\n", lines);
            // temporary storage
            RGBTRIPLE triple;
            //RGBTRIPLE* tripA = malloc(n * sizeof(RGBTRIPLE));

            //clear the array with place holder values
            for (int z = 0; z < aSize; z++)
            {
                tripA[z].rgbtBlue = 0x00;
                tripA[z].rgbtGreen = 0x00;
                tripA[z].rgbtRed = 0x00;
            }

            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                //printf("Values read: %hhu %hhu %hhu\n", triple.rgbtBlue, triple.rgbtGreen, triple.rgbtRed);
                //write pixel to array n times
                for (int m = 0; m < n; m++)
                {
                    tripA[aPlace] = triple;
                    aPlace++;
                }
            }

            //Print statement blocks for debugging and testing
            //printf("Arraysize: %d Current array values \n", aSize);
            // int aspot = 1;
            // for (int boss = 0; boss < aSize; boss++)
            // {

            //     printf("%d Blue: %hhu Green: %hhu Red: %hhu \n", aspot, tripA[boss].rgbtBlue, tripA[boss].rgbtGreen,
            //     tripA[boss].rgbtRed);
            //     aspot++;
            // }

            //for n times
            for (int k = 0; k < n; k++)
            {
                //write array to outfile
                for (int y = 0; y < aSize; y++)
                {
                    fwrite(&tripA[y], sizeof(RGBTRIPLE), 1, outptr);
                }
                //add padding
                for (int w = 0; w < padding2; w++)
                {
                    fputc(0x00, outptr);
                }
            }

            //skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // then add it back (to demonstrate how)
            // for (int k = 0; k < padding; k++)
            // {
            //     fputc(0x00, outptr);
            // }
            lines++; //increment line count by 1
        }

        // close infile
        fclose(inptr);

        // close outfile
        fclose(outptr);

        // success
        return 0;
    }
    return 1; //neither of if statement finished.
}