//
// inverseBlackAndWhite.cpp
//  HW2_P2.b.2
//
//  Inverse black and white
//
//  Input:  [program_name]  [input_image.raw]
//
//  Load the image, inverse the image, and output the image.
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef INVERSEBLACKANDWHITE
#define INVERSEBLACKANDWHITE

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE * file;
    int Height = 321, Width = 481;        // Define the variables
    int Threshold = 56;
    
    // argv[1] = "/Users/YJLee/Desktop/INPUT.raw";
    // argv[2] = "/Users/YJLee/Desktop/OUTPUT.array";
    
    // Check for proper syntax
    if (argc < 3)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [input image] [output image]" << endl;
        return 0;
    }
    
    // Read all the features by fread(ptr,size,count,fp)
    unsigned char Imagedata[Height][Width];
    if (!(file=fopen(argv[1],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Height*Width, file);
    fclose(file);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    unsigned char ImageOutput[Height][Width];
    
    // Thresholding
    for (int y=0; y<Height; y ++) {
        for (int x=0; x<Width; x ++) {
            ImageOutput[y][x] = 255-Imagedata[y][x];
        }
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save threshold image" << endl;
        exit(1);
    }
    fwrite(ImageOutput, sizeof(unsigned char), Height*Width, file);
    fclose(file);
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif