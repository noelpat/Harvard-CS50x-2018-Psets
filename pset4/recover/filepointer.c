#include <stdio.h>
#include <cs50.h>
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
    FILE* file = fopen(argv[1], "r");
    //check is file is found or not.
    if (file == NULL)
    {
        fprintf(stderr, "File not found!\n");
        return 2;
    }

    BYTE buff[512];
    int iter = 1;
    FILE* out;
    out = fopen("out2.txt", "w+");
    while (fread(buff, 512, 1, file) == 1)
    {
        printf("fread from forensic image! Iteration: %d\n", iter++);
        printf("Buff[0]: %hhu\n", buff[0]);
        printf("Buff[1]: %hhu\n", buff[1]);
        printf("Buff[2]: %hhu\n", buff[2]);
        printf("Buff[3]: %hhu\n", buff[3]);
        printf("Buff[511]: %hhu\n", buff[511]);

    //}


    string buffer = "New Milo Banana\n";

    fwrite(buffer, 12, 1, out);
    fwrite(buffer, 8, 1, out);
    fwrite(buffer, 4, 1, out);
    }
    printf("Finished fwriting to output file!\n");

    fclose(out);
    printf("I finished with NO segmentation fault!\n");
}