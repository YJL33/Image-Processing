//
//  applyCanny.cpp
//  HW2_P2.b.1
//
//  Apply Canny detector on gradient image file, which is converted from assignment (321x481 RGB image).
//
//  The goal is to find out edge and contour.
//
//
//  Input: [input image]......................................... single-channel 8-bit input image
//         [output image]........................................ output edge map; it has the same size and type as input
//         [low threshold].......................................
//         [high threshold]......................................
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

#ifndef APPLY_CANNY
#define APPLY_CANNY

using namespace cv;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE * file;
    int Height = 321, Width = 481;        // Define the variables
    unsigned char ImageGrad[Height][Width];
    unsigned char CannyImage[Height][Width];
    double threshold1;
    double threshold2;
    
    // argv[1] = "/Users/YJLee/Desktop/gray_img.raw";
    // argv[2] = "/Users/YJLee/Desktop/contour_img.raw";
    // argv[3] = "0.3";
    // argv[4] = "0.6";
    
    // Check for proper syntax
    if (argc < 5)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [input image] [output image] [low threshold] [high threshold]" << endl;
        return 0;
    }
    
    // Get threshold
    threshold1 = atof(argv[3]);
    threshold2 = atof(argv[4]);
    
    // Read image
    if (!(file=fopen(argv[1],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(ImageGrad, sizeof(unsigned char), Height*Width, file);
    fclose(file);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Assignments
    printf("Generate five edge maps using different low (A) and high (B) thresholds in the hysteresis thresholding step.\n");
    printf("1) A: 0.3; B: 0.6 (Default values)\n");
    printf("2) A: 0.2; B: 0.7 (Default A-∆, Default B+∆, with ∆=1)\n");
    printf("3) A: 0.2; B: 0.5 (Default A-∆, Default B-∆)\n");
    printf("4) A: 0.4; B: 0.7 (Default A+∆, Default B+∆)\n");
    printf("5) A: 0.4; B: 0.5 (Default A+∆, Default B-∆)\n");
    
    printf("\nCurrent threshold: %.01f, %.01f\n", threshold1, threshold2);
    
    // Make the array into Matrices
    Mat ImageGradMat = Mat(Height, Width, CV_8UC1, ImageGrad);   // columns -> height, rows -> width
    Mat ImageGradMatTranspose, ImageEdgeMatTranspose;
    transpose(ImageGradMat, ImageGradMatTranspose);
    //cout << ImageEdgeMat << endl;
    
    // Apply Canny detector
    Canny( ImageGradMatTranspose, ImageEdgeMatTranspose, threshold2*255, threshold1*255, 3, true );
    
    // Convert the matrix into array
    for (int x=0; x<Width; ++x) {
        for (int y=0; y<Height; ++y) {
            CannyImage[y][x] = (unsigned char)(ImageEdgeMatTranspose.at<unsigned char>(x, y));
            //cout << x << "," << y << "," << round(CannyImage[y][x]) << endl;
        }
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // output the array
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save NMS image" << endl;
        exit(1);
    }
    fwrite(CannyImage, sizeof(unsigned char), Height*Width, file);
    fclose(file);

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif