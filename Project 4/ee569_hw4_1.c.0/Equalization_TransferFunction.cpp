//
//  main.cpp
//  (modified from ee569_hw1_p2.1)
//
//  Equalization Method A
//  Using the transfer-function-based histogram equalization method to enhance the contrast of the image
//  (Defaulted: 267x360 24bit RGB Test_night image)
//
//  Input:
//  Raw image, path of contrast enhanced image path, path of transfer function of image(txt), (imput image size)
//
//  Output:
//  enhanced raw image, argv[2]; Transfer Function (txt), argv[3]
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef EQUALIZE_TF
#define EQUALIZE_TF

using namespace std;

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int ColorSpan = 255;                                // The color value span we want (e.g. from 10~50 => 0~255)
    int Width = 267, Height = 360, BytesPerPixel = 3;   // Define the variables
    int PixelCountOfColorLevel[3][256] = {0};           // Count of pixels equal to same single value(0~255) in each channel
    int TransferFunction[3][256];                       // Transfer Function
    
    // argv[1] = "/Users/YJLee/Desktop/P4_Output/P1/Test_night.raw"
    // argv[2] = "/Users/YJLee/Desktop/P4_Output/P1/Test_night_eq.raw"
    // argv[3] = "/Users/YJLee/Desktop/P4_Output/P1/Test_night_eq.txt"
    
    // Check for proper syntax
    cout << "Argument count: " << argc << endl;
    if (argc < 4){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw output_TFd.txt" << endl;
        return 0;
    }
    // Check if size is specified
    if (argc >= 5){
        Width = atoi(argv[4]);
        Height = atoi(argv[5]);
    }
    // Read the image contents by fread(ptr,Size,count,fp)
    unsigned char Imagedata[Height][Width][BytesPerPixel];
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Height*Width*BytesPerPixel, file);
    fclose(file);
    
    // The whole process can separate into 3 steps as follows:
    
    // 1st Step: Read count all RGB values from 0~255 of each pixels.
    // e.g. R=0, count=15 pixels, ... G=24, count=52 pixels,...etc.
    
    for (int y = 0; y < Height; y ++) {
        for(int x = 0; x < Width; x++) {
            for(int channel = 0; channel < BytesPerPixel; channel++) {
                PixelCountOfColorLevel[channel][(unsigned char)round(Imagedata[y][x][channel])]++;
            }
        }
    }
    
    // 2nd Step: Calculate the Transfer Function from the cumulative probability,
    // which can be derived from probability distribution(Pixel counts of each color level/Total pixel number).
    // The Transfer Function will be "cumulative probability" * "color span"
    
    for(int channel = 0;channel < BytesPerPixel;channel++){
        double CumulativeProb = 0.0;      //Cumulative Probability of each single color value (0~255) in each channel
        for(int colorvalue = 0; colorvalue <= ColorSpan; colorvalue ++){
            CumulativeProb += (double)(PixelCountOfColorLevel[channel][colorvalue]/(double)(Height*Width));
            TransferFunction[channel][colorvalue] = floor(CumulativeProb*ColorSpan);
        }
    }
    if (!(file = fopen(argv[3], "w"))) {
        cout << "Error: unable to save txt file" << endl;
    }
    fprintf(file, "Old RGB,ColorSpan,New RGB\n");
    for (int channel = 0; channel < BytesPerPixel; channel++){
        for(int colorvalue = 0; colorvalue <= ColorSpan; colorvalue++){
            fprintf(file, "%d,%d,%d\n", channel, colorvalue, TransferFunction[channel][colorvalue]);
        }
    }
    fclose(file);
    
    // 3rd Step: Map the c.d.f. into 0~255 distribution, and rewrite the RGB values as output image.
    // New RGB Value will be generated from transfer function, by input the original RGB Value.
    unsigned char ImageOutput[Height][Width][BytesPerPixel];
    
    for (int j = 0; j < Height; j++) {
        for (int i = 0; i < Width; i++) {
            for (int channel = 0; channel < BytesPerPixel; channel++) {
                ImageOutput[j][i][channel] = (unsigned char)(TransferFunction[channel][Imagedata[j][i][channel]]);
            }
        }
    }
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save img file" << endl;
        exit(1);
    }
    
    fwrite(ImageOutput, sizeof(unsigned char), (Height)*(Width)*BytesPerPixel, file);
    fclose(file);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif