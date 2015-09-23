//
//  main.cpp
//  HW1_P3
//
//  Remove noise via Linear Filter
//  To remove noises in raw square RGB image (512x512 is defaulted according to the homework assignment)
//  The code applies a 3x3 linear filter with a square window.
//
//  Input: Raw square image, desired output file path, (input size)
//  Output: De-noised square image
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef RM_NOISE_LINEAR
#define RM_NOISE_LINEAR

using namespace std;

// This function returns value after linear filtered by an 3x3 mask.
double getLinearFilter(float weightedValue[], float weight[]) {
    int sum_v = 0;
    int sum_w = 0;
    for (int i=0; i < 9; i++) {
        sum_v += weightedValue[i];
        sum_w += weight[i];
    }
    //cout << sum_v << "," << sum_w << endl;
    return (roundf(sum_v/sum_w));
}

int main(int argc, const char * argv[]) {
    clock_t begin = clock();
    FILE *file;
    int BytesPerPixel=3, Size = 512, WindowSize = 3;                      // Define the variables
    int Mask[9] = {1,2,1,2,4,2,1,2,1};           // Linear Filter Coefficient
    
    //argv[1] = "/Users/YJLee/Desktop/peppers_noisy.raw";
    //argv[2] = "/Users/YJLee/Desktop/peppers_rm_LF.raw";
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 3] [Size = 512]" << endl;
        return 0;
    }
    
    // Check if size is specified
    if (argc >= 5){
        Size = atoi(argv[4]);
    }
    printf("Size of Linear filter (squared window): %d, Size of image: %d x %d\n", (WindowSize*WindowSize), Size, Size);

    // Read the image contents by fread(ptr,size,count,fp)
    unsigned char Imagedata[Size][Size][BytesPerPixel];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    unsigned char ImageAfterLF[Size][Size][BytesPerPixel];
    float sum[WindowSize*WindowSize], weight[WindowSize*WindowSize];
    int halfsize = floor(WindowSize/2), count =0;
    
    for (int channel = 0; channel < BytesPerPixel; channel++) {
        for (int y = 0; y < Size; y++) {
            for (int x = 0; x < Size; x++) {
                count=0;                                                                        // The effective neighbors in following window
                for (int j = (y - halfsize); j <= (y + halfsize); j++) {                        // Apply a 3x3 square window on each pixel
                    for (int i = (x - halfsize); i <= (x + halfsize); i++) {
                        sum[count] = weight[count]=0;
                        if (j >= 0 && j < Size && i >= 0 && i < Size) {                         // Effective neighbors in Square-window inside the image
                            sum[count] += Imagedata[j][i][channel]*Mask[count];
                            weight[count] += Mask[count];
                        }
                        count++;
                    }
                }
                ImageAfterLF[y][x][channel] = (unsigned char)getLinearFilter(sum,weight);
            }
        }
    }
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(ImageAfterLF, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    cout << "Image after applying Linear Filter is successfully saved" <<endl;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif