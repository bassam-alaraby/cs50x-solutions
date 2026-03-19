#include <cs50.h>
#include <stdio.h>

void print_row(int bricks, int hieght);

int main(void)
{
    // prompt the user for input
    int hieght;
    do
    {
        hieght = get_int("What is a hieght of pyramid ? ");
    }
    while (hieght < 1);

    // print the pyramid
    for (int i = 0; i < hieght; i++)
    {
        print_row(i + 1, hieght);
    }
}

void print_row(int bricks, int hieght)
{
    int spaces = hieght - bricks;
    for (int j = 0; j < spaces; j++)
    {
        printf(" ");
    }
    for (int i = 0; i < bricks; i++)
    {
        printf("#");
    }
    printf("\n");
}
