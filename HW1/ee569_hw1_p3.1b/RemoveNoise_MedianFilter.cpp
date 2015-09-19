//
//  main.cpp
//  HW1_P3
//
//  Remove noise
//  To remove (impulse) noises in raw square RGB image (512x512 is defaulted according to the homework assignment)
//  The code applies a median filter with a square (or cross) window.
//
//  Input: Raw square image, desired output file path, (squared-window size or cross-window arm length), (input size)
//  Output: De-noised square image
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef RM_NOISE_MEDIAN
#define RM_NOISE_MEDIAN

using namespace std;

// This code returns the median of an 1-D array from input array and input length.
unsigned char getMedian(unsigned char arr[], int len) {
    unsigned char tp;
    for (int i = 0; i < (len+3)/2; i++) {                       // No need to sort all array
        for(int j = i+1 ; j < len; j++) {
            if (arr[i] > arr[j]) {
                tp = arr[i];
                arr[i] = arr[j];
                arr[j] = tp;
            }
        }
    }
    return arr[(int)floor(len/2)];
}

int main(int argc, const char * argv[]) {
    clock_t begin = clock();
    FILE *file;
    int BytesPerPixel=3, Size = 512, WindowSize, ArmLength;                     // Define the variables
    
    //argv[1] = "/Users/YJLee/Desktop/peppers_noisy.raw";
    //argv[2] = "/Users/YJLee/Desktop/peppers_rm_impulse.raw";
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 3] [Size = 512]" << endl;
        return 0;
    }
    
    // Check denoise window size
    if (argc < 4){
        WindowSize = ArmLength =5;                                             // default is 5
    }
    else {
        WindowSize = ArmLength = atoi(argv[3]);
    }
    // Check if size is specified
    if (argc >= 5){
        Size = atoi(argv[4]);
    }
    printf("Size of Median filter (squared window): %d, Size of image: %d x %d\n", (WindowSize*WindowSize), Size, Size);
    //printf("Size of Median filter (cross window): %d, Size of image: %d x %d\n", (ArmLength*4+1), Size, Size);

    // Read the image contents by fread(ptr,size,count,fp)
    unsigned char Imagedata[Size][Size][BytesPerPixel];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    unsigned char ImageAfterMedian[Size][Size][BytesPerPixel];
    unsigned char Window[WindowSize*WindowSize];                  // Apply a square window on each pixel...
    //unsigned char Cross[4*ArmLength+1];                           // Or Apply a cross window in each pixel
    
    for (int channel = 0; channel < BytesPerPixel; channel++) {
        for (int y = 0; y < Size; y++) {
            for (int x = 0; x < Size; x++) {
                int pixel_count = 0;
                for (int j = y - floor(WindowSize/2); j <= y + floor(WindowSize/2); j++) {
                    for (int i = x - floor(WindowSize/2); i <= x + floor(WindowSize/2); i++) {
                        if (j >= 0 && j < Size && i >= 0 && i < Size) {         // Neighbors in Square-window inside the image...
                            Window[pixel_count] = Imagedata[j][i][channel];
                            pixel_count++;
                        }
                    }
                }
                //for (int j = y - ArmLength; j <= y + ArmLength; j++) {
                //    if (j >= 0 && j < Size) {                                 // Neighbors in Cross-window inside the image
                //        Cross[pixel_count] = Imagedata[j][x][channel];
                //        pixel_count++;
                //    }
                //}
                //for (int i = x - ArmLength; i <= x + ArmLength; i++) {
                //    if (i >= 0 && i < Size) {                                 // Neighbors in Cross-window inside the image
                //        Cross[pixel_count] = Imagedata[y][i][channel];
                //        pixel_count++;
                //    }
                //}
                ImageAfterMedian[y][x][channel] = getMedian(Window, pixel_count);    // Assign the median
                                                    //^^^^^^^^  Change here for filter type.
            }
        }
    }
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(ImageAfterMedian, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    cout << "Image after imposing Median Filter is successfully saved" <<endl;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif