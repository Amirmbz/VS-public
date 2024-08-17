#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, n; j < width; j++)
        {
            n = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.00);
            image[i][j].rgbtBlue = n;
            image[i][j].rgbtGreen = n;
            image[i][j].rgbtRed = n;
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            red = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen +
                        .189 * image[i][j].rgbtBlue);
            green = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen +
                          .168 * image[i][j].rgbtBlue);
            blue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen +
                         .131 * image[i][j].rgbtBlue);
            if (red > 255)
            {
                red = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }
            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    uint16_t blue;
    uint16_t green;
    uint16_t red;
    RGBTRIPLE copy[height][width];
    int count;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            blue = 0;
            green = 0;
            red = 0;
            count = 0;
            for (int n = i - 1; n < i + 2; n++)
            {
                if (n < 0)
                {
                    n++;
                }
                if (n >= height)
                {
                    break;
                }
                for (int m = j - 1; m < j + 2; m++)
                {
                    if (m < 0)
                    {
                        m++;
                    }
                    if (m >= width)
                    {
                        break;
                    }
                    blue += image[n][m].rgbtBlue;
                    green += image[n][m].rgbtGreen;
                    red += image[n][m].rgbtRed;
                    count++;
                }
            }
            copy[i][j].rgbtBlue = round(blue / (float) count);
            copy[i][j].rgbtGreen = round(green / (float) count);
            copy[i][j].rgbtRed = round(red / (float) count);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
}
