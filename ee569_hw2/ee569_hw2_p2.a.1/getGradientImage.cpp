//
//  getGradientImage.cpp
//  HW2_P2.b.1
//
//  Get gradient image from grey image.
//
//  Input: [grey image] [output gradient image]
//
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef GRADIENTIMG
#define GRADIENTIMG

using namespace cv;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE * file;
    int Height = 321, Width = 481;        // Define the variables
    
    // argv[1] = "/Users/YJLee/Desktop/Farm_grey.raw";
    // argv[2] = "/Users/YJLee/Desktop/Farm_sobel.array";
    
    // Check for proper syntax
    if (argc < 3)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [grey input image] [output gradient image]" << endl;
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
    
    // Make the array into Matrices
    Mat InputMat = Mat(Height, Width, CV_8UC1, Imagedata);   // columns -> height, rows -> width
    Mat InputMatTranspose;
    transpose(InputMat, InputMatTranspose);
    
    // Get Gradients on x and y direction
    Mat GradxMat, GradyMat, absGradxMat, absGradyMat;
    Sobel( InputMatTranspose, GradxMat, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );
    Sobel( InputMatTranspose, GradyMat, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );
    
    // Normalize to 0-255
    convertScaleAbs( GradxMat, absGradxMat );
    convertScaleAbs( GradyMat, absGradyMat );
    
    // square the gradient, add them together, and get square root
    Mat absGradxSqMat, absGradySqMat, absGradySumSqMat, imgtemp, imgtemp2, imgtemp3, absGradSumSqMat, absGradMat;
    absGradxMat.convertTo(imgtemp, CV_32FC1);
    absGradyMat.convertTo(imgtemp2, CV_32FC1);
    pow(imgtemp, 2, absGradxSqMat);
    pow(imgtemp2, 2, absGradySqMat);
    absGradSumSqMat = absGradxSqMat + absGradySqMat;
    sqrt(absGradSumSqMat, imgtemp3);
    imgtemp3.convertTo(absGradMat, CV_8UC1);
    
    
    // Combine x and y part
    // addWeighted( absGradxMat, 0.5, absGradyMat, 0.5, 0, absGradMat );

    // Debug:
    //cout << absGradMat << endl;
    
    unsigned char Grad[Height][Width];
    int count =0, colorvalue =0;
    
    for (int y=0; y<Height; y ++) {
        for (int x=0; x<Width; x ++) {
            Grad[y][x] = absGradMat.at<unsigned char>(x, y);
            //printf("%d, ", Grad[y][x]);
        }
    }
    
    for (int cv=255; cv>0; cv--) {    // Count the number of high-intensity pixels
        for (int y=0; y<Height; y++) {
            for  (int x=0; x<Width; x++){
                if (Imagedata[y][x] == cv) {
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
                if (Imagedata[y][x] == cv) {
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
        cout << "Error: unable to save x-gradient image" << endl;
        exit(1);
    }
    fwrite(Grad, sizeof(unsigned char), Height*Width, file);
    fclose(file);
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif