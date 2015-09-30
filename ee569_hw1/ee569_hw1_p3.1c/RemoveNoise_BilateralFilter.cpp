//
//  main.cpp
//  HW1_P3
//
//  Remove noise
//  To remove noises in raw square RGB image (512x512 is defaulted according to the homework assignment)
//  The code applies a "Bilaterial Gaussian filter" with a square window.
//
//  Input: Raw square image, desired output file path, (squared-window size), (sigma of color), (sigma of distance)
//  Output: De-noised square image
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef RM_NOISE_BILATERAL
#define RM_NOISE_BILATERAL

using namespace std;

// This function returns the Gaussian weighted neighbor based on distance
double Gaussian_space(int x, int y, int i, int j, double sigma_d) {
    return exp(-(((double)(x-i)*(x-i))+((double)(y-j)*(y-j)))/(2*(double)sigma_d*sigma_d));
}

// This function returns the Gaussian weighted neighbor based on color level
double Gaussian_colorlv(unsigned char pixel, unsigned char neighbor, double sigma_r) {
    return exp(-((double)(pixel-neighbor)*(pixel-neighbor))/(2*(double)sigma_r*sigma_r));
}

int main(int argc, const char * argv[]) {
    clock_t begin = clock();
    FILE *file;
    int BytesPerPixel=3, Size = 512, WindowSize;                                // Define the variables
    double sigma_d, sigma_r;                                                    // Gaussian parameter
    
    //argv[1] = "/Users/YJLee/Desktop/peppers_noisy.raw";
    //argv[2] = "/Users/YJLee/Desktop/peppers_rm_BF.raw";
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [Filter size = 5] [Sigma of color = 3] [Sigma of distance = 10] [Image Size = 512]" << endl;
        return 0;
    }
    
    // Check denoise parameter
    if (argc < 6){
        WindowSize = 7;
        sigma_d = 3;                                             // default
        sigma_r = 70;
    }
    else {
        WindowSize = atoi(argv[3]);
        sigma_d = atoi(argv[4]);
        sigma_r = atoi(argv[5]);
    }
    // Check if image size is specified
    if (argc >= 7){
        Size = atoi(argv[6]);
    }
    printf("Size of Gaussian filter (squared window): %d,\n Sigma of distance: %.1f, Sigma of color %.1f\n", (WindowSize*WindowSize), sigma_d, sigma_r);

    // Read the image contents by fread(ptr,size,count,fp)
    unsigned char Imagedata[Size][Size][BytesPerPixel];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    unsigned char ImageAfterBilaterial[Size][Size][BytesPerPixel];
    
    for (int y = 0; y < Size; y++) {
        for (int x = 0; x < Size; x++) {
            for (int ch = 0; ch < BytesPerPixel; ch++) {
                double weighted_colorlv_sum = 0;                            // Sum of Gaussian weighted color level
                double weight_sum = 0;                                      // Sum of Gaussian weights
                for (int j = y - floor(WindowSize/2); j <= y + floor(WindowSize/2); j++) {
                    for (int i = x - floor(WindowSize/2); i <= x + floor(WindowSize/2); i++) {
                        if (j >= 0 && j < Size && i >= 0 && i < Size) {     // Neighbors in Square-window inside the image
                            weighted_colorlv_sum += Imagedata[y][x][ch]*Gaussian_space(x, y, i, j, sigma_d)*Gaussian_colorlv(Imagedata[y][x][ch], Imagedata[j][i][ch], sigma_r);
                            weight_sum += Gaussian_space(x, y, i, j, sigma_d)*Gaussian_colorlv(Imagedata[y][x][ch], Imagedata[j][i][ch], sigma_r);
                        }
                    }
                }
                ImageAfterBilaterial[y][x][ch] = (unsigned char)((weighted_colorlv_sum/weight_sum)+0.5);  // Apply the Bilaterial filter
            }
        }
    }
    
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(ImageAfterBilaterial, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    cout << "Image after imposing Bilateral Filter is successfully saved" <<endl;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif