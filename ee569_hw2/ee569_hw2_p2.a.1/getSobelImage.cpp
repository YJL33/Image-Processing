//
//  getGradientImage.cpp
//  HW2_P2.b.1
//
//  Get Sobel gradient image from grey image.
//
//  Input: [grey image] [output gradient image] [output Sobel image]
//
//  1. extend the boundary
//  2. apply the sobel detector filter (3x3) to get gradient image
//  3. output the gradient image and Sobel image
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "getGradient.h"

#ifndef GRADIENTIMG
#define GRADIENTIMG

using namespace std;

double getGradient(double arr[], int len, bool k);

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE * file;
    int Height = 321, Width = 481;        // Define the variables
    const int SobelWindowSize = 1;        // 1*2+1 = 3
    
    // argv[1] = "/Users/YJLee/Desktop/Farm_grey.raw";
    // argv[2] = "/Users/YJLee/Desktop/Farm_sobel.array";
    
    // Check for proper syntax
    if (argc < 4)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [grey input image] [output gradient image] [output Sobel image]" << endl;
        return 0;
    }
    
    // Read all the features by fread(ptr,size,count,fp)
    unsigned char Imagedata[Height][Width];
    if (!(file=fopen(argv[1],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Height*Width, file);
    fclose(file);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // 1. extend the image boundary (in order to enable filtering):
    double ImageAddFrame[Height+SobelWindowSize*2][Width+SobelWindowSize*2];
    
    for (int y=0; y<Height; y++){
        for (int x=0; x<Width; x++){
            ImageAddFrame[y+SobelWindowSize][x+SobelWindowSize] = Imagedata[y][x];
        }
    }
    // copy upper and lower boundaries from itself
    for (int x=0; x<Width; x++){
        for (int layer = 0; layer < SobelWindowSize; ++layer)
        {
            ImageAddFrame[SobelWindowSize-1-layer][x+SobelWindowSize] =  ImageAddFrame[SobelWindowSize+layer][x+SobelWindowSize];
            ImageAddFrame[Height+SobelWindowSize+layer][x+SobelWindowSize] = ImageAddFrame[Height+SobelWindowSize-1-layer][x+SobelWindowSize];
        }
        
    }
    // copy left and right boundaries from itself
    for (int y=0; y<(Height+SobelWindowSize*2); ++y){
        for (int layer = 0; layer < SobelWindowSize; ++layer)
        {
            ImageAddFrame[y][SobelWindowSize-1-layer] = ImageAddFrame[y][SobelWindowSize+layer];
            ImageAddFrame[y][Width+SobelWindowSize+layer] = ImageAddFrame[y][Width+SobelWindowSize-1-layer];
        }
    }
    
    // 2. Apply Sobel detector, to get gradient image
    double SobelImageX[Height][Width], SobelImageY[Height][Width];
    double Window[(2*SobelWindowSize+1)*(2*SobelWindowSize+1)];
    for (int y = SobelWindowSize; y < Height+SobelWindowSize; y++){
        for (int x = SobelWindowSize; x < Width+SobelWindowSize; x++){
            int pixel_count = 0;
            for (int j = y - SobelWindowSize; j <= y + SobelWindowSize; ++j){
                for (int i = x - SobelWindowSize; i <= x + SobelWindowSize; ++i){
                    Window[pixel_count] = ImageAddFrame[j][i];
                    pixel_count++;
                    //cout << i << ","<< j << "," << round(ImageAddFrame[j][i]) << "," << round(Window[pixel_count-1]) << "," << pixel_count << endl;
                }
            }
            SobelImageX[y-SobelWindowSize][x-SobelWindowSize] = getGradient(Window, pixel_count, true);    // Assign the Sobel Detector
            SobelImageY[y-SobelWindowSize][x-SobelWindowSize] = getGradient(Window, pixel_count, false);   // Assign the Sobel Detector
//            if (SobelImageX[y-SobelWindowSize][x-SobelWindowSize] > 1000 || SobelImageX[y-SobelWindowSize][x-SobelWindowSize] < -1000) {
//                cout << (x-SobelWindowSize) << ","<< (y-SobelWindowSize)<< "," << round(SobelImageX[y-SobelWindowSize][x-SobelWindowSize]) << endl;
//           }
        }
    }
    
    // Normalization the gradient image
    double max_y =0, max_x =0;
    double min_y =10000, min_x=10000;
    for (int y = 0; y<Height; ++y) {
        for (int x = 0; x<Width; ++x) {
            if (min_y > SobelImageY[y][x]) {
                min_y = SobelImageY[y][x];
            }
            if (min_x > SobelImageX[y][x]) {
                min_x = SobelImageX[y][x];
            }
            if (max_y < SobelImageY[y][x]) {
                max_y = SobelImageY[y][x];
            }
            if (max_x < SobelImageX[y][x]) {
                max_x = SobelImageX[y][x];
            }
//            cout << x << "," << y << "," << min_x << ","<< min_y << "," << max_x << ","<< max_y << endl;
        }
    }
    
    // Step 3. Output the gradient image and Sobel image
    
    // Preparation: Combine Gradient_x and Gradient_y
    double PreSobelImage[Height][Width];
    double PreGradImage[Height][Width];
    unsigned char SobelImage[Height][Width];
    unsigned char GradImage[Height][Width];
    double max_sb=0, min_sb=10000;
    double max_gd=0, min_gd=10000;
    for (int y=0; y<Height; y++) {
        for (int x=0; x<Width; x++) {
            double normalize_x, normalize_y;
            normalize_x = 255*((SobelImageX[y][x]-min_x)/max_x);
            normalize_y = 255*((SobelImageY[y][x]-min_y)/max_y);
            PreGradImage[y][x] = sqrt(pow(normalize_x, 2) + pow(normalize_y, 2));
            PreSobelImage[y][x] = sqrt(pow(SobelImageX[y][x], 2) + pow(SobelImageY[y][x], 2));

            if (min_sb > PreSobelImage[y][x]) {
                min_sb = PreSobelImage[y][x];
            }
            if (min_gd > PreGradImage[y][x]) {
                min_gd = PreGradImage[y][x];
            }
            if (max_sb < PreSobelImage[y][x]) {
                max_sb = PreSobelImage[y][x];
            }
            if (max_gd < PreGradImage[y][x]) {
                max_gd = PreGradImage[y][x];
            }
//            cout << x << "," << y << "," << min_sb << ","<< max_sb << endl;
        }
    }
    
    // Normalize Sobel gradient image
    for (int y=0; y<Height; y++) {
        for (int x=0; x<Width; x++) {
            GradImage[y][x] = (unsigned char)(255*((PreGradImage[y][x]-min_sb)/max_sb));
            SobelImage[y][x] = (unsigned char)(255*((PreSobelImage[y][x]-min_sb)/max_sb));
 //       cout << x << "," << y << "," << round(SobelImage[y][x]) << endl;
        }
    }

    int count =0, colorvalue =0;
    
    for (int cv=255; cv>0; cv--) {             // Count the number of high-intensity pixels
        for (int y=0; y<Height; y++) {
            for  (int x=0; x<Width; x++){
                if (SobelImage[y][x] == cv) {
                    count++;
                }
            }
        }
        if (count > (int)Height*Width*0.1) {
            colorvalue = cv;
            printf("\n10%% pixels have intensity more than %d.\n", cv);
            break;
        }
    }
    for (int cv=colorvalue-1; cv>0; cv--) {    // Count the number of high-intensity pixels
        for (int y=0; y<Height; y++) {
            for  (int x=0; x<Width; x++){
                if (SobelImage[y][x] == cv) {
                    count++;
                }
            }
        }
        if (count > (int)Height*Width*0.15) {
            printf("\n15%% pixels have intensity more than %d.\n", cv);
            break;
        }
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save gradient image" << endl;
        exit(1);
    }
    fwrite(GradImage, sizeof(unsigned char), Height*Width, file);
    fclose(file);
    
    if (!(file=fopen(argv[3],"wb"))) {
        cout << "Error: unable to save Sobel image" << endl;
        exit(1);
    }
    fwrite(SobelImage, sizeof(unsigned char), Height*Width, file);
    fclose(file);

    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif