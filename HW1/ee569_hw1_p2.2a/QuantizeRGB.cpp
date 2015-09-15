//
//  main.cpp
//  ee569_hw1_p2.2
//
//  Image Filtering - Creating Oil Painting Effect
//  Step 1. Quantizing original 24-bit RGB raw image (barn.raw is defaulted)
//
//  Based on color level, separate each channel into four parts in equal number.
//  Then assign new RGB values for each of them, based on average RGB value of each part.
//
//  Input:
//  Raw image; path of desired quantized image; (height of image); (width of image);
//
//  Output:
//  enhanced raw image, argv[2];
//
//  Created by Yun-Jun Lee on 9/7/15.
//  Copyright (c) 2015 USC. All rights reserved.
//
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef QUANTIZE_RAWIMG
#define QUANTIZE_RAWIMG

using namespace std;

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int ColorSpan = 256, BytesPerPixel = 3, width = 380, height = 275;    // Default height/width value (barn.raw)
    int NumberOfPixels = height*width;
    int PixelCount[3][256] = {0};                         // number of pixels of certain Color Level
    int Quartile[3][4] = {0};                             // quartile[channel][first, median, third]
    unsigned char NewRGBLv[3][4] = {0};                   // quantized color level for each channel
    int bin[3][4] = {0}, sum[3][4] = {0};                 // refer to step c.

    // argv[1] = "/Users/YJLee/Desktop/barn.raw"          // argv[1] = "/Users/YJLee/Desktop/coliseum.raw"
    // argv[2] = "/Users/YJLee/Desktop/barn64.raw"        // argv[2] = "/Users/YJLee/Desktop/coliseum64.raw"
    // argv[3] = 380                                      // argv[3] = 580
    // argv[4] = 275                                      // argv[4] = 247

    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image64.raw (input_image's_height) (input_image's_width)" << endl;
        return 0;
    }
    // Get specific size of input image
    if (argc >= 5){
         width= (int)atoi(argv[3]);
         height= (int)atoi(argv[4]);
    }
    printf("image width: %d , image height: %d\n", width, height);
    // Read the image contents by fread(ptr,Size,count,fp)
    unsigned char Imagedata[height][width][BytesPerPixel];
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), height*width*BytesPerPixel, file);
    fclose(file);
    //////////////////////////////////////////////////////////////////////////////////
    // a. Read count all RGB values from 0~255 of each pixels.
    for (int y = 0; y < height; y ++) {
        for(int x = 0; x < width; x++) {
            for(int channel = 0; channel < BytesPerPixel; channel++) {
                PixelCount[channel][(unsigned char)round(Imagedata[y][x][channel])]++;
            }
        }
    }
    // b. Find out quartiles.
    for (int channel=0; channel<BytesPerPixel; channel++) {
        int sum=0, quartile_count=0;
        for (int colorvalue =0; colorvalue <ColorSpan; colorvalue++) {
            sum += PixelCount[channel][colorvalue];
            if (sum >= (NumberOfPixels/4)) {
                Quartile[channel][quartile_count] = colorvalue;
                quartile_count++;
                sum -= (NumberOfPixels/4);
            }
        }
    }
    // c. Divide each channel into 4 bin based on quartiles, then calculate each bin's average color intensity.
    unsigned char ImageQuantize[height][width][BytesPerPixel];
    for (int lv=0; lv<ColorSpan; lv++) {
        for (int y=0; y<height; y++) {
            for (int x=0; x<width; x++) {
                for (int ch=0; ch<BytesPerPixel; ch++) {
                    if (round(Imagedata[y][x][ch])==lv){
                        if (lv<=Quartile[ch][0] && bin[ch][0]<(NumberOfPixels/4) ) {
                            ImageQuantize[y][x][ch]=0;
                            sum[ch][0]+=lv;
                            bin[ch][0]++;
                        }
                        if (lv>=Quartile[ch][0] && lv<=Quartile[ch][1] && bin[ch][1]<(NumberOfPixels/4)) {
                            ImageQuantize[y][x][ch]=1;
                            sum[ch][1]+=lv;
                            bin[ch][1]++;
                        }
                        if (lv>=Quartile[ch][1] && lv<=Quartile[ch][2] && bin[ch][2]<(NumberOfPixels/4)) {
                            ImageQuantize[y][x][ch]=2;
                            sum[ch][2]+=lv;
                            bin[ch][2]++;
                        }
                        if (lv>=Quartile[ch][2] && bin[ch][3]<(NumberOfPixels/4)) {
                            ImageQuantize[y][x][ch]=3;
                            sum[ch][3]+=lv;
                            bin[ch][3]++;
                        }
                    }
                }
            }
        }
    }
    for (int ch=0; ch<BytesPerPixel; ch++) {
        for (int qt=0; qt<4; qt++) {
            NewRGBLv[ch][qt] = (unsigned char)round((double)sum[ch][qt]/(double)bin[ch][qt]);
        }
    }
    //printf("Quantize R: %d, %d, %d, %d\n", (NewRGBLv[0][0]),(NewRGBLv[0][1]),(NewRGBLv[0][2]),(NewRGBLv[0][3]));
    //printf("Quantize G: %d, %d, %d, %d\n", (NewRGBLv[1][0]),(NewRGBLv[1][1]),(NewRGBLv[1][2]),(NewRGBLv[1][3]));
    //printf("Quantize B: %d, %d, %d, %d\n", (NewRGBLv[2][0]),(NewRGBLv[2][1]),(NewRGBLv[2][2]),(NewRGBLv[2][3]));
    
    // d. Assign new RGB values (only 4 different levels) to each pixel.
    for (int ch=0; ch<BytesPerPixel; ch++) {
        for (int y=0; y<height; y++) {
            for (int x=0; x<width; x++) {
                for (int lv=0; lv<4; lv++) {
                    if (ImageQuantize[y][x][ch] == 0) {
                        ImageQuantize[y][x][ch]=(unsigned char)NewRGBLv[ch][0];
                    }
                    if (ImageQuantize[y][x][ch] == 1) {
                        ImageQuantize[y][x][ch]=(unsigned char)NewRGBLv[ch][1];
                    }
                    if (ImageQuantize[y][x][ch] == 2) {
                        ImageQuantize[y][x][ch]=(unsigned char)NewRGBLv[ch][2];
                    }
                    if (ImageQuantize[y][x][ch] == 3) {
                        ImageQuantize[y][x][ch]=(unsigned char)NewRGBLv[ch][3];
                    }
                }
            }
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save img file" << endl;
        exit(1);
    }
    fwrite(ImageQuantize, sizeof(unsigned char), (height)*(width)*BytesPerPixel, file);
    fclose(file);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif