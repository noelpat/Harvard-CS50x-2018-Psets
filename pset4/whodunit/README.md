# Questions

## What's `stdint.h`?

'stdint.h' is a header file thats allows programmers to write more
portable code by providing a set of typedefs that specify exact-width
integer types, together with the defined minimum and maximum allowable
values for each type.

Source: https://en.wikibooks.org/wiki/C_Programming/stdint.h

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

First it has defined limits for each type.
For example, uint8_t can only be so big or so small and so on.

Second, they are primitive data types you can use and make the code more portable.

Third, for the ones starting with 'u', these integers all MUST be a positive number with no leading 0's.

Source: https://en.wikibooks.org/wiki/C_Programming/stdint.h#cite_note-pubs-1

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

In the 'bmp.h' header file they are aliases for primitive data types.

Recall that 8 bits is 1 byte.

BYTE is 1 byte.
DWORD is 4 bytes.
LONG is 4 bytes.
WORD is 2 bytes.

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

The first two bytes of any BMP file must be: 0x4D 0x42 (in hexadecimal)

source: https://en.wikipedia.org/wiki/BMP_file_format

## What's the difference between `bfSize` and `biSize`?

They both use DWORD (uint32_t).

However, bfSize defines the size, in bytes, of the bitmap file
and biSize is the number of bytes required by the structure.

Source 1: https://docs.microsoft.com/en-gb/windows/desktop/api/wingdi/ns-wingdi-tagbitmapfileheader
Source 2:https://msdn.microsoft.com/en-us/library/dd183376(v=vs.85).aspx

## What does it mean if `biHeight` is negative?

If positive, this indicates a bottom-up DIB with an origin in the lower-left corner.
If negative, this indicates a top-down DIB and its origin is the upper-left corner.

Source: https://msdn.microsoft.com/en-us/library/dd183376(v=vs.85).aspx

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

biBitCount.

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

If fopen returns 'NULL' when used with "r" it means the file must NOT exist or could not be found.
If fopen returns 'NULL' when used with "w" it means it failed to create the file.

Reference: https://reference.cs50.net/stdio/fopen

## Why is the third argument to `fread` always `1` in our code?

Because we only want to read the bytes (sizeof(BITMAPINFOHEADER) one time.

Reference: https://reference.cs50.net/stdio/fread

## What value does line 63 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

3.

int padding = (4 - (3 * sizeof(RGBTRIPLE)) % 4) % 4;
printf("Answer: %d\n", padding);

The above code will print 3.

## What does `fseek` do?

fseek sets the file position.

Reference: https://reference.cs50.net/stdio/fseek

## What is `SEEK_CUR`?

'SEEK_CUR'represents the current location of the pointer when setting the file position.

Reference: https://reference.cs50.net/stdio/fseek
