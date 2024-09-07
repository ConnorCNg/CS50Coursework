#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int gradMag(int Gx, int Gy);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // printf("%i\n", width);
    // printf("%d\n", image[1][2].rgbtBlue);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = round(((float) image[i][j].rgbtBlue + (float) image[i][j].rgbtGreen + (float) image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int halfw = width / 2; // For positive integers C always rounds down (skip middle column for images wiht odd widths)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < halfw; j++)
        {
            RGBTRIPLE storeLeftTriple = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = storeLeftTriple;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*imagehold)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (imagehold == NULL)
    {
        printf("Not enough memory to store image.\n");
        return;
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // printf("%i\n", i);
            // printf("%i\n", j);
            // printf("%d\n", image[599][399].rgbtBlue);
            // Get upper and lower bounds of 3x3 box
            int xlower = i - 1;
            int ylower = j - 1;
            int xupper = i + 1;
            int yupper = j + 1;

            // Account for going past bounds
            if (xlower < 0)
            {
                xlower = 0;
            }
            if (ylower < 0)
            {
                ylower = 0;
            }

            if (xupper > height - 1)
            {
                xupper = height - 1;
            }
            if (yupper > width - 1)
            {
                yupper = width - 1;
            }
            int blueSum = 0;
            int greenSum = 0;
            int redSum = 0;
            int avgdenom = 0;
            for (int a = xlower; a <= xupper; a++)
            {
                for (int b = ylower; b <= yupper; b++)
                {
                    blueSum += image[a][b].rgbtBlue;
                    greenSum += image[a][b].rgbtGreen;
                    redSum += image[a][b].rgbtRed;
                    avgdenom++;
                }
            }
            imagehold[i][j] = (RGBTRIPLE){round((float) blueSum / (float) avgdenom), round((float) greenSum / (float) avgdenom), round((float) redSum / (float) avgdenom)};
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = imagehold[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*imagehold)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (imagehold == NULL)
    {
        printf("Not enough memory to store image.\n");
        return;
    }
    int garray[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}; // Sobel operator gradient (going to use for both Gx and Gy)

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // printf("%i\n", i);
            // printf("%i\n", j);
            // printf("%d\n", image[599][399].rgbtBlue);
            // Get upper and lower bounds of 3x3 box
            int xlower = i - 1;
            int ylower = j - 1;
            int xupper = i + 1;
            int yupper = j + 1;
            int blueGx = 0;
            int greenGx = 0;
            int redGx = 0;
            int blueGy = 0;
            int greenGy = 0;
            int redGy = 0;

            for (int a = 0; a < 3; a++)
            {
                for (int b = 0; b < 3; b++)
                {
                    int xcoord = a + xlower;
                    int ycoord = b + ylower;
                    if ((xcoord >= 0) && (ycoord >= 0) && (xcoord <= height - 1) && (ycoord <= width - 1)) // Keep everything in bounds
                    {

                        // Gx calculations
                        blueGx += (image[xcoord][ycoord].rgbtBlue * garray[a][b]);
                        greenGx += (image[xcoord][ycoord].rgbtGreen * garray[a][b]);
                        redGx += (image[xcoord][ycoord].rgbtRed * garray[a][b]);

                        // Do Gy calculations (Gy is just Gx rotated 90 degrees)
                        blueGy += (image[xcoord][ycoord].rgbtBlue * garray[2 - b][a]);
                        greenGy += (image[xcoord][ycoord].rgbtGreen * garray[2 - b][a]);
                        redGy += (image[xcoord][ycoord].rgbtRed * garray[2 - b][a]);
                    }
                }
            }
            int blueG = gradMag(blueGx, blueGy);
            int greenG = gradMag(greenGx, greenGy);
            int redG = gradMag(redGx, redGy);
            imagehold[i][j] = (RGBTRIPLE){blueG, greenG, redG};
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = imagehold[i][j];
        }
    }
    return;
}

int gradMag(int Gx, int Gy)
{
    int val = round(sqrt(pow(Gx, 2) + pow(Gy, 2))); // (Gx^2 + Gy^2) ^ (1/2)
    if (val > 255)
    {
        val = 255;
    }
    return val;
}
