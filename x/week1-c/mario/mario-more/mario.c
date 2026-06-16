#include <cs50.h>
#include <stdio.h>

void print_row(int bricks, int height);

int main(void)
{
    // Prompt the user for the hieght
    int height;
    do
    {
        height = get_int("Height? ");
    }
    while (height < 1 || height > 8);

    // Print the pyramid??
    for (int i = 0; i < height; i++)
    {
        print_row(i + 1, height);
    }
}

void print_row(int bricks, int height)
{
    // Print the right pyramid
    int spaces = height - bricks;
    for (int i = 0; i < spaces; i++)
    {
        printf(" ");
    }
    for (int i = 0; i < bricks; i++)
    {
        printf("#");
    }

    // The space between the two pyramids
    printf("  ");

    // Print the left pyramid
    for (int i = 0; i < bricks; i++)
    {
        printf("#");
    }

    printf("\n");
}
