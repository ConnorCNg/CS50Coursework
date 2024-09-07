#include <cs50.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover input.raw\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    uint8_t buffer[512];
    bool write = false;
    int imagenum = 0;
    char filename[8];
    FILE *output = NULL;
    while (fread(&buffer, sizeof(uint8_t), 512, input))
    {
        // printf("0x%02X\n", buffer[0]);
        // count++;
        if ((buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF) && ((buffer[3] >> 4) == 0x0E) && (write == true))
        {
            fclose(output);
            imagenum++;
            sprintf(filename, "%03d.jpg", imagenum);
            output = fopen(filename, "wb");
            if (output == NULL)
            {
                printf("Could not open file.\n");
                return 1;
            }
            fwrite(buffer, 512, 1, output);
        }
        else if (write == true)
        {
            fwrite(buffer, 512, 1, output);
        }
        else if ((buffer[0] == 0xFF && buffer[1] == 0xD8 && buffer[2] == 0xFF) && ((buffer[3] & 0xF0) == 0xE0) && (write == false))
        {
            write = true;
            sprintf(filename, "%03d.jpg", imagenum);
            output = fopen(filename, "wb");
            if (output == NULL)
            {
                printf("Could not open file.\n");
                return 1;
            }
            fwrite(buffer, 512, 1, output);
        }
        /*
        if (buffer[0] == 0xE5)
        {
            printf("%i\n", count);
            return 1;
        }
        */
    }

    // Close files
    fclose(input);
    fclose(output);
}
