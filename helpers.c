#include "helpers.h"
#include <math.h>

RGBTRIPLE calculatergbaverages(RGBTRIPLE *pixels, int arraysize, int averagedenominator);
int calculateaveragedenominator(int row, int column, int height, int width);
void initializebigimage(int bigimage_h, int bigimage_w, RGBTRIPLE bigimage[bigimage_h][bigimage_w]);
RGBTRIPLE applysobeloperator(RGBTRIPLE *pixels, int arraysize);

// TODO: SEE REAL GRAYSCALE BELOW, THIS IS A SANDPIT FUNCTION
// MAKE THE IMAGE APPEAR BLUE
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int row = 0; row < height; row++)
    {
        for (int column = 0; column < width; column++)
        {
            image[row][column].rgbtGreen = image[row][column].rgbtGreen/2;
            image[row][column].rgbtRed = image[row][column].rgbtRed/2;
        }
    }

    return;
}

// Convert image to grayscale
// void grayscale(int height, int width, RGBTRIPLE image[height][width])
// {
//     double average = 0;

//     for (int i = 0; i < height; i++)
//     {
//         for (int j = 0; j < width; j++)
//         {
//             average += image[i][j].rgbtBlue;
//             average += image[i][j].rgbtGreen;
//             average += image[i][j].rgbtRed;

//             average = round(average / 3);

//             image[i][j].rgbtBlue = average;
//             image[i][j].rgbtGreen = average;
//             image[i][j].rgbtRed = average;

//             average = 0;
//         }
//     }
//     return;
// }

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width - j); j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int kernelsize = 9;
    int averagedenominator = kernelsize;
    RGBTRIPLE pixels[kernelsize];
    int bigimage_h = height + 2;
    int bigimage_w = width + 2;
    // Create a temp image with a 1 pixel empty border
    RGBTRIPLE bigimage[bigimage_h][bigimage_w];

    // Make sure array is empty
    initializebigimage(bigimage_h, bigimage_w, bigimage);

    // Copy contents from image into the bigimage
    for (int i = 1; i < (bigimage_h - 1); i++)
    {
        for (int j = 1; j < (bigimage_w - 1); j++)
        {
            bigimage[i][j] = image[i - 1][j - 1];
        }
    }

    // Use the bigimage to calculate new rgb values and assign them to image
    for (int i = 1; i < (bigimage_h - 1); i++)
    {
        for (int j = 1; j < (bigimage_w - 1); j++)
        {
            // 1st row
            pixels[0] = bigimage[i - 1][j - 1];
            pixels[1] = bigimage[i - 1][j];
            pixels[2] = bigimage[i - 1][j + 1];
            // // 2nd row
            pixels[3] = bigimage[i][j - 1];
            pixels[4] = bigimage[i][j];
            pixels[5] = bigimage[i][j + 1];
            // // 3rd row
            pixels[6] = bigimage[i + 1][j - 1];
            pixels[7] = bigimage[i + 1][j];
            pixels[8] = bigimage[i + 1][j + 1];

            // Calculate the amount we want to divide with to calculate the average
            averagedenominator = calculateaveragedenominator(i - 1, j - 1, height, width);
            // Calculate the blur rgb values for the given pixel
            RGBTRIPLE blurredpixel = calculatergbaverages(pixels, kernelsize, averagedenominator);

            // Overwrite pixel in original image with the blurred pixel
            image[i - 1][j - 1].rgbtBlue = blurredpixel.rgbtBlue;
            image[i - 1][j - 1].rgbtGreen = blurredpixel.rgbtGreen;
            image[i - 1][j - 1].rgbtRed = blurredpixel.rgbtRed;
        }
    }

    return;
}

// Make sure the rgb values of all pixels start at 0,0,0
void initializebigimage(int bigimage_h, int bigimage_w, RGBTRIPLE bigimage[bigimage_h][bigimage_w])
{
    for (int i = 0; i < bigimage_h; i++)
    {
        for (int j = 0; j < bigimage_w; j++)
        {
            bigimage[i][j].rgbtBlue = 0;
            bigimage[i][j].rgbtGreen = 0;
            bigimage[i][j].rgbtRed = 0;
        }
    }
}

int calculateaveragedenominator(int row, int column, int height, int width)
{
    int denominator = 9;

    // The pixel is at the top left, top right, bottom left, or bottom right corner
    if ((row == 0 && column == 0) ||
        (row == 0 && column == (width - 1)) ||
        (row == (height - 1) && column == 0) ||
        (row == (height - 1) && column == (width - 1)))
    {
        denominator = 4;
    }
    // The pixel is on the top, left, right, or bottom edge
    else if ((row == 0 && column > 0 && column < (width - 1)) ||
             (column == 0 && row > 0 && row < (height - 1)) ||
             (column == (width - 1) && row > 0 && row < (height - 1)) ||
             (row == (height - 1) && column > 0 && column < (width - 1)))
    {
        denominator = 6;
    }

    return denominator;
}

RGBTRIPLE calculatergbaverages(RGBTRIPLE *pixels, int arraysize, int averagedenominator)
{
    RGBTRIPLE temp;
    double redaverage = 0;
    double blueaverage = 0;
    double greenaverage = 0;

    for (int i = 0; i < arraysize; i++)
    {
        redaverage += pixels[i].rgbtRed;
        greenaverage += pixels[i].rgbtGreen;
        blueaverage += pixels[i].rgbtBlue;
    }

    temp.rgbtRed = round(redaverage / averagedenominator);
    temp.rgbtGreen = round(greenaverage / averagedenominator);
    temp.rgbtBlue = round(blueaverage / averagedenominator);

    return temp;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int kernelsize = 9;
    int arraysize = kernelsize;
    RGBTRIPLE pixels[kernelsize];
    // NOTE: make sure to cap gx gy values at 255
    int bigimage_h = height + 2;
    int bigimage_w = width + 2;
    // Create a temp image with a 1 pixel empty border
    RGBTRIPLE bigimage[bigimage_h][bigimage_w];

    // Make sure array is empty
    initializebigimage(bigimage_h, bigimage_w, bigimage);

    // Copy contents from image into the bigimage
    for (int i = 1; i < (bigimage_h - 1); i++)
    {
        for (int j = 1; j < (bigimage_w - 1); j++)
        {
            bigimage[i][j] = image[i - 1][j - 1];
        }
    }

    // Use the bigimage to calculate new rgb values and assign them to image
    for (int i = 1; i < (bigimage_h - 1); i++)
    {
        for (int j = 1; j < (bigimage_w - 1); j++)
        {
            // 1st row
            pixels[0] = bigimage[i - 1][j - 1];
            pixels[1] = bigimage[i - 1][j];
            pixels[2] = bigimage[i - 1][j + 1];
            // // 2nd row
            pixels[3] = bigimage[i][j - 1];
            pixels[4] = bigimage[i][j];
            pixels[5] = bigimage[i][j + 1];
            // // 3rd row
            pixels[6] = bigimage[i + 1][j - 1];
            pixels[7] = bigimage[i + 1][j];
            pixels[8] = bigimage[i + 1][j + 1];

            // // Calculate the blur rgb values for the given pixel
            RGBTRIPLE sobelpixel = applysobeloperator(pixels, arraysize);

            // Overwrite pixel in original image with the blurred pixel
            image[i - 1][j - 1].rgbtBlue = sobelpixel.rgbtBlue;
            image[i - 1][j - 1].rgbtGreen = sobelpixel.rgbtGreen;
            image[i - 1][j - 1].rgbtRed = sobelpixel.rgbtRed;
        }
    }

    return;
}

RGBTRIPLE applysobeloperator(RGBTRIPLE *pixels, int arraysize)
{
    RGBTRIPLE temp;
    int gx[9] = {-1,  0,  1, -2, 0, 2, -1, 0, 1};
    int gy[9] = {-1, -2, -1,  0, 0, 0,  1, 2, 1};
    int x_red = 0;
    int x_green = 0;
    int x_blue = 0;
    int y_red = 0;
    int y_green = 0;
    int y_blue = 0;

    for (int i = 0; i < arraysize; i++)
    {
        // X
        x_red += pixels[i].rgbtRed * gx[i];
        x_green += pixels[i].rgbtGreen * gx[i];
        x_blue += pixels[i].rgbtBlue * gx[i];
        // Y
        y_red += pixels[i].rgbtRed * gy[i];
        y_green += pixels[i].rgbtGreen * gy[i];
        y_blue += pixels[i].rgbtBlue * gy[i];
    }

    int red = round(sqrt(x_red * x_red + y_red * y_red));
    int green = round(sqrt(x_green * x_green + y_green * y_green));
    int blue = round(sqrt(x_blue * x_blue + y_blue * y_blue));

    // Cap rgb values at 255 individually
    temp.rgbtRed =  red > 255 ? 255 : red;
    temp.rgbtGreen = green > 255 ? 255 : green;
    temp.rgbtBlue = blue > 255 ? 255 : blue;

    return temp;
}