//
//  SobelThreshold.cpp
//  HW2_P2.b.2
//
//  Set Threshold on gradient image.
//
//  Input: [gradient image] [threshold] [output threshold image]
//
//  Load the image, assign the new value according to the threshold, and output the image.
//
//
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
//#include <opencv2/core/core.hpp>
//#include <opencv2/opencv.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/highgui/highgui.hpp>

#ifndef SOBELTHRESHOLD
#define SOBELTHRESHOLD

//using namespace cv;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE * file;
    int Height = 321, Width = 481;        // Define the variables
    int Threshold = 56;
    
    // argv[1] = "/Users/YJLee/Desktop/Farm_grey.raw";
    // argv[2] = 56;
    // argv[3] = "/Users/YJLee/Desktop/Farm_sobel.array";
    
    // Check for proper syntax
    if (argc < 4)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [gradient image] [threshold (0~255)] [threshold-ed image]" << endl;
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
    
    //Get threshold
    Threshold = atoi(argv[2]);
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Make the array into Matrices
//    Mat InputMat = Mat(Height, Width, CV_8UC1, Imagedata);   // columns -> height, rows -> width
//    Mat InputMatTranspose;
//    transpose(InputMat, InputMatTranspose);
    
    // Debug:
    // cout << absGradxMat << endl;
    
    printf("From gradient image histogram, we know that:\nFor Cougar.raw, the threshold of 10%% and 15%% are 56 and 46.\nFor Farm.raw, the threshold of 10%% and 15%% are 81 and 65.\n(May varied depending on Sobel detector setting)\n");
    printf("\nCurrent threshold: %d", Threshold);
 
    unsigned char SobelThreshold[Height][Width];
    
    // Thresholding & inversing black <-> white
    for (int y=0; y<Height; y ++) {
        for (int x=0; x<Width; x ++) {
//            if ((unsigned char)(InputMatTranspose.at<unsigned char>(x, y)) < Threshold) {
            if (Imagedata[y][x] < Threshold) {

                SobelThreshold[y][x] = 255;
            }
            else{
                SobelThreshold[y][x] = (unsigned char)(255-(Imagedata[y][x]));
            }
        }
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!(file=fopen(argv[3],"wb"))) {
        cout << "Error: unable to save threshold image" << endl;
        exit(1);
    }
    fwrite(SobelThreshold, sizeof(unsigned char), Height*Width, file);
    fclose(file);
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif