//
//  StructureElement.cpp
//  HW3_P3.a.2
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
#ifndef STRUCTURE_ELEMENT
#define STRUCTURE_ELEMENT

using namespace std;

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int BytesPerPixel=1, height = 108, width = 91;                      // Define the variables
    bool inverseImage=false;

    //argv[1] = "/Users/YJLee/Desktop/Horseshoe.raw";
    //argv[2] = "/Users/YJLee/Desktop/Horseshoe_shrink2_black.raw";
    //argv[3] = 0
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [input_image.raw] [output_image.raw] [Object, 0=White 1=Black]" << endl;
        return 0;
    }
    if ((argc = 4) && argv[3] !=0) {
        inverseImage=true;
        cout << "<<<<Black pixels as object in Horseshoe.raw>>>>" << endl;
    }
    else {
        cout << "<<<<White pixels as object in Horseshoe.raw>>>>" << endl;
    }
    
    // Read the image contents by fread(ptr,size,count,fp)
    unsigned char Imagedata[height][width][BytesPerPixel];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), height*width*BytesPerPixel, file);
    fclose(file);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////
    
    unsigned char ImageOutput[height][width][BytesPerPixel];
    
    // Step 0. Make it into 0 or 1
    if (inverseImage == false) {
        for (int y=0; y<height; ++y) {
            for (int x=0; x<width; ++x) {
                ImageOutput[y][x][0] = (Imagedata[y][x][0])/255;
            }
        }
    }
    else
    {
        for (int y=0; y<height; ++y) {
            for (int x=0; x<width; ++x) {
                ImageOutput[y][x][0] = (255-Imagedata[y][x][0])/255;
            }
        }
    }
    
    // Extend the boundary
    bool ImageAddFrame[height+2][width+2][1];
    
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            ImageAddFrame[y+1][x+1][0] = ImageOutput[y][x][0];
        }
    }
    // copy upper and lower boundaries
    for (int x=0; x<width; x++) {
        ImageAddFrame[0][x+1][0] = ImageOutput[0][x][0];
        ImageAddFrame[height+1][width+1][0] = ImageOutput[height-1][x][0];
    }
    // copy left and right boundaries from itself
    for (int y=0; y<(height+2); y++) {
        ImageAddFrame[y][0][0] = ImageAddFrame[y][1][0];
        ImageAddFrame[y][width+1][0] = ImageAddFrame[y][width][0];
    }

    
    // Step 1. Dilation
    unsigned char ImageDil[height][width][BytesPerPixel];
    for (int y=0; y<height; ++y) {
        for (int x=0; x<width; ++x) {
            if (inverseImage == false) {
                ImageDil[y][x][0] = 255*(ImageAddFrame[y][x][0] || ImageAddFrame[y+1][x][0]);
            }
            else {
                ImageDil[y][x][0] = 255*(ImageAddFrame[y][x][0] || ImageAddFrame[y+1][x][0] || ImageAddFrame[y-1][x][0] || ImageAddFrame[y][x+1][0] || ImageAddFrame[y][x-1][0]);
            }
            //printf("(%d, %d)= %d\n",x,y,ImageDil[y][x][0]);
        }
    }
    
    // Extend the boundary again
    bool ImageAddFrame2[height+2][width+2][1];
    
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            ImageAddFrame2[y+1][x+1][0] = ImageDil[y][x][0];
        }
    }
    // copy upper and lower boundaries
    for (int x=0; x<width; x++) {
        ImageAddFrame2[0][x+1][0] = ImageDil[0][x][0];
        ImageAddFrame2[height+1][width+1][0] = ImageDil[height-1][x][0];
    }
    // copy left and right boundaries from itself
    for (int y=0; y<(height+2); y++) {
        ImageAddFrame2[y][0][0] = ImageAddFrame[y][1][0];
        ImageAddFrame2[y][width+1][0] = ImageAddFrame[y][width][0];
    }

    
    // Step 2. Erosion
    unsigned char ImageEro[height][width][BytesPerPixel];
    for (int y=0; y<height; ++y) {
        for (int x=0; x<width; ++x) {
            if (inverseImage == false){
                ImageEro[y][x][0] = 255*(ImageAddFrame2[y+2][x][0] && ImageAddFrame2[y+1][x+1][0] && ImageAddFrame2[y+1][x][0] && ImageAddFrame2[y+1][x-1][0] && ImageAddFrame2[y][x][0] && ImageAddFrame2[y][x+1][0] && ImageAddFrame2[y][x-1][0] && ImageAddFrame2[y-1][x+1][0] && ImageAddFrame2[y-1][x][0] && ImageAddFrame2[y-1][x-1][0] && ImageAddFrame2[y-2][x][0]);
            }
            if (inverseImage == true) {
                ImageEro[y][x][0] = 255*(ImageAddFrame2[y][x][0] && ImageAddFrame2[y+1][x][0] && ImageAddFrame2[y-1][x][0]);
            }
            //printf("(%d, %d)= %d\n",x,y,ImageEro[y][x][0]);
        }
    }
    // Convert it back to 0 to 255
    if (inverseImage == false)
    {
        for (int y=0; y<height; ++y) {
            for (int x=0; x<width; ++x) {
                ImageOutput[y][x][0] = ImageEro[y][x][0];
                // ImageMark[y][x][0] = ImageMark[y][x][0]*255;
            }
        }
    }
    else
    {
        for (int y=0; y<height; ++y) {
            for (int x=0; x<width; ++x) {
                ImageOutput[y][x][0] = (255-ImageEro[y][x][0]);
                // ImageMark[y][x][0] = (1-ImageMark[y][x][0])*255;
            }
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////

    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(ImageOutput, sizeof(unsigned char), height*width*BytesPerPixel, file);
    fclose(file);
    cout << "Scaled image is successfully saved" <<endl;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif