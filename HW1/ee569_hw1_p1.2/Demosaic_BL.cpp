//
//  main.cpp
//  HW1_P1.2
//
//  Bilinear Demosaicing
//  Implement bilinear demosaicing (424x636 Parrot image is defaulted)
//
//  Input: Raw image, desired output file path, (Number of color channel), (input height), (output width)
//  Output: BL demosaic image
//
//  Created by Yun-Jun Lee on 8/31/15.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef DEMOSAIC_RAWIMG_BL
#define DEMOSAIC_RAWIMG_BL

using namespace std;

int main(int argc, const char * argv[])
{
    // Define the variables
    
    FILE *file;
    int BytesPerPixel = 3, width = 424, height = 636;
    
    // argv[1] = "/Users/YJLee/Desktop/parrot_CFA.raw"
    // argv[2] = "/Users/YJLee/Desktop/parrot_bl.raw"
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw" << endl;
        return 0;
    }
    // Check if size is specified
    if (argc >= 5){
        height = atoi(argv[4]);
        width = atoi(argv[5]);
    }

    unsigned char Imagedata[height][width][1];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    else {
        cout << "Image successfully is loaded" <<endl;
    }
    fread(Imagedata, sizeof(unsigned char), height*width*1, file);
    fclose(file);

    //////////////////////////////////////////////////////////////////////////////////////////////////////

    // extend the edge of original image with imaginary boundaries, which is needed later
    
    unsigned char ImageAddFrame[height+2][width+2][1];
    
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            ImageAddFrame[y+1][x+1][0] = Imagedata[y][x][0];
        }
    }
    // copy upper and lower boundaries
    for (int x=0; x<width; x++) {
        ImageAddFrame[0][x+1][0] = Imagedata[0][x][0];
        ImageAddFrame[height+1][width+1][0] = Imagedata[height-1][x][0];
    }
    // copy left and right boundaries from itself
    for (int y=0; y<(height+2); y++) {
        ImageAddFrame[y][0][0] = ImageAddFrame[y][1][0];
        ImageAddFrame[y][width+1][0] = ImageAddFrame[y][width][0];
    }
    
    // Bilinear Demosaicing - construct RGB value of each pixel according to Bayer pattern
    // The reconstructed color value is computed as the average of its two or four adjacent pixels of the same color.
    
    unsigned char ImageOutput[height][width][BytesPerPixel];
    
    // Construct "Red Pixel Points"
    for (int y = 0; y <= height-1; y+=2) {
        for (int x= 0; x <= width-1; x+=2) {
            ImageOutput[y][x][0] = (unsigned char)roundf(ImageAddFrame[(y+1)][x+1][0]);
            ImageOutput[y][x][1] = (unsigned char)roundf((0.25)*(ImageAddFrame[y][x+1][0]+ImageAddFrame[y+2][x+1][0]+ImageAddFrame[y+1][x][0]+ImageAddFrame[y+1][x+2][0]));
            ImageOutput[y][x][2] = (unsigned char)roundf((0.25)*(ImageAddFrame[y][x][0]+ImageAddFrame[y+2][x][0]+ImageAddFrame[y][x+2][0]+ImageAddFrame[y+2][x+2][0]));
        }
    }
    // Construct "Green Pixel Points"
    for (int y = 0; y <= height-1; y+=2) {
        for (int x= 1; x <= width-1; x+=2) {
            ImageOutput[y][x][0] = (unsigned char)roundf((0.5)*(ImageAddFrame[y+1][x][0]+ImageAddFrame[y+1][x+2][0]));
            ImageOutput[y][x][1] = (unsigned char)roundf(ImageAddFrame[y+1][x+1][0]);
            ImageOutput[y][x][2] = (unsigned char)roundf((0.5)*(ImageAddFrame[y][x+1][0]+ImageAddFrame[y+2][x+1][0]));
        }
    }
    for (int y = 1; y <= height-1; y+=2) {
        for (int x= 0; x <= width-1; x+=2) {
            ImageOutput[y][x][0] = (unsigned char)roundf((0.5)*(ImageAddFrame[y][x+1][0]+ImageAddFrame[y+2][x+1][0]));
            ImageOutput[y][x][1] = (unsigned char)roundf(ImageAddFrame[y+1][x+1][0]);
            ImageOutput[y][x][2] = (unsigned char)roundf((0.5)*(ImageAddFrame[y+1][x][0]+ImageAddFrame[y+1][x+2][0]));
        }
    }
    // Construct "Blue Pixel Points"
    for (int y = 1; y <= height-1; y+=2) {
        for (int x= 1; x <= width-1; x+=2) {
            ImageOutput[y][x][0] = (unsigned char)roundf((0.25)*(ImageAddFrame[y][x][0]+ImageAddFrame[y+2][x][0]+ImageAddFrame[y][x+2][0]+ImageAddFrame[y+2][x+2][0]));
            ImageOutput[y][x][1] = (unsigned char)roundf((0.25)*(ImageAddFrame[y][x+1][0]+ImageAddFrame[y+2][x+1][0]+ImageAddFrame[y+1][x][0]+ImageAddFrame[y+1][x+2][0]));
            ImageOutput[y][x][2] = (unsigned char)roundf(ImageAddFrame[y+1][x+1][0]);
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    fwrite(ImageOutput, sizeof(unsigned char), (height)*(width)*BytesPerPixel, file);
    fclose(file);
    cout << "Bi-linear demosaic image is successfully saved" <<endl;

    return 0;
}
#endif