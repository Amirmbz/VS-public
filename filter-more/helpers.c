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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int gx[3][3];
    int gy[3][3];
    int32_t redx;
    int32_t bluex;
    int32_t greenx;
    int32_t redy;
    int32_t bluey;
    int32_t greeny;
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            gx[i][j] = (-1 + j) * ((i % 2) + 1);
            gy[j][i] = (-1 + j) * ((i % 2) + 1);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            redx = 0;
            greenx = 0;
            bluex = 0;
            redy = 0;
            greeny = 0;
            bluey = 0;
            for (int n = 0; n < 3; n++)
            {
                if (i - 1 + n < 0)
                {
                    n++;
                }
                if (i - 1 + n >= height)
                {
                    break;
                }
                for (int m = 0; m < 3; m++)
                {
                    if (j - 1 + m < 0)
                    {
                        m++;
                    }
                    if (j - 1 + m >= width)
                    {
                        break;
                    }
                    redx += gx[n][m] * image[i - 1 + n][j - 1 + m].rgbtRed;
                    greenx += gx[n][m] * image[i - 1 + n][j - 1 + m].rgbtGreen;
                    bluex += gx[n][m] * image[i - 1 + n][j - 1 + m].rgbtBlue;
                    redy += gy[n][m] * image[i - 1 + n][j - 1 + m].rgbtRed;
                    greeny += gy[n][m] * image[i - 1 + n][j - 1 + m].rgbtGreen;
                    bluey += gy[n][m] * image[i - 1 + n][j - 1 + m].rgbtBlue;
                }
            }
            redx = round((double) sqrt(pow(redx, 2) + pow(redy, 2)));
            if (redx > 255)
            {
                redx = 255;
            }
            copy[i][j].rgbtRed = redx;
            greenx = round((double) sqrt(pow(greenx, 2) + pow(greeny, 2)));
            if (greenx > 255)
            {
                greenx = 255;
            }
            copy[i][j].rgbtGreen = greenx;
            bluex = round((double) sqrt(pow(bluex, 2) + pow(bluey, 2)));
            if (bluex > 255)
            {
                bluex = 255;
            }
            copy[i][j].rgbtBlue = bluex;
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
