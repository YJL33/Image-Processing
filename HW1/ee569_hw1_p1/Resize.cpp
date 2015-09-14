//
//  main.cpp
//  HW1_P1
//
//  Image Resizing via Bilinear Interpolation
//  To resize a raw square image (512x512 to 650x650 is defaulted according to the homework assignment)
//
//  Input: Raw square image, desired output file path, (Number of color channel), (input size), (output size)
//  Output: Resized square image
//
//  Created by Yun-Jun Lee on 8/29/15.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef RESIZE_RAWIMG
#define RESIZE_RAWIMG

using namespace std;

int main(int argc, const char * argv[])
{
    // Define the variables
    
    FILE *file;
    int BytesPerPixel, Size = 512, new_Size = 650;
    float ratio = (Size-1.0) / (new_Size-1.0);                          // the resizing ratio
    
    //argv[1] = "/Users/YJLee/Desktop/the_starry_night.raw";
    //argv[2] = "/Users/YJLee/Desktop/the_starry_night_scaled.raw";
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 3] [Size = 512]" << endl;
        return 0;
    }
    
    // Check if image is grayscale or color
    if (argc < 4){
        BytesPerPixel = 3; // default is color image
    }
    else {
        BytesPerPixel = atoi(argv[3]);
    }
    // Check if size is specified
    if (argc >= 5){
        Size = atoi(argv[4]);
        new_Size = atoi(argv[5]);
    }
    
    // Read the image contents by fread(ptr,size,count,fp)
    unsigned char Imagedata[Size][Size][BytesPerPixel];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // Resizing via bilinear interpolation
    // The idea is to squeeze the size of output_array as the input_array.
    // According to the "squeezed position", calculate the element of each point.
    
    unsigned char ImageOutput[new_Size][new_Size][BytesPerPixel];
    
    for (int x = 0; x < new_Size; x++) {
        for (int  y= 0; y < new_Size; y++) {
            for(int channel = 0; channel < BytesPerPixel; channel++){
                
                float a, b;                                                  // The weight of neighbor points
                if ((int)floorf(ratio*y) == (int)ceilf(ratio*y)) {
                    a = 1.0;
                } else {
                    a = ceilf(ratio*y) - ratio*y;
                }
                if ((int)floorf(ratio*x) == (int)ceilf(ratio*x)) {
                    b = 1.0;
                } else {
                    b = ceilf(ratio*x) - ratio*x;
                }
                
                // To sum four neighbor components into one point
                ImageOutput[x][y][channel] = static_cast<unsigned char>
                (roundf((a*b*Imagedata[(int)floorf(ratio*x)][(int)floorf(ratio*y)][channel])
                +((1.0-a)*b*Imagedata[(int)floorf(ratio*x)][(int)ceilf(ratio*y)][channel])
                +(a*(1.0-b)*Imagedata[(int)ceilf(ratio*x)][(int)floorf(ratio*y)][channel])
                +((1.0-a)*(1.0-b)*Imagedata[(int)ceilf(ratio*x)][(int)ceilf(ratio*y)][channel])));
            }
        }
    }
    
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(ImageOutput, sizeof(unsigned char), new_Size*new_Size*BytesPerPixel, file);
    fclose(file);
    cout << "Scaled image is successfully saved" <<endl;
    return 0;
}
#endif