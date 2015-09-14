//
//  main.cpp
//  ee569_hw1_p2.2
//
//  Image Filtering - Creating Oil Painting Effect
//  Step 2. Impose oil painting effect on quantized 24-bit RGB raw image (barn64.raw is defaulted)
//
//  The idea is to build an N*N "window" which contains NxN surrounding pixels for each point,
//  and then assign the most frequent RGB combination for the central point.
//
//  Input:
//  Raw quantized image; path of desired output image; (height of image); (width of image);
//
//  Output:
//  oil-painted effected raw-format image, argv[2];
//
//  Created by Yun-Jun Lee on 9/7/15.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef OILPAINT_QTIMG
#define OILPAINT_QTIMG

using namespace std;

unsigned char MostFrequentColor(int RGBarray[] ,int row_number, int OutputChannel){
    int ColorCount[row_number];
    for (int i=0; i<row_number; i++) {
        ColorCount[i]=0;
        for (int j=i+1; j<row_number; j++) {
            if (RGBarray[i*3] == RGBarray[j*3] && RGBarray[i*3+1] == RGBarray[j*3+1] && RGBarray[i*3+2] == RGBarray[j*3+2]) {
                ColorCount[i]++;
            }
        }
    }
    int MaxAppearance=0;
    int MostFrequent=0;
    for (int i=0; i<row_number; i++) {
        if (ColorCount[i]>MaxAppearance) {
            MaxAppearance=ColorCount[i];
            MostFrequent=i;
        }
    }
    return (unsigned char)round(RGBarray[MostFrequent*3+OutputChannel]);
}

int main(int argc, const char * argv[])
{
    // Define the variables
    FILE *file;
    int BytesPerPixel = 3, height = 380, width = 275;    // Default height/width value (barn.raw)
    int WindowSize = 5;                            // Default window size
    int h = floor(WindowSize/2);                                   // Half of windor size
    
    // argv[1] = "/Users/YJLee/Desktop/barn64.raw"          // argv[1] = "/Users/YJLee/Desktop/coliseum64.raw"
    // argv[2] = "/Users/YJLee/Desktop/barn_oil.raw"        // argv[2] = "/Users/YJLee/Desktop/coliseum_oil.raw"
    // argv[3] = 380                                        // argv[3] = 580
    // argv[4] = 275                                        // argv[4] = 247
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image64.raw input_image's_height input_image's_width" << endl;
        return 0;
    }
    // Get specific size of input image
    if (argc >= 5){
        height = (int)atoi(argv[3]);
        width = (int)atoi(argv[4]);
    }
    // Read the image contents by fread(ptr,Size,count,fp)
    
    unsigned char ImageQuantize[height][width][BytesPerPixel];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(ImageQuantize, sizeof(unsigned char), height*width*BytesPerPixel, file);
    fclose(file);
    
    //////////////////////////////////////////////////////////////////////////////////
    // Assign each pixel new RGB value, according to the most frequent combination in the window.
    unsigned char ImageOutput[height][width][BytesPerPixel];
    
    for (int y = 0; y < height; y ++) {
        for(int x = 0; x < width; x++) {
            
            int Window[]={0};                                                                // RGB combination of all pixels in the window
            int PixelCount=0;
            
            for (int ch=0; ch<BytesPerPixel; ch++) {
                for (int j= y-h; j<=y+h; j++) {                      // Apply the window on each one single Pixel
                    for (int i= x-h; i<=x+h; i++) {
                        if (j >= 0 && j <= height && i >= 0 && i <= width) {                              // Make sure pixel in both window and image
                            Window[PixelCount*BytesPerPixel+ch]= ImageQuantize[j][i][ch];                // Get all RGB combinations
                        }
                    }
                }
            }
            
            PixelCount = sizeof(Window)/3;
            cout << PixelCount << endl;
            
            for (int ch=0; ch< BytesPerPixel; ch++) {                                                 // Apply the most frequent color to the output pixel.
                ImageOutput[(int)y][(int)x][ch] = MostFrequentColor(Window, PixelCount, ch);
            }

            }
        }
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save img file" << endl;
        exit(1);
    }
    fwrite(ImageOutput, sizeof(unsigned char), (height)*(width)*BytesPerPixel, file);
    fclose(file);
    
    return 0;
}
#endif