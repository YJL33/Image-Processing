//
//  Shrink.cpp
//  HW3_P3.a.1
//
//  Apply shrinking filter on a raw square image (91x108 is defaulted according to the homework assignment)
//
//  Steps:
//  0. Normalization (White=255, Black=0 => White=1, Black=0; assume that white is default object)
//  1. Calculate rank for each pixel
//  2. For rank >0, Apply 1st h/m filter and check whether hit or miss
//  3. Pick up canditates, and apply 2nd h/m filter.
//  4. If 2nd h/m filter hit, kill the pixel. If not, leave it.
//  5. Iterate the above process until kill=0.
//
//  Input: Raw image, desired output file path
//  Output: Shrinked image
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "hitormiss_1.h"
#include "hitormiss_2.h"

#ifndef SHRINKING
#define SHRINKING

using namespace std;

unsigned char HitOrMiss1(unsigned char arr[], int len);       // Pick and mark the candidate from input data
unsigned char HitOrMiss2(unsigned char arr[], int len);       // Pick and mark the desired pixels from marks

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int BytesPerPixel=1, Height = 108, Width = 91;                      // Define the variables
    bool inverseImage=false;
    
    //argv[1] = "/Users/YJLee/Desktop/Horseshoe.raw";
    //argv[2] = "/Users/YJLee/Desktop/Horseshoe_shrink.raw";
    //argv[3] = 0
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [1 if interested in black object]" << endl;
        return 0;
    }
    if ((argc = 4) && argv[3] !=0) {
        inverseImage=true;
        cout << "<<<< Black pixels as object in horseshoe.raw >>>>" << endl;
    }
    else {
        cout << "<<<< White pixels as object in horseshoe.raw >>>>" << endl;
    }
    
    // Read the image contents by fread(ptr,size,count,fp)
    unsigned char Imagedata[Height][Width][BytesPerPixel];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Height*Width*BytesPerPixel, file);
    fclose(file);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////
    
    unsigned char ImageOutput[Height][Width][BytesPerPixel];
    // unsigned char ImageMark[Height][Width][BytesPerPixel];         // Debug: Mark (candidate) image
    
    // Step 0. Make it into 0 or 1
    if (inverseImage == false) {
        for (int y=0; y<Height; ++y) {
            for (int x=0; x<Width; ++x) {
                ImageOutput[y][x][0] = (Imagedata[y][x][0])/255;
            }
        }
    }
    else
    {
        for (int y=0; y<Height; ++y) {
            for (int x=0; x<Width; ++x) {
                ImageOutput[y][x][0] = (255-Imagedata[y][x][0])/255;
            }
        }
    }
    
    // Step 1~5. All included inside the while loop
    int kill_count=1;
    int loop_count=0;
    
    while ((kill_count != 0))
    //while (loop_count != 2)   // Debug
    {
        loop_count++;
        //printf(" ************************* Loop: %d *************************\n", loop_count);     // Debug
        
        // Step 1 & 2
        unsigned char Metadata[Height][Width][BytesPerPixel];
        unsigned char Window[3*3];
    
        for (int y=0; y<Height; ++y) {
            for (int x=0; x<Width; ++x) {
                if (ImageOutput[y][x][0] == 1)
                {
                    int pixel_count = 0;
                    for (int j = y - 1; j <= y + 1; j++){
                        for (int i = x - 1; i <= x + 1; i++){
                            if (j >= 0 && j < Height && i >= 0 && i < Width){
                                Window[pixel_count] = ImageOutput[j][i][0];
                                pixel_count++;
                            }
                        }
                    }
                    Metadata[y][x][0] = HitOrMiss1(Window, pixel_count);
                }
                else
                {
                    Metadata[y][x][0] = 0;
                }
            }
        }
        /*
        for (int y=0; y<Height; ++y) {
            for (int x=0; x<Width; ++x) {
                ImageMark[y][x][0] = Metadata[y][x][0];
            }
        }  // Debug: Mark (candidate) image
        */
        
        // Step 3
        unsigned char Metadata2[Height][Width][BytesPerPixel];
        
        for (int y=0; y<Height; ++y) {
            for (int x=0; x<Width; ++x) {
                if (Metadata[y][x][0] == 1)
                {
                    int pixel_count = 0;
                    for (int j = y - 1; j <= y + 1; j++){
                        for (int i = x - 1; i <= x + 1; i++){
                            if (j >= 0 && j < Height && i >= 0 && i < Width){
                                Window[pixel_count] = Metadata[j][i][0];
                                pixel_count++;
                            }
                        }
                    }
                    //printf("candidate: (%d, %d)\n", x, y);
                    //printf("3x3 window: %d, %d, %d, %d, (%d), %d, %d, %d, %d \n", Window[0], Window[1], Window[2], Window[3], Window[4], Window[5], Window[6], Window[7], Window[8]);
                    
                    Metadata2[y][x][0] = HitOrMiss2(Window, pixel_count);
                    /*
                    if (Metadata2[y][x][0] != 0) {
                        printf("candidate to be killed: (%d, %d)\n", x, y);
                    }
                    if (Metadata2[y][x][0] != 1) {
                        printf("candidate survived: (%d, %d)\n", x, y);
                    }      // (Debug)
                    */
                }
                else
                {
                    Metadata2[y][x][0] = 0;
                }
            }
        }
        // Step 4 & 5
        kill_count = 0;
        for (int y=0; y<Height; ++y) {
            for (int x=0; x<Width; ++x) {
                if (Metadata2[y][x][0] != 0) {
                    ImageOutput[y][x][0] = 0;
                    // printf("kill candidate: (%d, %d)\n", x, y);
                    kill_count++;
                }
            }
        }
    }
    /*
    // Find Spot
    for (int y=0; y<Height; ++y) {
        for (int x=0; x<Width; ++x) {
            if (ImageOutput[y][x][0] == 1) {
                int pixel_count = 0;
                unsigned char Window[3*3];
                for (int j = y - 1; j <= y + 1; j++){
                    for (int i = x - 1; i <= x + 1; i++){
                        if (j >= 0 && j < Height && i >= 0 && i < Width){
                            Window[pixel_count] = ImageOutput[j][i][0];
                            pixel_count++;
                        }
                    }
                }
                if (pixel_count == 9 && Window[0] == 0 && Window[1] == 0 && Window[2] == 0 && Window[3] == 0 && Window[5] == 0 && Window[6] == 0 && Window[7] == 0 && Window[8] == 0) {
                    if (inverseImage == false) {
                        printf("White Spot: (%d, %d)\n", x, y);
                    }
                    else {
                        printf("Black Spot: (%d, %d)\n", x, y);
                    }
                    
                }
            }
        }
    }
     */
    
    // Convert it back to 0 to 255
    if (inverseImage == false)
    {
        for (int y=0; y<Height; ++y) {
            for (int x=0; x<Width; ++x) {
                ImageOutput[y][x][0] = ImageOutput[y][x][0]*255;
                // ImageMark[y][x][0] = ImageMark[y][x][0]*255;
            }
        }
    }
    else
    {
        for (int y=0; y<Height; ++y) {
            for (int x=0; x<Width; ++x) {
                ImageOutput[y][x][0] = (1-ImageOutput[y][x][0])*255;
                // ImageMark[y][x][0] = (1-ImageMark[y][x][0])*255;
            }
        }
    }
    printf("Total loops: %d\n", loop_count);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////

    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(ImageOutput, sizeof(unsigned char), Height*Width*BytesPerPixel, file);
    fclose(file);
    cout << "Scaled image is successfully saved" <<endl;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif