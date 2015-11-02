//
//  applyDithering_I4.cpp
//  HW3_P2.a
//
//  Apply dithering on a raw square image (512x512 is defaulted according to the homework assignment)
//
//  Input: Raw square image, desired output file path
//  Output: Dithered square image
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef APPLYDITHERING
#define APPLYDITHERING

using namespace std;

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int BytesPerPixel = 1, Size = 512;               // Define the variables
    int IndexNum;
    
    // argv[1] = "/Users/YJLee/Desktop/mandrill.raw"
    // argv[2] = "/Users/YJLee/Desktop/mandrill_i4.raw" (or i8)
    
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
    
    IndexNum = 4;
    
    printf("Using Index number = %d \n", IndexNum);
    
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
    
    // Bayer Index
    int i_four[] =
    {
        5, 9, 6, 10,
        13, 1, 14, 2,
        7, 11, 4, 8,
        15, 3, 12, 0
    };
    
    unsigned char ImageOutput[Size][Size][BytesPerPixel];
    
    for (int y = 0; y < Size; ++y) {
        for (int x = 0; x < Size; ++x) {
            //printf("input value = %d \n", Imagedata[y][x][0]);
            
            int element = ((y % IndexNum)*IndexNum + (x % IndexNum));
            double threshold = 255*((i_four[element] + 0.5)/pow(IndexNum, 2));
            
            //printf("threshold = %.1f, \n", threshold);
            if (Imagedata[y][x][0] < threshold)
            {
                ImageOutput[y][x][0] = 0;
                //printf("0\n");
            }
            else
            {
                ImageOutput[y][x][0] = 255;
                //printf("1\n");
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