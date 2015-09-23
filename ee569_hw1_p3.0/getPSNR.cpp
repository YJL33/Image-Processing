//
//  main.cpp
//  HW1_P3
//
//  Get PSNR
//  The code calculates PSNR.
//
//  Input: Raw noise-free square image, Comparing image, (input size)
//  Output: De-noised square image, PSNR value
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef GET_PSNR
#define GET_PSNR

using namespace std;

int main(int argc, const char * argv[]) {
    clock_t begin = clock();
    FILE *file;
    int BytesPerPixel=3, Size = 512;                                            // Define the variables
    double MSE[BytesPerPixel], PSNR[BytesPerPixel];                             // Calculate PSNR
    
    //argv[1] = "/Users/YJLee/Desktop/peppers.raw";
    //argv[2] = "/Users/YJLee/Desktop/peppers_rm_impulse.raw";
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 3] [Size = 512]" << endl;
        return 0;
    }
    
    // Check if size is specified
    if (argc >= 4){
        Size = atoi(argv[3]);
    }
    printf("Size of image: %d x %d\n", Size, Size);

    // Read the noise-free image contents by fread(ptr,size,count,fp)
    unsigned char Imagedata[Size][Size][BytesPerPixel];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    
    // Read the filtered image contents by fread(ptr,size,count,fp)
    unsigned char ImageFiltered[Size][Size][BytesPerPixel];
    
    if (!(file=fopen(argv[2],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Print PSNR
    for (int ch = 0; ch < BytesPerPixel; ch++) {
        MSE[ch] = 0;
        for (int y = 0; y < Size; y++) {
            for (int x = 0; x < Size; x++) {
                MSE[ch] += ((double)pow((double)(ImageFiltered[y][x][ch]-Imagedata[y][x][ch]), 2.0));
            }
        }
        MSE[ch] = MSE[ch]/(double)(pow(Size, 2.0));
        PSNR[ch] = 10*log10((double)(255*255)/(MSE[ch]));
    }
    printf("PSNR in R: %.3f, PSNR in G: %.3f, PSNR in B: %.3f\n",PSNR[0], PSNR[1], PSNR[2]);
    printf("MSE in R: %.3f, MSE in G: %.3f, MSE in B: %.3f\n",MSE[0], MSE[1], MSE[2]);
    ///////////////////////////////////////////////////////////////////////////////////////////////
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif