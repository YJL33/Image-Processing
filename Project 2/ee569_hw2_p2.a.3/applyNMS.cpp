//
//  applyNMS.cpp
//  HW2_P2.b.3
//
//  Apply NMS on Sobel thresholded image.
//
//  Input: [Thresholded Sobel image] [output NMS image] ([NMS window size])
//
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "nms.h"

#ifndef SOBELTHRESHOLD
#define SOBELTHRESHOLD

using namespace std;

unsigned char NMS(unsigned char arr_in[], int len);
/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE * file;
    int Height = 321, Width = 481;        // Define the variables
    int NMSWindowSize = 1;
    
    // argv[1] = "/Users/YJLee/Desktop/Farm_Sobel_th10.raw";
    // argv[2] = "/Users/YJLee/Desktop/Farm_Sobel_th10_nms.raw";
    
    // Check for proper syntax
    if (argc < 3)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [Thresholded Sobel image] [output NMS image] (NMS window size)" << endl;
        return 0;
    }
    
    if (argc == 4)
    {
        NMSWindowSize = atoi(argv[3]);
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
    
    unsigned char ImagedataInv[Height][Width];
    // inversing black <-> white
    // After inversion: 0 = black, 255 = white
    for (int y=0; y<Height; y ++) {
        for (int x=0; x<Width; x ++) {
            ImagedataInv[y][x] = (unsigned char)(255-Imagedata[y][x]);
        }
    }
    
    // Debug:
    // cout << absGradxMat << endl;
    
    // extend the image boundary (in order to enable filtering):
    double ImageAddFrame[Height+NMSWindowSize*2][Width+NMSWindowSize*2];
    
    for (int y=0; y<Height; y++){
        for (int x=0; x<Width; x++){
            ImageAddFrame[y+NMSWindowSize][x+NMSWindowSize] = ImagedataInv[y][x];
        }
    } 
    // copy upper and lower boundaries from itself
    for (int x=0; x<Width; x++){
        for (int layer = 0; layer < NMSWindowSize; ++layer)
        {
            ImageAddFrame[NMSWindowSize-1-layer][x+NMSWindowSize] =  ImageAddFrame[NMSWindowSize+layer][x+NMSWindowSize];
            ImageAddFrame[Width+NMSWindowSize+layer][x+NMSWindowSize] = ImageAddFrame[Width+NMSWindowSize-1-layer][x+NMSWindowSize];
        }
        
    }
    // copy left and right boundaries from itself
    for (int y=0; y<(Height+NMSWindowSize*2); ++y){
        for (int layer = 0; layer < NMSWindowSize; ++layer)
        {
            ImageAddFrame[y][NMSWindowSize-1-layer] = ImageAddFrame[y][NMSWindowSize+layer];
            ImageAddFrame[y][Width+NMSWindowSize+layer] = ImageAddFrame[y][Width+NMSWindowSize-1-layer];
        }
    }


    
    // Apply NMS
    unsigned char NMSImage[Height][Width];
    unsigned char Window[(2*NMSWindowSize+1)*(2*NMSWindowSize+1)];
    for (int y = NMSWindowSize; y < Height+NMSWindowSize; y++){
        for (int x = NMSWindowSize; x < Width+NMSWindowSize; x++){
            int pixel_count = 0;
            for (int j = y - NMSWindowSize; j <= y + NMSWindowSize; j++){
                for (int i = x - NMSWindowSize; i <= x + NMSWindowSize; i++){
                    if (j >= 0 && j < Height+4 && i >= 0 && i < Width+4){
                        Window[pixel_count] = ImageAddFrame[j][i];
                        pixel_count++;
                    }
                }
            }
            unsigned char local_max = NMS(Window, pixel_count);
            if (ImageAddFrame[y+NMSWindowSize][x+NMSWindowSize] < local_max)
            {
                NMSImage[y-NMSWindowSize][x-NMSWindowSize] = 0;
            }
            else
            {
                NMSImage[y-NMSWindowSize][x-NMSWindowSize] = local_max;
//                cout << (x-2) << ","<< (y-2)<< "," << round(NMSImage[y-2][x-2]) << endl;
            }
        }
    }
        
    // inversing black <-> white & drawing contour
    unsigned char NMSImageInv[Height][Width];
    
    for (int y =0; y<Height; ++y) {
        for (int x=0; x<Width; ++x) {
            if (NMSImage[y][x] == 0) {
                NMSImageInv[y][x] = 255;
            }
            else
                NMSImageInv[y][x] = 0;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save NMS image" << endl;
        exit(1);
    }
    fwrite(NMSImageInv, sizeof(unsigned char), Height*Width, file);
    fclose(file);
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif