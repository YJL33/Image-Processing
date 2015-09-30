//
//  FeatureExtraction.cpp
//  HW2_P1.1
//
//  Extract the features from sets of files: Apply Law's filters on 128x128 8-bit samples, and output the feature array.
//  The output will be 2-D array: [25 Law's features] * [number of samples (24 or 36)]
//
//  **** Need to execute 3 times to get: "grass feature array", "straw feature array", and "unknown feature array". *****
//
//  Input: [the full description of sets of files]...................... e.g. "../part_a/unknown_%02d.raw"
//         [number of samples].......................................... 36 labeled samples or 24 unlabeled samples
//         [output "feature array"]..................................... 2-D array: [25 Law's features] * [number of samples (24 or 36)]
//
//  There are three major steps, and used three corresponding functions:
//  1. Remove DC part from image: calculate local mean and substract local mean from original array.
//  2. Apply Law's filter: based on 5 kernels, derive the corresponding filter, then apply on zero-local-mean array.
//  3. Compute the local energy: calculate variance in surrounding area for each pixel, then get the array average.
//
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "get_localmean.h"
#include "apply_Lawsfilter.h"
#include "calc_localenergy.h"

#ifndef FEATURE_EXTRACTION
#define FEATURE_EXTRACTION

using namespace std;

double getLocalMean(double arr[], int len);
double applyLawsFilter(double arr[], int len, int k);
double calcLocalEnergy(double arr[], int len);

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int Size = 128, WindowSize = 5, NumberOfFilter = 25;                     // Define the variables
    int FileCount = atoi(argv[2]);
    double FeatureVector[NumberOfFilter][FileCount];
    
    // argv[1] = "/Users/YJLee/Desktop/part_a/unknown_%02d.raw";
    // argv[2] = 24;
    // argv[3] = "/Users/YJLee/Desktop/part_a/unknown_mean.txt"

    // Check for proper syntax
    if (argc < 4){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [image's general name] [number of series of files]" << endl;
        cout << "e.g. [/Users/YJLee/Desktop/part_a/unknown_%02d.raw] [24]" << endl;
        return 0;
    }
    printf("Size of Law's filter: %d, Size of image: %d x %d", (WindowSize*WindowSize), Size, Size);

    // Read the image contents by fread(ptr,size,count,fp)
    // use a for loop to handle all image repeatedly
    
    unsigned char Imagedata[Size][Size];
    char filepath[42];
    
    for (int filecount = 0; filecount < FileCount; filecount++) {
        sprintf(filepath, argv[1], (char)(filecount+1));
        cout << "\n\nExtracting image feature from:  \n" << filepath << " \n" << endl;
        
        if (!(file=fopen(filepath,"rb")))
        {
            cout << "Error: unable to open file" <<endl;
            exit(1);
        }
        fread(Imagedata, sizeof(unsigned char), Size*Size, file);
        fclose(file);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // 1. Remove the DC part
        double ImageLocalMean[Size][Size];
        double Window[WindowSize*WindowSize];                            // Apply a square window on each pixel.
        
        for (int filecount = 0; filecount < FileCount; filecount++) {
            for (int y = 0; y < Size; y++){
                for (int x = 0; x < Size; x++){
                    int pixel_count = 0;
                    for (int j = y - floor(WindowSize/2); j <= y + floor(WindowSize/2); j++){
                        for (int i = x - floor(WindowSize/2); i <= x + floor(WindowSize/2); i++){
                            if (j >= 0 && j < Size && i >= 0 && i < Size){
                                Window[pixel_count] = Imagedata[j][i];
                                pixel_count++;
                            }
                        }
                    }
                    ImageLocalMean[y][x] = Imagedata[y][x] - getLocalMean(Window, pixel_count);
                }
            }
        }
            
        // extend the image boundary (in order to enable filtering):
        double ImageAddFrame[Size+4][Size+4];
        
        for (int y=0; y<Size; y++){
            for (int x=0; x<Size; x++){
                ImageAddFrame[y+2][x+2] = ImageLocalMean[y][x];
            }
        }
        // copy upper and lower boundaries
        for (int x=0; x<Size; x++){
            ImageAddFrame[0][x+2] =  ImageLocalMean[1][x];
            ImageAddFrame[1][x+2] =  ImageLocalMean[0][x];
            ImageAddFrame[Size+2][x+2] = ImageLocalMean[Size-1][x];
            ImageAddFrame[Size+3][x+2] = ImageLocalMean[Size-2][x];
        }
        // copy left and right boundaries from itself
        for (int y=0; y<(Size+4); y++){
            ImageAddFrame[y][0] = ImageAddFrame[y][3];
            ImageAddFrame[y][1] = ImageAddFrame[y][2];
            ImageAddFrame[y][Size+2] = ImageAddFrame[y][Size+1];
            ImageAddFrame[y][Size+3] = ImageAddFrame[y][Size];
        }

        // 2. Apply 25 Law's filter
        double ImageFiltered [Size][Size][NumberOfFilter];
        for (int flt=0; flt < NumberOfFilter; flt++){
            for (int y = 2; y < Size+2; y++){
                for (int x = 2; x < Size+2; x++){
                    int pixel_count = 0;
                    for (int j = y - floor(WindowSize/2); j <= y + floor(WindowSize/2); j++){
                        for (int i = x - floor(WindowSize/2); i <= x + floor(WindowSize/2); i++){
                            if (j >= 0 && j < Size+4 && i >= 0 && i < Size+4){
                                Window[pixel_count] = ImageAddFrame[j][i];
                                pixel_count++;
                            }
                        }
                    }
                    ImageFiltered[y-2][x-2][flt] = applyLawsFilter(Window, pixel_count, flt);    // Assign the Law's Filter
                    // cout << (x-2) << ","<< (y-2)<< "," << round(ImageFiltered[y-2][x-2][flt]) << endl;
                }
            }
        }
        
        // 3. Calculate the energy
        double ImageEnergy[Size][Size][NumberOfFilter];
        for (int flt=0; flt < NumberOfFilter; flt++){
            for (int y = 0; y < Size; y++){
                for (int x = 0; x < Size; x++){
                    int pixel_count = 0;
                    for (int j = y - floor(WindowSize/2); j <= y + floor(WindowSize/2); j++){
                        for (int i = x - floor(WindowSize/2); i <= x + floor(WindowSize/2); i++){
                            if (j >= 0 && j < Size && i >= 0 && i < Size){
                                Window[pixel_count] = ImageFiltered[j][i][flt];
                                pixel_count++;
                            }
                        }
                    }
                    ImageEnergy[y][x][flt] = calcLocalEnergy(Window, pixel_count);
                }
            }
        }
        for (int flt =0; flt < NumberOfFilter; flt++){
            int pixel_count = 0;
            double sum = 0;
            for (int y =0; y < Size; y++){
                for (int x =0; x < Size; x++){
                    sum += ImageEnergy[y][x][flt];
                    pixel_count++;
                }
            }
            FeatureVector[flt][filecount] = (sum/pixel_count);
            printf("%f, ", FeatureVector[flt][filecount]);
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[3],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(FeatureVector, sizeof(double), NumberOfFilter*FileCount, file);
    fclose(file);
    cout << "\n\nAll Feature vector arrays successfully saved" <<endl;
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif