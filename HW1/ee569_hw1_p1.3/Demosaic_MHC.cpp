//
//  main.cpp
//  HW1_P1.3
//
//  MHC Demosaicing
//  Implement MHC demosaicing (424x636 Parrot image is defaulted)
//
//  Input: Raw image, desired output file path, (Number of color channel), (input height), (output width)
//  Output: MHC demosaic image
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef DEMOSAIC_RAWIMG_MHC
#define DEMOSAIC_RAWIMG_MHC

using namespace std;

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int width = 424, height = 636, BytesPerPixel = 3;                   // Define the variables
    
    // argv[1] = "/Users/YJLee/Desktop/parrot_CFA.raw"
    // argv[2] = "/Users/YJLee/Desktop/parrot_MHC.raw"
    
    // Check for proper syntax
    cout << "Argument count: " << argc << endl;
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw" << endl;
        return 0;
    }
    // Check if size is specified
    if (argc >= 5){
        height = atoi(argv[4]);
        width = atoi(argv[5]);
    }

    // Read the image contents by fread(ptr,width,count,fp)
    unsigned char Imagedata[height][width][1];
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), height*width*1, file);
    fclose(file);
    
    // "Pre-treatment: Add a frame" the image before interpolation:
    // extend the edge of original image with imaginary 2-pixels boundaries, which is needed in further interpolation operations
    unsigned char ImageAddFrame[height+4][width+4][1];
    
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            ImageAddFrame[y+2][x+2][0] = Imagedata[y][x][0];
        }
    }
    // copy upper and lower boundaries
    for (int x=0; x<width; x++) {
        ImageAddFrame[0][x+2][0] =  Imagedata[1][x][0];
        ImageAddFrame[1][x+2][0] =  Imagedata[0][x][0];
        ImageAddFrame[height+2][x+2][0] = Imagedata[height-1][x][0];
        ImageAddFrame[height+3][x+2][0] = Imagedata[height-2][x][0];
    }
    // copy left and right boundaries from itself
    for (int y=0; y<(height+4); y++) {
        ImageAddFrame[y][0][0] = ImageAddFrame[y][3][0];
        ImageAddFrame[y][1][0] = ImageAddFrame[y][2][0];
        ImageAddFrame[y][width+2][0] = ImageAddFrame[y][width+1][0];
        ImageAddFrame[y][width+3][0] = ImageAddFrame[y][width][0];
    }
    
    // MHC Demosaicing - Construct color value of each pixel according to Bayer pattern
    // The reconstructed color value is based on 2nd order correction term.
    // These color values after calculation may go less than 0 or more than 255 -- needs to be corrected
    
    unsigned char Image2ndOrder[height][width][BytesPerPixel];
    
    // Construct "Red Pixel Points"
    for (int y = 0; y <= height-1; y+=2) {
        for (int x= 0; x <= width-1; x+=2) {
            int GreenAfterCalc = round((4*ImageAddFrame[y+2][x+2][0]+2*ImageAddFrame[y+3][x+2][0]+2*ImageAddFrame[y+1][x+2][0]+
                                        2*ImageAddFrame[y+2][x+3][0]+2*ImageAddFrame[y+2][x+1][0]-1*ImageAddFrame[y+4][x+2][0]-
                                        1*ImageAddFrame[y][x+2][0]-1*ImageAddFrame[y+2][x+4][0]-1*ImageAddFrame[y+2][x][0])/8);
            int BlueAfterCalc = round((6*ImageAddFrame[y+2][x+2][0]+2*ImageAddFrame[y+3][x+3][0]+2*ImageAddFrame[y+3][x+1][0]+
                                       2*ImageAddFrame[y+1][x+3][0]+2*ImageAddFrame[y+1][x+1][0]-1.5*ImageAddFrame[y+4][x+2][0]-
                                       1.5*ImageAddFrame[y+2][x+4][0]-1.5*ImageAddFrame[y][x+2][0]-1.5*ImageAddFrame[y+2][x][0])/8);
            
            if ( GreenAfterCalc <= 255 && GreenAfterCalc >= 0) {GreenAfterCalc = (unsigned char)GreenAfterCalc;}
            else if ( GreenAfterCalc <0) {GreenAfterCalc = (unsigned char)0;}
            else if ( GreenAfterCalc >255) {GreenAfterCalc = (unsigned char)255;}
            if ( BlueAfterCalc <= 255 && BlueAfterCalc >= 0) {BlueAfterCalc = (unsigned char)BlueAfterCalc;}
            else if ( BlueAfterCalc <0) {BlueAfterCalc = (unsigned char)0;}
            else if ( BlueAfterCalc >255) {BlueAfterCalc = (unsigned char)255;}
            
            Image2ndOrder[y][x][0] = (unsigned char)round(ImageAddFrame[y+2][x+2][0]);
            Image2ndOrder[y][x][1] = GreenAfterCalc;
            Image2ndOrder[y][x][2] = BlueAfterCalc;
        }
    }
    
    // Construct "Green Pixel Points" at R row B column
    for (int y = 0; y <= height-1; y+=2) {
        for (int x= 1; x <= width-1; x+=2) {
            int RedAfterCalc = round((5*ImageAddFrame[y+2][x+2][0]+4*ImageAddFrame[y+2][x+1][0]+4*ImageAddFrame[y+2][x+3][0]-
                                      1*ImageAddFrame[y+3][x+3][0]-1*ImageAddFrame[y+3][x+1][0]-1*ImageAddFrame[y+1][x+3][0]-
                                      1*ImageAddFrame[y+1][x+1][0]-1*ImageAddFrame[y+2][x+4][0]-1*ImageAddFrame[y+2][x][0]+
                                      0.5*ImageAddFrame[y+4][x+2][0]+0.5*ImageAddFrame[y][x+2][0])/8);
            int BlueAfterCalc = round((5*ImageAddFrame[y+2][x+2][0]+4*ImageAddFrame[y+1][x+2][0]+4*ImageAddFrame[y+3][x+2][0]-
                                       1*ImageAddFrame[y+3][x+3][0]-1*ImageAddFrame[y+3][x+1][0]-1*ImageAddFrame[y+1][x+3][0]-
                                       1*ImageAddFrame[y+1][x+1][0]-1*ImageAddFrame[y+4][x+2][0]-1*ImageAddFrame[y][x+2][0]+
                                       0.5*ImageAddFrame[y+2][x+4][0]+0.5*ImageAddFrame[y+2][x][0])/8);
            
            if ( RedAfterCalc <= 255 && RedAfterCalc >= 0) {RedAfterCalc = (unsigned char)RedAfterCalc;}
            else if ( RedAfterCalc <0) {RedAfterCalc = (unsigned char)0;}
            else if ( RedAfterCalc >255) {RedAfterCalc = (unsigned char)255;}
            if ( BlueAfterCalc <= 255 && BlueAfterCalc >= 0) {BlueAfterCalc = (unsigned char)BlueAfterCalc;}
            else if ( BlueAfterCalc <0) {BlueAfterCalc = (unsigned char)0;}
            else if ( BlueAfterCalc >255) {BlueAfterCalc = (unsigned char)255;}

            Image2ndOrder[y][x][0] = RedAfterCalc;
            Image2ndOrder[y][x][1] = (unsigned char)round(ImageAddFrame[y+2][x+2][0]);
            Image2ndOrder[y][x][2] = BlueAfterCalc;
        }
    }
    // Construct "Green Pixel Points" at B row R column
    for (int y = 1; y <= height-1; y+=2) {
        for (int x= 0; x <= width-1; x+=2) {
            int RedAfterCalc = round((5*ImageAddFrame[y+2][x+2][0]+4*ImageAddFrame[y+1][x+2][0]+4*ImageAddFrame[y+3][x+2][0]-
                                      1*ImageAddFrame[y+3][x+3][0]-1*ImageAddFrame[y+3][x+1][0]-1*ImageAddFrame[y+1][x+3][0]-
                                      1*ImageAddFrame[y+1][x+1][0]-1*ImageAddFrame[y+4][x+2][0]-1*ImageAddFrame[y][x+2][0]+
                                      0.5*ImageAddFrame[y+2][x+4][0]+0.5*ImageAddFrame[y+2][x][0])/8);
            int BlueAfterCalc = round((5*ImageAddFrame[y+2][x+2][0]+4*ImageAddFrame[y+2][x+1][0]+4*ImageAddFrame[y+2][x+3][0]-
                                       1*ImageAddFrame[y+3][x+3][0]-1*ImageAddFrame[y+3][x+1][0]-1*ImageAddFrame[y+1][x+3][0]-
                                       1*ImageAddFrame[y+1][x+1][0]-1*ImageAddFrame[y+2][x+4][0]-1*ImageAddFrame[y+2][x][0]+
                                       0.5*ImageAddFrame[y+4][x+2][0]+0.5*ImageAddFrame[y][x+2][0])/8);
            
            if ( RedAfterCalc <= 255 && RedAfterCalc >= 0) {RedAfterCalc = (unsigned char)RedAfterCalc;}
            else if ( RedAfterCalc <0) {RedAfterCalc = (unsigned char)0;}
            else if ( RedAfterCalc >255) {RedAfterCalc = (unsigned char)255;}
            if ( BlueAfterCalc <= 255 && BlueAfterCalc >= 0) {BlueAfterCalc = (unsigned char)BlueAfterCalc;}
            else if ( BlueAfterCalc <0) {BlueAfterCalc = (unsigned char)0;}
            else if ( BlueAfterCalc >255) {BlueAfterCalc = (unsigned char)255;}
            
            Image2ndOrder[y][x][0] = RedAfterCalc;
            Image2ndOrder[y][x][1] = (unsigned char)round(ImageAddFrame[y+2][x+2][0]);
            Image2ndOrder[y][x][2] = BlueAfterCalc;
        }
    }
    // Construct "Blue Pixel Points"
    for (int y = 1; y <= height-1; y+=2) {
        for (int x= 1; x <= width-1; x+=2) {
            int RedAfterCalc = round((6*ImageAddFrame[y+2][x+2][0]+2*ImageAddFrame[y+3][x+3][0]+2*ImageAddFrame[y+3][x+1][0]+
                                      2*ImageAddFrame[y+1][x+3][0]+2*ImageAddFrame[y+1][x+1][0]-1.5*ImageAddFrame[y+4][x+2][0]-
                                      1.5*ImageAddFrame[y+2][x+4][0]-1.5*ImageAddFrame[y][x+2][0]-1.5*ImageAddFrame[y+2][x][0])/8);
            int GreenAfterCalc = round((4*ImageAddFrame[y+2][x+2][0]+2*ImageAddFrame[y+3][x+2][0]+2*ImageAddFrame[y+1][x+2][0]+
                                        2*ImageAddFrame[y+2][x+3][0]+2*ImageAddFrame[y+2][x+1][0]-1*ImageAddFrame[y+4][x+2][0]-
                                        1*ImageAddFrame[y][x+2][0]-1*ImageAddFrame[y+2][x+4][0]-1*ImageAddFrame[y+2][x][0])/8);
            
            if ( RedAfterCalc <= 255 && RedAfterCalc >= 0) {RedAfterCalc = (unsigned char)RedAfterCalc;}
            else if ( RedAfterCalc <0) {RedAfterCalc = (unsigned char)0;}
            else if ( RedAfterCalc >255) {RedAfterCalc = (unsigned char)255;}
            if ( GreenAfterCalc <= 255 && GreenAfterCalc >= 0) {GreenAfterCalc = (unsigned char)GreenAfterCalc;}
            else if ( GreenAfterCalc <0) {GreenAfterCalc = (unsigned char)0;}
            else if ( GreenAfterCalc >255) {GreenAfterCalc = (unsigned char)255;}

            Image2ndOrder[y][x][0] = RedAfterCalc;
            Image2ndOrder[y][x][1] = GreenAfterCalc;
            Image2ndOrder[y][x][2] = (unsigned char)round(ImageAddFrame[y+2][x+2][0]);
        }
    }
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(Image2ndOrder, sizeof(unsigned char), (height)*(width)*BytesPerPixel, file);
    fclose(file);
    cout << "MHC demosaic image is successfully saved" <<endl;
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);

    return 0;
}
#endif