//
//  applyMS.cpp
//  HW2_P2.b.1
//
//  Apply Mean Shift detector on gradient image file, which is converted from assignment (321x481 RGB image).
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

#ifndef APPLY_MS
#define APPLY_MS

using namespace cv;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE * file;
    int Height = 321, Width = 481, Color = 3;;        // Define the variables
    unsigned char Imagedata[Height][Width][Color];
    unsigned char MSImage[Height][Width][Color];
    int sp, sr, minsize;
    
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
    
    // Read image
    if (!(file=fopen(argv[1],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Height*Width*Color, file);
    fclose(file);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Make the array into Matrices
    Mat ImagedataMat = Mat(Height, Width, CV_8UC3, Imagedata);   // columns -> height, rows -> width
    Mat ImagedataMatTranspose, ImageMSMatTranspose;
    transpose(ImagedataMat, ImagedataMatTranspose);
    //cout << ImageEdgeMat << endl;
    
    // Apply MS detector
    gpu::meanShiftSegmentation( ImagedataMatTranspose, ImageMSMatTranspose, sp, sr, minsize);
    
    // Convert the matrix into array
    for (int x=0; x<Width; ++x) {
        for (int y=0; y<Height; ++y) {
            for (int ch =0; ch<Color; ++ch) {
                MSImage[y][x][ch] = (unsigned char)(ImageMSMatTranspose.at<unsigned char>(x, y));
                //cout << x << "," << y << "," << round(MSImage[y][x]) << endl;
            }
        }
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // output the array
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save NMS image" << endl;
        exit(1);
    }
    fwrite(MSImage, sizeof(unsigned char), Height*Width, file);
    fclose(file);

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif