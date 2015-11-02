//
//  applyVectorHalftoning.cpp
//  HW3_P2.d.1
//
//  Apply Floyd-Steinberg error diffusion method on a raw square color image (512x512x3 = default homework assignment)
//  Using 3x3 filter
//  Need to convert the color space from RGBW -> CMYK
//
//  Input: Raw square color image, desired output file path
//  Output: Vector Halftoning square image
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef VECTOR_HALFTON
#define VECTOR_HALFTON

using namespace std;

// This function returns the address of closest vertex to the target pixel in WRGB-CMYK space.
// dist = sqrt( (x-a)^2 + (y-b)^2 + (z-c)^2  )
int* MinDistFinder(double target[3], int vertex1[3], int vertex2[3], int vertex3[3], int vertex4[3]) {
    double sum1=0, sum2=0, sum3=0, sum4=0;
    for (int i=0; i < 3; i++) {
        sum1 += pow((target[i] - vertex1[i]),2);
        sum2 += pow((target[i] - vertex2[i]),2);
        sum3 += pow((target[i] - vertex3[i]),2);
        sum4 += pow((target[i] - vertex4[i]),2);
    }
    
    if (sum1 < sum2 && sum1<sum3 && sum1 < sum4) {
        return vertex1;
    }
    else {
        if (sum2 < sum3 && sum2 < sum4) {
            return vertex2;
        }
        else {
            if (sum3 < sum4) {
                return vertex3;
            }
            else {
                return vertex4;
            }
        }
    }
}

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int BytesPerPixel = 3, Size = 512;               // Define the variables
    
    // argv[1] = "/Users/YJLee/Desktop/Sailboat.raw"
    // argv[2] = "/Users/YJLee/Desktop/Sailboat_V.raw"
    
    // Check for proper syntax
    if (argc < 3) {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [input_image.raw] [output_image.raw]" << endl;
        return 0;
    }
    
    /*
    if ((atoi(argv[3]) != 4) && (atoi(argv[3]) != 8)) {
        cout << "Syntax Error - Index Number = 4 or 8" << endl;
        return 0;
    }
     */
    
    unsigned char Imagedata[Size][Size][BytesPerPixel];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    else {
        cout << "Image successfully is loaded" <<endl;
    }
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Extend the boundary
    double MetaImage[Size+2][Size+2][BytesPerPixel];
    
    for (int y=0; y<Size; y++) {
        for (int x=0; x<Size; x++) {
            for (int ch=0; ch<BytesPerPixel; ch++) {
                MetaImage[y+1][x+1][ch] = Imagedata[y][x][ch];
            }
        }
    }
    // copy upper and lower boundaries
    for (int x=0; x<Size; x++) {
        for (int ch=0; ch<BytesPerPixel; ch++) {
            MetaImage[0][x+1][ch] = Imagedata[0][x][ch];
            MetaImage[Size+1][x+1][ch] = Imagedata[Size-1][x][ch];
        }
    }
    // copy left and right boundaries from itself
    for (int y=0; y<(Size+2); y++) {
        for (int ch=0; ch<BytesPerPixel; ch++) {
            MetaImage[y][0][ch] = MetaImage[y][1][ch];
            MetaImage[y][Size+1][ch] = MetaImage[y][Size][ch];
        }
    }
    
    unsigned char ImageOutput[Size][Size][BytesPerPixel];
    
    // Vertices matrix
    int KVert[] = {0,0,0}, RVert[] = {255,0,0}, GVert[] = {0,255,0}, BVert[] = {0,0,255};
    int CVert[] = {0,255,255}, MVert[] = {255,0,255}, YVert[] = {255,255,0}, WVert[] = {255,255,255};
    
    // How to choose proper vertex and distribute the error:
    // 1. Distribute the pixel into one of 6 spaces
    // (according to Fig 2, "Color Diffusion: Error-Diffusion for Color Halftones", Doron Shaked, Nur Arad, Andrew Fitzhugh, Irwin Sobel)
    // 2. Determine which vertex is the closet one - assign to that one
    // 3. Calculate the error and distribute to future pixels
    
    
    for (int y = 0; y < Size; ++y) {
        // Apply serpentine algorithm
        // Check whether in odd row or even row
        // even row - ascending count
        
        if (y%2 == 0) {
            for (int x = 0; x < Size; ++x) {
                //printf("(x, y)= %d, %d, value= %.2f, %.2f, %.2f \n", x, y, MetaImage[y+1][x+1][0], MetaImage[y+1][x+1][1], MetaImage[y+1][x+1][2]);
                double RGB[] = {(MetaImage[y+1][x+1][0]), (MetaImage[y+1][x+1][1]), (MetaImage[y+1][x+1][2])};
                double err[BytesPerPixel];
                
                if (RGB[0]+RGB[1]>255) {
                    if (RGB[1]+RGB[2]>255) {
                        if (RGB[0]+RGB[1]+RGB[2]>510) {
                            //cout << "Fall in CMYW space" << endl;
                            int *NearstVert = MinDistFinder(RGB, CVert, MVert, YVert, WVert);
                            for (int ch=0; ch<BytesPerPixel; ch++) {
                                ImageOutput[y][x][ch] = NearstVert[ch];
                                err[ch] = MetaImage[y+1][x+1][ch] - ImageOutput[y][x][ch];
                            }
                        }
                        else {
                            //cout << "GCMY space" << endl;
                            int *NearstVert = MinDistFinder(RGB, CVert, MVert, YVert, GVert);
                            for (int ch=0; ch<BytesPerPixel; ch++) {
                                ImageOutput[y][x][ch] = NearstVert[ch];
                                err[ch] = MetaImage[y+1][x+1][ch] - ImageOutput[y][x][ch];
                            }
                        }
                    }
                    else {
                        //cout << "RGMY space" << endl;
                        int *NearstVert = MinDistFinder(RGB, RVert, MVert, YVert, GVert);
                        for (int ch=0; ch<BytesPerPixel; ch++) {
                            ImageOutput[y][x][ch] = NearstVert[ch];
                            err[ch] = MetaImage[y+1][x+1][ch] - ImageOutput[y][x][ch];
                        }
                    }
                }
                else {
                    if (RGB[1]+RGB[2]<=255) {
                        if (RGB[0]+RGB[1]+RGB[2]<=255) {
                            //cout << "KRGB space" << endl;
                            int *NearstVert = MinDistFinder(RGB, KVert, RVert, GVert, BVert);
                            for (int ch=0; ch<BytesPerPixel; ch++) {
                                ImageOutput[y][x][ch] = NearstVert[ch];
                                err[ch] = MetaImage[y+1][x+1][ch] - ImageOutput[y][x][ch];
                            }
                        }
                        else {
                            //cout << "RGBM space" << endl;
                            int *NearstVert = MinDistFinder(RGB, MVert, RVert, GVert, BVert);
                            for (int ch=0; ch<BytesPerPixel; ch++) {
                                ImageOutput[y][x][ch] = NearstVert[ch];
                                err[ch] = MetaImage[y+1][x+1][ch] - ImageOutput[y][x][ch];
                            }
                        }
                    }
                    else {
                        //cout << "GBCM space" << endl;
                        int *NearstVert = MinDistFinder(RGB, CVert, MVert, GVert, BVert);
                        for (int ch=0; ch<BytesPerPixel; ch++) {
                            ImageOutput[y][x][ch] = NearstVert[ch];
                            err[ch] = MetaImage[y+1][x+1][ch] - ImageOutput[y][x][ch];
                        }
                    }
                }
                
                // "diffuse" the error to neighbor pixels
                for (int ch=0; ch<BytesPerPixel; ++ch) {
                    //printf("Before: %.2f, %.2f, %.2f, %.2f \n", MetaImage[y+1][x+2][ch], MetaImage[y+2][x+2][ch], MetaImage[y+2][x+1][ch], MetaImage[y+2][x][ch]);
                    MetaImage[y+1][x+2][ch] += 0.4375*err[ch];
                    MetaImage[y+2][x+2][ch] += 0.0625*err[ch];
                    MetaImage[y+2][x+1][ch] += 0.3125*err[ch];
                    MetaImage[y+2][x][ch] += 0.1875*err[ch];
                    //printf("After: %.2f, %.2f, %.2f, %.2f \n", MetaImage[y+1][x+2][ch], MetaImage[y+2][x+2][ch], MetaImage[y+2][x+1][ch], MetaImage[y+2][x][ch]);
                }
            }
        }
        // odd row - decending count
        else {
            for (int x = (Size-1); x >= 0; --x) {
                // printf("(x, y)= %d, %d, value= %f \n", x, y, MetaImage[y+1][x+1][ch]);
                double RGB[] = {(MetaImage[y+1][x+1][0]), (MetaImage[y+1][x+1][1]), (MetaImage[y+1][x+1][2])};
                double err[BytesPerPixel];
                
                if (RGB[0]+RGB[1]>255) {
                    if (RGB[1]+RGB[2]>255) {
                        if (RGB[0]+RGB[1]+RGB[2]>510) {
                            //CMYW space
                            int *NearstVert = MinDistFinder(RGB, CVert, MVert, YVert, WVert);
                            for (int ch=0; ch<BytesPerPixel; ch++) {
                                ImageOutput[y][x][ch] = NearstVert[ch];
                                err[ch] = MetaImage[y+1][x+1][ch] - ImageOutput[y][x][ch];
                            }
                        }
                        else {
                            //GCMY
                            int *NearstVert = MinDistFinder(RGB, CVert, MVert, YVert, GVert);
                            for (int ch=0; ch<BytesPerPixel; ch++) {
                                ImageOutput[y][x][ch] = NearstVert[ch];
                                err[ch] = MetaImage[y+1][x+1][ch] - ImageOutput[y][x][ch];
                            }
                        }
                    }
                    else {
                        //RGMY
                        int *NearstVert = MinDistFinder(RGB, RVert, MVert, YVert, GVert);
                        for (int ch=0; ch<BytesPerPixel; ch++) {
                            ImageOutput[y][x][ch] = NearstVert[ch];
                            err[ch] = MetaImage[y+1][x+1][ch] - ImageOutput[y][x][ch];
                        }
                    }
                }
                else {
                    if (RGB[1]+RGB[2]<=255) {
                        if (RGB[0]+RGB[1]+RGB[2]<=255) {
                            //KRGB
                            int *NearstVert = MinDistFinder(RGB, KVert, RVert, GVert, BVert);
                            for (int ch=0; ch<BytesPerPixel; ch++) {
                                ImageOutput[y][x][ch] = NearstVert[ch];
                                err[ch] = MetaImage[y+1][x+1][ch] - ImageOutput[y][x][ch];
                            }
                        }
                        else {
                            //RGBM
                            int *NearstVert = MinDistFinder(RGB, MVert, RVert, GVert, BVert);
                            for (int ch=0; ch<BytesPerPixel; ch++) {
                                ImageOutput[y][x][ch] = NearstVert[ch];
                                err[ch] = MetaImage[y+1][x+1][ch] - ImageOutput[y][x][ch];
                            }
                        }
                    }
                    else {
                        //GBCM
                        int *NearstVert = MinDistFinder(RGB, CVert, MVert, GVert, BVert);
                        for (int ch=0; ch<BytesPerPixel; ch++) {
                            ImageOutput[y][x][ch] = NearstVert[ch];
                            err[ch] = MetaImage[y+1][x+1][ch] - ImageOutput[y][x][ch];
                        }
                    }
                }
                
                // "diffuse" the error to neighbor pixels
                for (int ch=0; ch<BytesPerPixel; ++ch) {
                    MetaImage[y+1][x][ch] += 0.4375*err[ch];
                    MetaImage[y+2][x][ch] += 0.0625*err[ch];
                    MetaImage[y+2][x+1][ch] += 0.3125*err[ch];
                    MetaImage[y+2][x+2][ch] += 0.1875*err[ch];
                                 }
            }
        }
    }

    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(ImageOutput, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    cout << "Scaled image is successfully saved" <<endl;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif