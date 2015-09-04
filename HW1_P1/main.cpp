//
//  main.cpp
//  HW1_P1
//
//  Image Resizing via Bilinear Interpolation
//  To resize a raw image, from 512x512 to 650x650 according to the homework assignment.
//
//  Created by Yun-Jun Lee on 8/29/15.
//  Copyright (c) 2015 USC. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

using namespace std;

int main(int argc, const char * argv[])
{
    // Define the variables
    
    FILE *file;
    int Size = 512;
    int new_Size = 650;
    int BytesPerPixel;
    
    cout << "Argument count: " << argc << endl;
    
    // argv[1] = "/Users/YJLee/Desktop/the_starry_night.raw"
    // argv[2] = "/Users/YJLee/Desktop/the_starry_night_2.raw"
    
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
        // Check if size is specified
    }
    
    if (argc >= 5){
        Size = atoi(argv[4]);
        new_Size = atoi(argv[5]);
        }
    
    // Determine the resizing ratio

    float ratio = (Size-1.0) / (new_Size-1.0);
    
    cout << "bytes per pixel: " << BytesPerPixel << endl;
    cout << "original size: " << Size <<endl;
    cout << "target size: " << new_Size <<endl;
    cout << "original/target ratio: " << ratio <<endl;
    
    // Read the image contents by fread(ptr,size,count,fp)
    
	unsigned char Imagedata[Size][Size][BytesPerPixel];
    
    if (!(file=fopen(argv[1],"rt"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    else {
        cout << "Image successfully loaded" <<endl;
    }
    
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);

    // Resizing via bilinear interpolation
    // The idea is to squeeze the size of output_array as the input_array.
    // According to the "squeezed position", calculate the element of each point.

    unsigned char ImageOutput[new_Size][new_Size][BytesPerPixel];
    
    for (int x = 0; x < new_Size; x++) {
        for (int  y= 0; y < new_Size; y++) {
            for(int channel = 0; channel < BytesPerPixel; channel++){
                
                // The weight of neighbor points
                
                float a;
                if ((int)floorf(ratio*y) == (int)ceilf(ratio*y)) {
                    a = 1.0;
                } else {
                    a = ceilf(ratio*y) - ratio*y;
                }
                float b;
                if ((int)floorf(ratio*x) == (int)ceilf(ratio*x)) {
                    b = 1.0;
                } else {
                    b = ceilf(ratio*x) - ratio*x;
                }
                
                // To sum four neighbor components into one point
                
                float result_all =
                (a*b*Imagedata[(int)floorf(ratio*x)][(int)floorf(ratio*y)][channel])
                +((1.0-a)*b*Imagedata[(int)floorf(ratio*x)][(int)ceilf(ratio*y)][channel])
                +(a*(1.0-b)*Imagedata[(int)ceilf(ratio*x)][(int)floorf(ratio*y)][channel])
                +((1.0-a)*(1.0-b)*Imagedata[(int)ceilf(ratio*x)][(int)ceilf(ratio*y)][channel])
                +0.5;
                
                ImageOutput[x][y][channel] = static_cast<unsigned char>(result_all*0.8);            //Bug: can work only this way
            }
        }
    }
    
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    
    FILE *new_file;
    if (!(new_file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(ImageOutput, sizeof(unsigned char), new_Size*new_Size*BytesPerPixel, new_file);
    fclose(new_file);
    cout << "Scaled image successfully saved" <<endl;

    return 0;
}