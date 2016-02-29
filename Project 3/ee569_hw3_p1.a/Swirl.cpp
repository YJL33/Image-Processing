//
//  applySwirl.cpp
//  HW3_P1
//
//  Apply swirl effect on a raw square image (512x512 is defaulted according to the homework assignment)
//
//  Input: Raw square image, desired output file path
//  Output: Swirled square image
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef SWIRL_EFF
#define SWIRL_EFF

using namespace std;

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int BytesPerPixel=3, Size = 512;                      // Define the variables
    float imgCenter = (Size-1)/2;
    double PI = atan(1) * 4;
    
    //argv[1] = "/Users/YJLee/Desktop/Kate.raw";
    //argv[2] = "/Users/YJLee/Desktop/Kate_swirl.raw";
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw " << endl;
        return 0;
    }
    
    // Read the image contents by fread(ptr,size,count,fp)
    unsigned char Imagedata[Size][Size][BytesPerPixel];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////
    // The idea is to similar to resizing.
    // Reverse the process from the output image, in order to find out the color value of pixel.
    // According to the "position-should-be", calculate the element of each point.
    // Here all consider as Cartesian coordinate
    
    unsigned char ImageOutput[Size][Size][BytesPerPixel];

    for (int x = 0; x < Size; x++) {
        for (int  y= 0; y < Size; y++) {

            // convert image coordinate into cartesian - using image center as origin.
            double x_cart = x-imgCenter;
            double y_cart = y-imgCenter;
                
            // distance from origin to pixel
            double radius = sqrt((x_cart)*(x_cart) + (y_cart)*(y_cart));
            // rotation degree is propotional to radius
            double theta = -90*(radius/(imgCenter));
            printf( "theta: %f \n", theta);

            // calculate reversed location
            double x_cart_new = cos(theta*PI/180)*x_cart - sin(theta*PI/180)*y_cart;
            double y_cart_new = sin(theta*PI/180)*x_cart + cos(theta*PI/180)*y_cart;

            // convert the cartesian into image coordinate
            double x_new = x_cart_new + imgCenter;
            double y_new = y_cart_new + imgCenter;

            for(int channel = 0; channel < BytesPerPixel; channel++){

                float a, b;                                                  // The weight of neighbor points
                if ((int)floorf(y_new) == (int)ceilf(y_new)) {
                    a = 1.0;
                } else {
                    a = ceilf(y_new) - y_new;
                }
                if ((int)floorf(x_new) == (int)ceilf(x_new)) {
                    b = 1.0;
                } else {
                    b = ceilf(x_new) - x_new;
                }

                if ( x_new >= 0 && x_new < 512 && y_new >= 0 && y_new < 512)
                {
                    // To sum four neighbor components into one point
                    ImageOutput[x][y][channel] = static_cast<unsigned char>(roundf(
                        (a*b*Imagedata[(int)floorf(x_new)][(int)floorf(y_new)][channel])
                        +((1.0-a)*b*Imagedata[(int)floorf(x_new)][(int)ceilf(y_new)][channel])
                        +(a*(1.0-b)*Imagedata[(int)ceilf(x_new)][(int)floorf(y_new)][channel])
                        +((1.0-a)*(1.0-b)*Imagedata[(int)ceilf(x_new)][(int)ceilf(y_new)][channel]))
                    );
                }
                else
                {
                    ImageOutput[y][x][channel] = 0;
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