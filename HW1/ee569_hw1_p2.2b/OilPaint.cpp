//
//  main.cpp
//  ee569_hw1_p2.2
//
//  Image Filtering - Creating Oil Painting Effect
//  Step 2. Impose oil painting effect on quantized 24-bit RGB raw image (barn64.raw is defaulted)
//
//  The idea is to build an customized N*N "window" which contains NxN surrounding pixels for each point,
//  and then assign the most frequent RGB combination for the central point.
//
//  Input:
//  Raw quantized image; path of desired output image; (window size); (height of image); (width of image);
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

// this function outputs the corresponding component of most frequent "color" (RGB combination), based on Window size, effective pixels, and desired channel.

unsigned char getComponentOfMostFrequentRGB(unsigned char RGBarray[] ,int pixels_number, int OutputChannel){
        
    int color_index[pixels_number];                         // Index each pixel's RGB combination
    int color_count[pixels_number];                         // Count each RGB combination's appearance
        
    for (int i=0; i<pixels_number; i++) {
        color_index[i] = i;
        color_count[i] = 1;
    }
    for (int i=0; i<pixels_number; i++) {                   // Change repeated combination's index into same one
        if (i == color_index[i]) {
        for (int j=i+1; j<pixels_number; j++) {
            if (RGBarray[i*3] == RGBarray[j*3] && RGBarray[i*3+1] == RGBarray[j*3+1] && RGBarray[i*3+2] == RGBarray[j*3+2]) {
                color_index[j] = color_index[i];
                color_count[i]++;
            }
        }
        }
    }
    //int color_count[pixels_number];
    //for (int i=0; i<pixels_number; i++) {                      // Count each combination's appearance
    //    color_count[i]=0;
    //    for (int j=0; j<pixels_number; j++) {
    //        if (RGBarray[i*3] == RGBarray[j*3] && RGBarray[i*3+1] == RGBarray[j*3+1] && RGBarray[i*3+2] == RGBarray[j*3+2]) {
    //            color_count[i]++;}}}
        
    int max_appearance = 0 , most_frequent_combo = 0;
    for (int i=0; i<pixels_number; i++) {                      // Find out the most frequent combination
        if (color_count[i]>max_appearance) {
            max_appearance=color_count[i];
            most_frequent_combo=i;
        }
    }
    // cout << round(RGBarray[most_frequent_combo*3]) << "," << round(RGBarray[most_frequent_combo*3+1]) << "," << round(RGBarray[most_frequent_combo*3+2]) << "," << endl;
    return (unsigned char)RGBarray[most_frequent_combo*3+OutputChannel];
}

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int BytesPerPixel = 3, height = 275, width = 380;    // Default height/width value (barn.raw)
    int WindowSize;                                      // Default window size
    
    // argv[1] = "/Users/YJLee/Desktop/barn64.raw"          // argv[1] = "/Users/YJLee/Desktop/coliseum64.raw"
    // argv[2] = "/Users/YJLee/Desktop/barn_oil.raw"        // argv[2] = "/Users/YJLee/Desktop/coliseum_oil.raw"
    // argv[3] = N                                          // argv[3] = N
    // argv[4] = 380                                        // argv[4] = 580
    // argv[5] = 275                                        // argv[5] = 247
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name  input_image.raw  output_image64.raw  (window_size)  (input_image's_width)  (input_image's_height)" << endl;
        return 0;
    }
    // Get specific size of windows and input image
    if (argc < 4){
        WindowSize=3;
    }
    else {
        WindowSize = (int)atoi(argv[3]);
    }
    if (argc == 6){
        width= (int)atoi(argv[4]);
        height= (int)atoi(argv[5]);
    }
    printf("window size = %d x %d\n", WindowSize, WindowSize);
    printf("image width = %d , image height = %d\n", width, height);

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

    for (int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {

            int pixel_count;
            unsigned char window[pixel_count];                                          // Use 1-D array to save RGB values of all pixels inside the window
            pixel_count=0;
            for (int j= y-floor(WindowSize/2); j<=y+floor(WindowSize/2); j++) {         // Apply the window on each one single Pixel
                for (int i= x-floor(WindowSize/2); i<=x+floor(WindowSize/2); i++) {
                    
                    if (j >= 0 && j <= height-1 && i >= 0 && i <= width-1) {            // Make sure pixel not outside the image
                        for (int ch=0; ch<BytesPerPixel; ch++) {
                        window[pixel_count*BytesPerPixel+ch]= ImageQuantize[j][i][ch];  // Save all RGB combinations from input (quantized) data
                        }
                    pixel_count++;
                    //cout << round(ImageQuantize[j][i][0]) << "," << round(ImageQuantize[j][i][1]) << "," << round(ImageQuantize[j][i][2]) << endl;
                    }
                }
            }
            for (int ch=0; ch<BytesPerPixel; ch++) {
                ImageOutput[y][x][ch] = getComponentOfMostFrequentRGB(window, pixel_count, ch);     // Apply the most frequent "color" to the output pixel.
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
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n",elapsed_secs);
    return 0;
}
#endif