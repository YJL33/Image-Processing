//
//  main.cpp
//  ee569_hw1_p2.1
//
//  Method 1
//  Using the transfer-function-based histogram equalization method to enhance the contrast of the 512x512 24bit RGB Jet image
//
//  Output:
//  No. of pixels in each RGB values (txt), argv[3]
//  Cumulative Distribution Function (txt), argv[4]
//  Enhanced Image (raw), argv[2]
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
    int ColorSpan = 255;      // The color value span we want (e.g. from 10~50 => 0~255)
    int Size = 512;
    int BytesPerPixel = 3;
    int PixelCountOfColorLevel[3][256] = {0};        // Count of pixels equal to same single value(0~255) in each channel
    double CDF[BytesPerPixel][ColorSpan];      // Cumulative Distribution Function
    
    // argv[1] = "/Users/YJLee/Desktop/jet.raw"
    // argv[2] = "/Users/YJLee/Desktop/jet_methodA.raw"
    // argv[3] = "/Users/YJLee/Desktop/jet_histA.txt"
    // argv[4] = "/Users/YJLee/Desktop/jet_cphistA.txt"
    
    // Check for proper syntax
    cout << "Argument count: " << argc << endl;
    if (argc < 5){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw output_hist.txt" << endl;
        return 0;
    }
    
    // Read the image contents by fread(ptr,Size,count,fp)
    unsigned char Imagedata[Size][Size][BytesPerPixel];
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    else {
        cout << "Image successfully loaded" <<endl;
    }
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    
    // The whole process can separate into 3 steps as follows:
    
    // 1st Step: Read count all RGB values from 0~255 of each pixels, then output the result to plot histogram.
    // e.g. R=0, count=15 pixels, ... G=24, count=52 pixels,...etc.
    
    for (int i = 0; i < Size; i ++) {
        for(int j = 0; j < Size; j++) {
            for(int channel = 0; channel < BytesPerPixel; channel++) {
                PixelCountOfColorLevel[channel][(unsigned char)round(Imagedata[i][j][channel])]++;
            }
        }
    }
    FILE * file_hist;
    if (!(file_hist = fopen(argv[3], "w"))) {
        cout << "Error: unable to save txt file" << endl;
        }
    fprintf(file_hist, "RGB,ColorSpan,Count\n");
    for (int i = 0; i < BytesPerPixel; i++){
        for(int j = 0;j < 256; j++){
            fprintf(file_hist, "%d,%d,%d\n", i, j, PixelCountOfColorLevel[i][j]);
        }
    }
    fclose(file_hist);
    
    // 2nd Step: Calculate the cumulative distribution function (c.d.f.) from the normalized probability distribution,
    // which can be derived from Pixel counts of each color level/Total pixel number.
    
    for(int channel = 0;channel < BytesPerPixel;channel++){
        double CumulativeProb = 0.0;      //Cumulative Probability of each single color value (0~255) in each channel
        for(int i = 0;i < ColorSpan;i ++){
            CumulativeProb += (double)PixelCountOfColorLevel[channel][i]/(double)(Size*Size);
            CDF[channel][i] = CumulativeProb;
        }
    }
    FILE * file_hist_CDF;
    if (!(file_hist_CDF = fopen(argv[4], "w"))) {
        cout << "Error: unable to save txt file" << endl;
    }
    fprintf(file_hist_CDF, "RGB,ColorSpan,Count\n");
    for (int i = 0; i < BytesPerPixel; i++){
        for(int j = 0;j < 256; j++){
            fprintf(file_hist_CDF, "%d,%d,%f\n", i, j, CDF[i][j]);
        }
    }
    fclose(file_hist_CDF);

    // 3rd Step: Map the c.d.f. into 0~255 distribution, and rewrite the RGB values as output image.
    // New RGB Value should be "the original RGB Value's cumulative probability" * "Color Span"
    unsigned char ImageOutput[Size][Size][BytesPerPixel];
    
    for (int i = 0; i < Size; i++) {
        for (int j = 0; j < Size; j++) {
            for (int channel = 0; channel < BytesPerPixel; channel++) {
                ImageOutput[i][j][channel] = (unsigned char)floor(CDF[channel][Imagedata[i][j][channel]]*ColorSpan);
            }
        }
    }
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    FILE *new_file;
    if (!(new_file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save img file" << endl;
        exit(1);
    }
    
    fwrite(ImageOutput, sizeof(unsigned char), (Size)*(Size)*BytesPerPixel, new_file);
    fclose(new_file);
    cout << "Contrast adjusted image successfully saved" <<endl;
    
    //Clear the memory
    memset(ImageOutput, 0, sizeof(ImageOutput));
    memset(Imagedata, 0, sizeof(Imagedata));
    
    return 0;
}