//
//  main.cpp
//  ee569_hw1_p2.1
//
//  Method 1
//  Using the transfer-function-based histogram equalization method to enhance the contrast of the 512x512 24bit RGB Jet image
//
//  Created by Yun-Jun Lee on 9/3/15.
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
    int BytesPerPixel = 3;
    
    // argv[1] = "/Users/YJLee/Desktop/jet.raw"
    // argv[2] = "/Users/YJLee/Desktop/jet_methodA.raw"
    
    // Check for proper syntax
    cout << "Argument count: " << argc << endl;
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw" << endl;
        return 0;
    }
    
    // Read the image contents by fread(ptr,Size,count,fp)
    unsigned char Imagedata[Size][Size][1];
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    else {
        cout << "Image successfully loaded" <<endl;
    }
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    
    // The whole process can separate into 4 steps as follows:
    
    // 1st Step: Read count all RGB values from 0~255 of each pixels, then output the result to plot histogram.
    // e.g. R=0, count=15 pixels, ... G=24, count=52 pixels,...etc.
    
    // 2nd Step: Calculate the normalized probability distribution.
    
    // 3rd Step: Calculate the cumulative distribution function. (c.d.f.)
    
    // 4th Step: Map the c.d.f. into 0~255 distribution, and rewrite the RGB values as output image.
    unsigned char ImageOutput[Size][Size][1];
    
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    FILE *new_file;
    if (!(new_file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(ImageOutput, sizeof(unsigned char), (Size)*(Size)*BytesPerPixel, new_file);
    fclose(new_file);
    cout << "Contrast adjusted image successfully saved" <<endl;
    
    //Clear the memory
    memset(ImageOutput, 0, sizeof(ImageOutput));
    
    return 0;
}