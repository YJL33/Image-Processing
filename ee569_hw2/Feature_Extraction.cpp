//
//  main.cpp
//  HW2_P1
//
//  Extract the features by applying 25 Law's filters on 128x128 8-bit samples.
//
//  There are three major parts, and used three corresponding functions:
//
//  a. remove DC part from image: calculate local mean and substract local mean from original array.
//  b. apply Law's filter: based on 5 kernels, derive the corresponding filter, then apply on zero-local-mean array.
//  c. compute the local energy: calculate variance in surrounding area for each pixel, then get the array average.
//
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef FEATURE_EXTRACTION
#define FEATURE_EXTRACTION

using namespace std;

// This function returns the local mean from input array and input length. (used in step 1)
double calcLocalMean(double arr[], int len)
{
    double sum=0;
    for (int i = 0; i < len; i++)
    {
        sum += arr[i];
    }
    return double(sum/len);
}

// This function applies specific Law's filter on the image (used in step 2)
double applyLawsFilter(double arr[], int len, int k)
{
    int kernels[25] =
    {
        1, 4, 6, 4, 1,
        -1, -2, 0, 2, 1,
        -1, 0, 2, 0, -1,
        -1, 2, 0, -2, 1,
        -1, -4, 6, -4, 1,
    };
    
    // Which kernel go to the tensor product
    int A[5] = {kernels[(int)(floor(k/5))*5], kernels[(int)(floor(k/5))*5+1], kernels[(int)(floor(k/5))*5+2], kernels[(int)(floor(k/5))*5+3], kernels[(int)(floor(k/5))*5+4]};
    int B[5] = {kernels[(k%5)*5], kernels[(k%5)*5+1], kernels[(k%5)*5+2], kernels[(k%5)*5+3], kernels[(k%5)*5+4]};
    //cout << A[0] << "," << A[1] << "," << A[2] << "," << A[3] << "," << A[4] << "," << endl;
    //cout << B[0] << "," << B[1] << "," << B[2] << "," << B[3] << "," << B[4] << "," << endl;
    
    // Calculate tensor product
    int LawsFilter[25];
    for (int j=0; j<5; j++)
    {
        for (int i=0; i<5; i++)
        {
            LawsFilter[j*5+i]=A[j]*B[i];
            //cout << LawsFilter[j*5+i] << endl;
        }
    }
    
    // Apply the filter
    double sum=0;
    double num=0;
    for (int i=0; i<len; i++) {
        sum += LawsFilter[i]*arr[i];
        num ++;
    }
    return double(sum/num);
}

// This function returns the variance from input array and input length. (used in step 3)
double calcLocalEnergy(double arr[], int len)
{
    double sum=0;
    for (int i = 0; i < len; i++)
    {
        sum += arr[i];
    }
    double mean = (sum/len);
    double tmp=0;
    for (int i = 0; i < len; i++) {
        tmp += (arr[i] - mean) * (arr[i] - mean);
    }
    return (tmp/len);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int Size = 128, WindowSize = 5, NumberOfFilter = 25;                     // Define the variables
    
    // argv[1] = "/Users/YJLee/Desktop/unknown_01.raw";
    // argv[2] = "/Users/YJLee/Desktop/unknown_01.txt";

    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [image's general name] [number of series of files]" << endl;
        cout << "e.g. [/Users/YJLee/Desktop/part_a/unknown_%02d.raw] [24]" << endl;
        return 0;
    }
    //printf("Size of squared window: %d, Size of image: %d x %d\n", (WindowSize*WindowSize), Size, Size);

    // Read the image contents by fread(ptr,size,count,fp)
    unsigned char Imagedata[Size][Size];
    
    int filenumber[24];
    for(int i = 0; i < 24; i++)
    {
        filenumber[i] = (i+1);
    }
    
    char filepath[42];
    
    // filepath = argv[1] + filename + str_filenumber + filesubname;
    for (int filecount = 0; filecount < atoi(argv[2]); filecount++) {
        cout << endl;
        sprintf(filepath, argv[1], (char)filenumber[filecount]);
        cout << filepath << endl;
        
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
        
        for (int y = 0; y < Size; y++)
        {
            for (int x = 0; x < Size; x++)
            {
                int pixel_count = 0;
                for (int j = y - floor(WindowSize/2); j <= y + floor(WindowSize/2); j++)
                {
                    for (int i = x - floor(WindowSize/2); i <= x + floor(WindowSize/2); i++)
                    {
                        if (j >= 0 && j < Size && i >= 0 && i < Size)
                        {
                            Window[pixel_count] = Imagedata[j][i];
                            pixel_count++;
                        }
                    }
                }
                ImageLocalMean[y][x] = Imagedata[y][x] - calcLocalMean(Window, pixel_count);
            }
        }
        // extend the image boundary (in order to enable filtering):
        double ImageAddFrame[Size+4][Size+4];
        
        for (int y=0; y<Size; y++)
        {
            for (int x=0; x<Size; x++)
            {
                ImageAddFrame[y+2][x+2] = ImageLocalMean[y][x];
            }
        }
        // copy upper and lower boundaries
        for (int x=0; x<Size; x++)
        {
            ImageAddFrame[0][x+2] =  ImageLocalMean[1][x];
            ImageAddFrame[1][x+2] =  ImageLocalMean[0][x];
            ImageAddFrame[Size+2][x+2] = ImageLocalMean[Size-1][x];
            ImageAddFrame[Size+3][x+2] = ImageLocalMean[Size-2][x];
        }
        // copy left and right boundaries from itself
        for (int y=0; y<(Size+4); y++)
        {
            ImageAddFrame[y][0] = ImageAddFrame[y][3];
            ImageAddFrame[y][1] = ImageAddFrame[y][2];
            ImageAddFrame[y][Size+2] = ImageAddFrame[y][Size+1];
            ImageAddFrame[y][Size+3] = ImageAddFrame[y][Size];
        }

        // 2. Apply 25 Law's filter
        double ImageFiltered [Size][Size][NumberOfFilter];
        for (int flt=0; flt < NumberOfFilter; flt++)
        {
            for (int y = 2; y < Size+2; y++)
            {
                for (int x = 2; x < Size+2; x++)
                {
                    int pixel_count = 0;
                    for (int j = y - floor(WindowSize/2); j <= y + floor(WindowSize/2); j++)
                    {
                        for (int i = x - floor(WindowSize/2); i <= x + floor(WindowSize/2); i++)
                        {
                            if (j >= 0 && j < Size+4 && i >= 0 && i < Size+4)
                            {
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
        for (int flt=0; flt < NumberOfFilter; flt++)
        {
            for (int y = 0; y < Size; y++)
            {
                for (int x = 0; x < Size; x++)
                {
                    int pixel_count = 0;
                    for (int j = y - floor(WindowSize/2); j <= y + floor(WindowSize/2); j++)
                    {
                        for (int i = x - floor(WindowSize/2); i <= x + floor(WindowSize/2); i++)
                        {
                            if (j >= 0 && j < Size && i >= 0 && i < Size)
                            {
                                Window[pixel_count] = ImageFiltered[j][i][flt];
                                pixel_count++;
                            }
                        }
                    }
                    ImageEnergy[y][x][flt] = calcLocalEnergy(Window, pixel_count);
                }
            }
        }
        
        double FeatureVector[NumberOfFilter][filecount];
        for (int flt =0; flt < NumberOfFilter; flt++)
        {
            int pixel_count = 0;
            double sum = 0;
            for (int y =0; y < Size; y++)
            {
                for (int x =0; x < Size; x++)
                {
                    sum += ImageEnergy[y][x][flt];
                    pixel_count++;
                }
            }
            FeatureVector[flt][filecount] = (sum/pixel_count);
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //if (!(file = fopen(argv[2], "w"))) {
        //    cout << "Error: unable to save txt file" << endl;
        //}
        //fprintf(file, "Feature Vector\n");
        for (int flt = 0; flt < NumberOfFilter; flt++)
        {
            printf("%f,", FeatureVector[flt][filecount]);
        }
        fclose(file);
    }
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\nelapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif