#include "helpers.h"
#include <math.h>

int min(int n);
void swap(RGBTRIPLE *a, RGBTRIPLE *b);
int average(int sum, int count);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Take average of red, green, and blue
            float average =
                (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0;
            int rounded = round(average);

            // Update pixel values
            image[i][j].rgbtRed = rounded;
            image[i][j].rgbtGreen = rounded;
            image[i][j].rgbtBlue = rounded;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Compute sepia values
            float sepiaRed = .393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen +
                             .189 * image[i][j].rgbtBlue;
            float sepiaGreen = .349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen +
                               .168 * image[i][j].rgbtBlue;
            float sepiaBlue = .272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen +
                              .131 * image[i][j].rgbtBlue;

            int finalRed = min(round(sepiaRed));
            int finalGreen = min(round(sepiaGreen));
            int finalBlue = min(round(sepiaBlue));

            // Update pixel with sepia values
            image[i][j].rgbtRed = finalRed;
            image[i][j].rgbtGreen = finalGreen;
            image[i][j].rgbtBlue = finalBlue;
        }
    }

    return;
}

// Return the min of 255, sepiacolour
int min(int n)
{
    if (n > 255)
        return 255;
    else
        return n;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            swap(&image[i][j], &image[i][width - 1 - j]);
        }
    }

    return;
}

// Swap pixels
void swap(RGBTRIPLE *a, RGBTRIPLE *b)
{
    RGBTRIPLE temp = *a;
    *a = *b;
    *b = temp;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Loop over each pixle
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Loop over pixel's neighbors, and calculate the average
            int redSum = 0;
            int greenSum = 0;
            int blueSum = 0;
            int count = 0;
            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    // Check edge cases
                    if (k >= 0 && k < height && l >= 0 && l < width)
                    {
                        redSum += copy[k][l].rgbtRed;
                        greenSum += copy[k][l].rgbtGreen;
                        blueSum += copy[k][l].rgbtBlue;
                        count++;
                    }
                }
            }

            int averageRed = average(redSum, count);
            int averageGreen = average(greenSum, count);
            int averageBlue = average(blueSum, count);

            // Set the color values of the current pixel in the original image to these averages
            image[i][j].rgbtRed = averageRed;
            image[i][j].rgbtGreen = averageGreen;
            image[i][j].rgbtBlue = averageBlue;
        }
    }

    return;
}

// Average function
int average(int sum, int count)
{
    float average = sum / (float) count;
    int rounded = round(average);

    return rounded;
}
