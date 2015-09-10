//
//  main.cpp
//  HW1_P1.2
//
//  Bilinear Demosaicing
//  Implement bilinear demosaicing and apply it to the 424x636 Parrot image
//
//  Created by Yun-Jun Lee on 8/31/15.
//  Copyright (c) 2015 USC. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

using namespace std;

int main(int argc, const char * argv[])
{
    // Define the variables
    
    FILE *file;
    int width = 424;
    int height = 636;
    int BytesPerPixel = 3;
    
    cout << "Argument count: " << argc << endl;
    
    // argv[1] = "/Users/YJLee/Desktop/parrot_CFA.raw"
    // argv[2] = "/Users/YJLee/Desktop/parrot_bl.raw"
    
    // Check for proper syntax
    
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw" << endl;
        return 0;
    }
    cout << "original width: " << width <<endl;
    cout << "original height: " << height <<endl;
    
    // Read the image contents by fread(ptr,width,count,fp)
    unsigned char Imagedata[height][width][1];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    else {
        cout << "Image successfully loaded" <<endl;
    }
    
    fread(Imagedata, sizeof(unsigned char), height*width*1, file);
    fclose(file);

    // "Pre-treat" the image before interpolation:
    // extend the edge of original image with imaginary boundaries, which is needed in further interpolation operations
    
    unsigned char ImageAddFrame[height+2][width+2][1];
    
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            ImageAddFrame[i+1][j+1][0] = Imagedata[i][j][0];
        }
    }
    // copy upper and lower boundaries
    for (int j=0; j<width; j++) {
        ImageAddFrame[0][j+1][0] = Imagedata[0][j][0];
        ImageAddFrame[height+1][j+1][0] = Imagedata[height-1][j][0];
    }
    // copy left and right boundaries from itself
    for (int i=0; i<(height+2); i++) {
        ImageAddFrame[i][0][0] = ImageAddFrame[i][1][0];
        ImageAddFrame[i][width+1][0] = ImageAddFrame[i][width][0];
    }
    
    // Bilinear Demosaicing - construct RGB value of each pixel according to Bayer pattern
    // The reconstructed color value is computed as the average of its two or four adjacent pixels of the same color.
    
    unsigned char ImageOutput[height][width][BytesPerPixel];
    
    // Construct "Red Pixel Points"
    for (int y = 0; y <= height-1; y+=2) {
        for (int x= 0; x <= width-1; x+=2) {
            ImageOutput[y][x][0] = (unsigned char)roundf(ImageAddFrame[(y+1)][x+1][0]);
            ImageOutput[y][x][1] = (unsigned char)roundf((0.25)*(ImageAddFrame[y][x+1][0]+ImageAddFrame[y+2][x+1][0]+ImageAddFrame[y+1][x][0]+ImageAddFrame[y+1][x+2][0]));
            ImageOutput[y][x][2] = (unsigned char)roundf((0.25)*(ImageAddFrame[y][x][0]+ImageAddFrame[y+2][x][0]+ImageAddFrame[y][x+2][0]+ImageAddFrame[y+2][x+2][0]));
        }
    }
    
    // Construct "Green Pixel Points"
    for (int y = 0; y <= height-1; y+=2) {
        for (int x= 1; x <= width-1; x+=2) {
            ImageOutput[y][x][0] = (unsigned char)roundf((0.5)*(ImageAddFrame[y+1][x][0]+ImageAddFrame[y+1][x+2][0]));
            ImageOutput[y][x][1] = (unsigned char)roundf(ImageAddFrame[y+1][x+1][0]);
            ImageOutput[y][x][2] = (unsigned char)roundf((0.5)*(ImageAddFrame[y][x+1][0]+ImageAddFrame[y+2][x+1][0]));
        }
    }
    for (int y = 1; y <= height-1; y+=2) {
        for (int x= 0; x <= width-1; x+=2) {
            ImageOutput[y][x][0] = (unsigned char)roundf((0.5)*(ImageAddFrame[y][x+1][0]+ImageAddFrame[y+2][x+1][0]));
            ImageOutput[y][x][1] = (unsigned char)roundf(ImageAddFrame[y+1][x+1][0]);
            ImageOutput[y][x][2] = (unsigned char)roundf((0.5)*(ImageAddFrame[y+1][x][0]+ImageAddFrame[y+1][x+2][0]));
        }
    }
    
    // Construct "Blue Pixel Points"
    for (int y = 1; y <= height-1; y+=2) {
        for (int x= 1; x <= width-1; x+=2) {
            ImageOutput[y][x][0] = (unsigned char)roundf((0.25)*(ImageAddFrame[y][x][0]+ImageAddFrame[y+2][x][0]+ImageAddFrame[y][x+2][0]+ImageAddFrame[y+2][x+2][0]));
            ImageOutput[y][x][1] = (unsigned char)roundf((0.25)*(ImageAddFrame[y][x+1][0]+ImageAddFrame[y+2][x+1][0]+ImageAddFrame[y+1][x][0]+ImageAddFrame[y+1][x+2][0]));
            ImageOutput[y][x][2] = (unsigned char)roundf(ImageAddFrame[y+1][x+1][0]);
        }
    }
    
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    FILE *new_file;
    if (!(new_file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(ImageOutput, sizeof(unsigned char), (height)*(width)*BytesPerPixel, new_file);
    fclose(new_file);
    cout << "Bi-linear demosaic image successfully saved" <<endl;
    
    return 0;
}