#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("could not open file.\n");
        return 1;
    }

    // Creat a buffer for a block of data
    uint8_t buffer[BLOCK_SIZE];

    int count = 0;
    bool writing_jpeg = false;
    char *file_name = malloc(8);
    FILE *img = NULL;

    // While there's still data left to read from the memory card
    while (fread(buffer, 1, BLOCK_SIZE, card) == BLOCK_SIZE)
    {
        // Create JPEGs from the data
        // Is the current block a start of a JPEG?
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // If yes, and writing_jpeg == true
            if (writing_jpeg)
            {
                // Close the previose image file, then open another image file
                fclose(img);
            }

            // If yes, and writing_jpeg == false
            // Just open another image file
            sprintf(file_name, "%03i.jpg", count);
            count++;

            img = fopen(file_name, "w");
            if (img == NULL)
            {
                printf("could not open image file.\n");
                return 1;
            }

            fwrite(buffer, 1, BLOCK_SIZE, img);
            writing_jpeg = true;
        }
        else
        {
            // If no, and writing_jpeg == true
            if (writing_jpeg)
            {
                // Write the current block to the current image file
                fwrite(buffer, 1, BLOCK_SIZE, img);
            }
        }
    }

    fclose(img);
    fclose(card);
    free(file_name);
}

