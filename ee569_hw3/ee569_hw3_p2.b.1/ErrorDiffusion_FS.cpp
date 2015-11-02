//
//  applyErrorDiffusion_FS.cpp
//  HW3_P2.b.1
//
//  Apply Floyd-Steinberg error diffusion method on a raw square image (512x512 is defaulted according to the homework assignment)
//  Using 3x3 filter
//
//  Input: Raw square image, desired output file path
//  Output: Error diffusion square image
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef ERR_DIFF_FS
#define ERR_DIFF_FS

using namespace std;

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int BytesPerPixel = 1, Size = 512;               // Define the variables
    
    // argv[1] = "/Users/YJLee/Desktop/mandrill.raw"
    // argv[2] = "/Users/YJLee/Desktop/mandrill_FS.raw" (or i8)
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [input_image.raw] [output_image.raw]" << endl;
        return 0;
    }
    
    /*
    if ((atoi(argv[3]) != 4) && (atoi(argv[3]) != 8)) {
        cout << "Syntax Error - Index Number = 4 or 8" << endl;
        return 0;
    }
     */
    
    unsigned char Imagedata[Size][Size][BytesPerPixel];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    else {
        cout << "Image successfully is loaded" <<endl;
    }
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Extend the boundary
    double MetaImage[Size+2][Size+2][BytesPerPixel];
    
    for (int y=0; y<Size; y++) {
        for (int x=0; x<Size; x++) {
            MetaImage[y+1][x+1][0] = Imagedata[y][x][0];
        }
    }
    // copy upper and lower boundaries
    for (int x=0; x<Size; x++) {
        MetaImage[0][x+1][0] = Imagedata[0][x][0];
        MetaImage[Size+1][x+1][0] = Imagedata[Size-1][x][0];
    }
    // copy left and right boundaries from itself
    for (int y=0; y<(Size+2); y++) {
        MetaImage[y][0][0] = MetaImage[y][1][0];
        MetaImage[y][Size+1][0] = MetaImage[y][Size][0];
    }
    
    unsigned char ImageOutput[Size][Size][BytesPerPixel];
    
    for (int y = 0; y < Size; ++y) {
        // Apply serpentine algorithm
        // Check whether in odd row or even row
        // even row - ascending count
        if (y%2 == 0)
        {
            for (int x = 0; x < Size; ++x)
            {
                double diff = MetaImage[y+1][x+1][0] - 127.5;
                //printf("(x, y)= %d, %d, value= %f \n", x, y, MetaImage[y+1][x+1][0]);
                if (diff <0)
                {
                    ImageOutput[y][x][0] = 0;
                }
                else
                {
                    ImageOutput[y][x][0] = 255;
                }
                double err = MetaImage[y+1][x+1][0] - ImageOutput[y][x][0];
                // "diffuse" the error to neighbor pixels
                //printf("Before: %f, %f, %f, %f \n", MetaImage[y+1][x+2][0], MetaImage[y+2][x+2][0], MetaImage[y+2][x+1][0], MetaImage[y+2][x][0]);
                MetaImage[y+1][x+2][0] += 0.4375*err;
                MetaImage[y+2][x+2][0] += 0.0625*err;
                MetaImage[y+2][x+1][0] += 0.3125*err;
                MetaImage[y+2][x][0] += 0.1875*err;
                //printf("After: %f, %f, %f, %f \n", MetaImage[y+1][x+2][0], MetaImage[y+2][x+2][0], MetaImage[y+2][x+1][0], MetaImage[y+2][x][0]);
            }
        }
        // odd row - decending count
        else
        {
            for (int x = (Size-1); x >= 0; --x)
            {
                double diff = MetaImage[y+1][x+1][0] - 127.5;
                if (diff <0)
                {
                    ImageOutput[y][x][0] = 0;
                }
                else
                {
                    ImageOutput[y][x][0] = 255;
                }
                double err = MetaImage[y+1][x+1][0] - ImageOutput[y][x][0];
                // "diffuse" the error to neighbor pixels - in oppisite way
                MetaImage[y+1][x][0] += 0.4375*err;
                MetaImage[y+2][x][0] += 0.0625*err;
                MetaImage[y+2][x+1][0] += 0.3125*err;
                MetaImage[y+2][x+2][0] += 0.1875*err;
            }
        }
    }

    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(ImageOutput, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    cout << "Scaled image is successfully saved" <<endl;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif