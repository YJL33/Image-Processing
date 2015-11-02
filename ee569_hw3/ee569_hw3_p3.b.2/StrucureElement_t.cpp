//
//  StructureElement.cpp
//  HW3_P3.b.2
//
//  Apply the hole-filing process first,
//  using concepts of structure elements: closing process (dilation => erosion)
//
//  Steps:
//  0. Normalization (White=255, Black=0 => White=1, Black=0; assume that white is default object)
//  1. Dilation
//  2. Erosion
//  3. un-normalization
//
//
//  Input: Raw image, desired output file path
//  Output: Pretreated image
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef STRUCTURE_ELEMENT_T
#define STRUCTURE_ELEMENT_T

using namespace std;

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    // Define the variables

    //argv[1] = "/Users/YJLee/Desktop/Horse1.raw";
    //argv[2] = "/Users/YJLee/Desktop/Horse1SE.raw";
    //argv[3] = 0
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [input_image.raw] [output_image.raw] [Object, 0=White 1=Black]" << endl;
        return 0;
    }

    cout << "<<<<White pixels as object in Horseshoe.raw>>>>" << endl;
    
    // Read the image contents by fread(ptr,size,count,fp)
    unsigned char Imagedata[275][360][1];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), 275*360*1, file);
    fclose(file);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////
    
    unsigned char ImageOutput[275][360][1];
    
    // Step 0. Make it into 0 or 1
    for (int y=0; y<275; ++y) {
        for (int x=0; x<360; ++x) {
            ImageOutput[y][x][0] = (Imagedata[y][x][0])/255;
        }
    }
    
    // Extend the boundary
    bool ImageAddFrame[277][362][1];
    
    for (int y=0; y<275; y++) {
        for (int x=0; x<360; x++) {
            ImageAddFrame[y+1][x+1][0] = ImageOutput[y][x][0];
        }
    }
    // copy upper and lower boundaries
    for (int x=0; x<360; x++) {
        ImageAddFrame[0][x+1][0] = ImageOutput[0][x][0];
        ImageAddFrame[276][361][0] = ImageOutput[275-1][x][0];
    }
    // copy left and right boundaries from itself
    for (int y=0; y<(277); y++) {
        ImageAddFrame[y][0][0] = ImageAddFrame[y][1][0];
        ImageAddFrame[y][361][0] = ImageAddFrame[y][360][0];
    }

    
    // Step 1. Dilation
    bool ImageDil[275][360][1];
    for (int y=0; y<275; ++y) {
        for (int x=0; x<360; ++x) {
            ImageDil[y][x][0] =
            ((ImageAddFrame[y+3][x][0]) ||
             (ImageAddFrame[y+2][x+2][0] || ImageAddFrame[y+2][x+1][0] || ImageAddFrame[y+2][x][0] || ImageAddFrame[y+2][x-1][0] || ImageAddFrame[y+2][x-2][0]) ||
             (ImageAddFrame[y+1][x+2][0] || ImageAddFrame[y+1][x+1][0] || ImageAddFrame[y+1][x][0] || ImageAddFrame[y+1][x-1][0] || ImageAddFrame[y+1][x-2][0]) ||
             (ImageAddFrame[y][x+2][0] || ImageAddFrame[y][x+1][0] || ImageAddFrame[y][x][0] || ImageAddFrame[y][x-1][0] || ImageAddFrame[y][x-2][0]) ||
             (ImageAddFrame[y-1][x+2][0] || ImageAddFrame[y-1][x+1][0] || ImageAddFrame[y-1][x][0] || ImageAddFrame[y-1][x-1][0] || ImageAddFrame[y-1][x-2][0]) ||
             (ImageAddFrame[y-2][x+2][0] || ImageAddFrame[y-2][x+1][0] || ImageAddFrame[y-2][x][0] || ImageAddFrame[y-2][x-1][0] || ImageAddFrame[y-2][x-2][0]) ||
             (ImageAddFrame[y-3][x][0]));
            //printf("(%d, %d)= %d\n",x,y,ImageDil[y][x][0]);
        }
    }
    
    // Extend the boundary again
    for (int y=0; y<275; y++) {
        for (int x=0; x<360; x++) {
            ImageAddFrame[y+1][x+1][0] = ImageDil[y][x][0];
        }
    }
    // copy upper and lower boundaries
    for (int x=0; x<360; x++) {
        ImageAddFrame[0][x+1][0] = ImageDil[0][x][0];
        ImageAddFrame[276][361][0] = ImageDil[275-1][x][0];
    }
    // copy left and right boundaries from itself
    for (int y=0; y<(277); y++) {
        ImageAddFrame[y][0][0] = ImageAddFrame[y][1][0];
        ImageAddFrame[y][361][0] = ImageAddFrame[y][360][0];
    }

    // Step 2. Erosion
    bool ImageEro[275][360][1];
    for (int y=0; y<275; ++y) {
        for (int x=0; x<360; ++x) {
            ImageEro[y][x][0] =
            (ImageAddFrame[y+2][x][0] && ImageAddFrame[y+2][x+1][0] && ImageAddFrame[y+2][x-1][0] && ImageAddFrame[y+2][x+2][0] && ImageAddFrame[y+2][x-2][0] &&
             ImageAddFrame[y+1][x][0] && ImageAddFrame[y+1][x+1][0] && ImageAddFrame[y+1][x-1][0] && ImageAddFrame[y+1][x+2][0] && ImageAddFrame[y+1][x-2][0] &&
             ImageAddFrame[y][x][0] && ImageAddFrame[y][x+1][0] && ImageAddFrame[y][x-1][0] && ImageAddFrame[y][x+2][0] && ImageAddFrame[y][x-2][0] && ImageAddFrame[y][x+3][0] && ImageAddFrame[y][x-3][0] &&
             ImageAddFrame[y-1][x][0] && ImageAddFrame[y-1][x+1][0] && ImageAddFrame[y-1][x-1][0] && ImageAddFrame[y-1][x+2][0] && ImageAddFrame[y-1][x-2][0] &&
             ImageAddFrame[y-2][x][0] && ImageAddFrame[y-2][x+1][0] && ImageAddFrame[y-2][x-1][0] && ImageAddFrame[y-2][x+2][0] && ImageAddFrame[y-2][x-2][0]);
            //printf("(%d, %d)= %d\n",x,y,ImageEro[y][x][0]);
        }
    }
    // Convert it back to 0 to 255
    for (int y=0; y<275; ++y) {
        for (int x=0; x<360; ++x) {
            ImageOutput[y][x][0] = 255*(ImageEro[y][x][0]);
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////

    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(ImageOutput, sizeof(unsigned char), 275*360*1, file);
    fclose(file);
    cout << "Scaled image is successfully saved" <<endl;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif