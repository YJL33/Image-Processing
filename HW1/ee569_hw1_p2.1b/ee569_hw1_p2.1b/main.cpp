//
//  main.cpp
//  ee569_hw1_p2.1
//
//  Method B
//  Using the "Bucket Filling" method to enhance the contrast of the 512x512 24bit RGB Jet image
//  In this qustion, each bucket has exactly (512X512)/256 = 1024
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
    int ColorSpan = 256;      // The color value span we want to expand (e.g. from 10~50 => 0~255)
    int Size = 512;
    int BytesPerPixel = 3;
    int BucketSize;
    int MinLevel[3] = {0};    // The minimum color value in each channel
    int MaxLevel[3] = {0};    // The maximum color value in each channel
    int PixelCountOfColorLevelBefore[3][256] = {0};        // Count of pixels equal to same single value(0~255) in each channel
    int PixelCountOfColorLevelAfter[3][256] = {0};        // Count of pixels after processing
    unsigned char Imagedata[Size][Size][BytesPerPixel];
    unsigned char ImageOutput[Size][Size][BytesPerPixel];
    
    // argv[1] = "/Users/YJLee/Desktop/jet.raw"
    // argv[2] = "/Users/YJLee/Desktop/jet_B.raw"
    // argv[3] = "/Users/YJLee/Desktop/jet_hist_before_B.txt"
    // argv[4] = "/Users/YJLee/Desktop/jet_hist_after_B.txt"
    
    // Check for proper syntax
    cout << "Argument count: " << argc << endl;
    if (argc < 5){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw input_hist.txt output_hist.txt" << endl;
        return 0;
    }
    
    // Read the image contents by fread(ptr,Size,count,fp)
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
    // Meahwhile, get the minimum and maximum value of each channel.
    
    for (int y = 0; y < Size; y ++) {
        for(int x = 0; x < Size; x++) {
            for(int channel = 0; channel < BytesPerPixel; channel++) {
                PixelCountOfColorLevelBefore[channel][(unsigned char)round(Imagedata[y][x][channel])]++;
            }
        }
    }
    for (int channel = 0; channel < BytesPerPixel; channel++) {
        for (int colorvalue = ColorSpan; colorvalue>0 ; colorvalue--) {
            if (PixelCountOfColorLevelBefore[channel][colorvalue] != 0) {
                MinLevel[channel] = colorvalue;
            }
        }
        for (int colorvalue = 0; colorvalue<ColorSpan ; colorvalue++) {
            if (PixelCountOfColorLevelBefore[channel][colorvalue] != 0) {
                MaxLevel[channel] = colorvalue;
            }
        }
    }
    printf("MinR: %d, MaxR: %d\n", MinLevel[0], MaxLevel[0]);
    printf("MinG: %d, MaxG: %d\n", MinLevel[1], MaxLevel[1]);
    printf("MinB: %d, MaxB: %d\n", MinLevel[2], MaxLevel[2]);
    
    if (!(file = fopen(argv[3], "w"))) {
        cout << "Error: unable to save txt file" << endl;
    }
    fprintf(file, "RGB,ColorSpan,Count\n");
    for (int channel = 0; channel < BytesPerPixel; channel++){
        for(int colorvalue = 0; colorvalue < ColorSpan; colorvalue++){
            fprintf(file, "%d,%d,%d\n", channel, colorvalue, PixelCountOfColorLevelBefore[channel][colorvalue]);
        }
    }
    fclose(file);
    
    // 2nd Step: Calculate "Bucket Size"
    BucketSize = Size*Size/ColorSpan;
    
    // 3rd Step: Allocate pixels in Output Image to new RGB level, based on original RGB level from minimum to maximum, pixel by pixel.
    int count =0;
    int bucketNo=0;  // "Bucket number": means the desired level in output image (here from 0 to 255)
    
    for (int channel=0; channel<BytesPerPixel; channel++) {
        for (int colorvalue=MinLevel[channel]; colorvalue<=MaxLevel[channel]; colorvalue++) {    // Begin with minimum value
            for (int y=0; y<Size; y++) {
                for  (int x=0; x<Size; x++){
                        if (Imagedata[y][x][channel] == colorvalue) {
                            ImageOutput[y][x][channel] = (unsigned char)round(bucketNo);
                            count++;
                        }
                        if (count == BucketSize) {
                            bucketNo++;    // If the "bucket" is fulled, change one.
                            count = 0;
                        }
                    }
                }
            }
        }

    // Count all RGB values from 0~255 of each pixels from output image. (in order to output and plot histogram)
    for (int y = 0; y < Size; y++) {
        for(int x = 0; x < Size; x++) {
            for(int channel = 0; channel < BytesPerPixel; channel++) {
                PixelCountOfColorLevelAfter[channel][(unsigned char)round(ImageOutput[y][x][channel])]++;
            }
        }
    }
    if (!(file = fopen(argv[4], "w"))) {
        cout << "Error: unable to save txt file" << endl;
    }
    fprintf(file, "RGB,ColorSpan,Count\n");
    for (int channel = 0; channel < BytesPerPixel; channel++){
        for(int colorvalue = 0; colorvalue < ColorSpan; colorvalue++){
            fprintf(file, "%d,%d,%d\n", channel, colorvalue, PixelCountOfColorLevelAfter[channel][colorvalue]);
        }
    }
    fclose(file);
    
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save img file" << endl;
        exit(1);
    }
    
    fwrite(ImageOutput, sizeof(unsigned char), (Size)*(Size)*BytesPerPixel, file);
    fclose(file);
    cout << "Contrast adjusted image successfully saved" <<endl;
    
    //Clear the memory
    memset(ImageOutput, 0, sizeof(ImageOutput));
    memset(Imagedata, 0, sizeof(Imagedata));
    
    return 0;
}