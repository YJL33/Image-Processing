//
//  SmoothCharacters.cpp
//  HW4_1.a.0.3
//
//  Apply the hole-filing process first,
//  using concepts of "structure elements": closing process (dilation => erosion)
//
//  Steps:
//  0. Normalization (White=255, Black=0 => White=0, Black=1; assume that black is default object)
//  1. Dilation
//  2. Erosion
//  3. un-normalization
//
//
//  Input: Raw image, desired output file path
//  Output: Pretreated image
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string>

#ifndef SMOOTHCHARACTERS
#define SMOOTHCHARACTERS

using namespace std;

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    int NumOfObjects = 12;
    int BytesPerPixel=1, Height = 245, Width = 224;
    
    FILE *file;
    // Define the variables

    //argv[1] = "/Users/YJLee/Desktop/P4_Output/P1/ch/";
    //argv[2] = "/Users/YJLee/Desktop/P4_Output/P1/ch_s/";
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [directory where characters are] [directory for output]" << endl;
        return 0;
    }
    
    for (int object_id=1; object_id <= NumOfObjects; ++object_id) {
        
        char filepath[42] = {0};
        char filename[] = "%02d.raw";           // Combine the directory and filename for Input images
        strcat(filepath, argv[1]);
        strcat(filepath, filename);
        sprintf(filepath, filepath, (char)(object_id));
        
        unsigned char Imagedata[Height][Width][BytesPerPixel];
        
        if (!(file=fopen(filepath,"rb"))) {
            cout << "Error: unable to open file" <<endl;
            exit(1);
        }
        fread(Imagedata, sizeof(unsigned char), Height*Width*BytesPerPixel, file);
        fclose(file);
        
        /////////////////////////////////////////////////////////////////////////////////////////////////
        
        unsigned char ImageOutput[Height][Width][1];
        unsigned char MetaImage[Height][Width][1];                                // Enclosed area is 254 in the image data, change it into 255.
        
        // Step 0. Make it into 0 or 1
        for (int y=0; y<Height; ++y) {
            for (int x=0; x<Width; ++x) {
                MetaImage[y][x][0] = Imagedata[y][x][0];
                if (MetaImage[y][x][0] == 254){
                    MetaImage[y][x][0] = 255;
                }
                ImageOutput[y][x][0] = 1-(MetaImage[y][x][0])/255;
            }
        }
        
        // Extend the boundary
        bool ImageAddFrame[Height+2][Width+2][1];
        
        for (int y=0; y<Height; y++) {
            for (int x=0; x<Width; x++) {
                ImageAddFrame[y+1][x+1][0] = (ImageOutput[y][x][0]);
            }
        }
        // copy upper and lower boundaries
        for (int x=0; x<Width; x++) {
            ImageAddFrame[0][x+1][0] = (ImageAddFrame[1][x+1][0]);
            ImageAddFrame[Height+1][x+1][0] = (ImageAddFrame[Height][x+1][0]);
        }
        // copy left and right boundaries from itself
        for (int y=0; y<(Height+2); y++) {
            ImageAddFrame[y][0][0] = ImageAddFrame[y][1][0];
            ImageAddFrame[y][Width+1][0] = ImageAddFrame[y][Width][0];
        }

        // Step 1. Dilation
        bool ImageDil[Height][Width][1];
        for (int y=0; y<Height; ++y) {
            for (int x=0; x<Width; ++x) {
                ImageDil[y][x][0] =
                ((ImageAddFrame[y+1][x+1][0] || ImageAddFrame[y+1][x][0] || ImageAddFrame[y+1][x-1][0]) ||
                 (ImageAddFrame[y][x+1][0] || ImageAddFrame[y][x][0] || ImageAddFrame[y][x-1][0]) ||
                 (ImageAddFrame[y-1][x+1][0] || ImageAddFrame[y-1][x][0] || ImageAddFrame[y-1][x-1][0])
                 );
                //printf("(%d, %d)= %d\n",x,y,ImageDil[y][x][0]);
            }
        }
        
        // Extend the boundary again
        for (int y=0; y<Height; y++) {
            for (int x=0; x<Width; x++) {
                ImageAddFrame[y+1][x+1][0] = ImageDil[y][x][0];
            }
        }
        // copy upper and lower boundaries
        for (int x=0; x<Width; x++) {
            ImageAddFrame[0][x+1][0] = ImageAddFrame[1][x+1][0];
            ImageAddFrame[Height+1][x+1][0] = ImageAddFrame[Height][x+1][0];
        }
        // copy left and right boundaries from itself
        for (int y=0; y<(Height+2); y++) {
            ImageAddFrame[y][0][0] = ImageAddFrame[y][1][0];
            ImageAddFrame[y][Width+1][0] = ImageAddFrame[y][Width][0];
        }
        

        // Step 2. Erosion
        bool ImageEro[Height][Width][1];
        for (int y=0; y<Height; ++y) {
            for (int x=0; x<Width; ++x) {
                ImageEro[y][x][0] =
                (ImageAddFrame[y+2][x][0] && ImageAddFrame[y+2][x+1][0] && ImageAddFrame[y+2][x-1][0] &&
                 ImageAddFrame[y+1][x][0] && ImageAddFrame[y+1][x+1][0] && ImageAddFrame[y+1][x-1][0] &&
                 ImageAddFrame[y][x][0] && ImageAddFrame[y][x+1][0] && ImageAddFrame[y][x-1][0] &&
                 ImageAddFrame[y-1][x][0] && ImageAddFrame[y-1][x+1][0] && ImageAddFrame[y-1][x-1][0] &&
                 ImageAddFrame[y-2][x][0] && ImageAddFrame[y-2][x+1][0] && ImageAddFrame[y-2][x-1][0] );
                //printf("(%d, %d)= %d\n",x,y,ImageEro[y][x][0]);
            }
        }
        
        for (int y=0; y<Height; ++y) {
            for (int x=0; x<Width; ++x) {
                ImageOutput[y][x][0] = 255*(1-ImageEro[y][x][0]);         // Convert it back to 0 to 255
                if (Imagedata[y][x][0] == 254){                           // Mark the enclosed area 254
                    ImageOutput[y][x][0] = 254;
                }
            }
        }
        
        int wrong_enclosedpoint_count = 1;                                // Now, some enclosed pixels are now have wrong value 255
        while (wrong_enclosedpoint_count != 0) {
            
            wrong_enclosedpoint_count = 0;
            
            for (int y=0; y<Height; y++){
                for (int x=0; x<Width; x++){
                    
                    if (ImageOutput[y][x][0] == 255) {                    // Not sure whether its background or enclosed area
                        
                        for (int j = y - 1; j <= y + 1; j++){
                            for (int i = x - 1; i <= x + 1; i++){
                                if (j >= 0 && j < 70 && i >= 0 && i < 64){
                                    if (ImageOutput[j][i][0] == 254) {    // It has a neighbor = 254 (enclosed)
                                        wrong_enclosedpoint_count++;      // Count it
                                        ImageOutput[y][x][0] = 254;       // And correct it to 254
                                    }
                                }
                            }
                        }
            
                    }
                }
            }
        }

        
        /////////////////////////////////////////////////////////////////////////////////////////////////

        // Save the output_array into output image by fwrite(), the parameters are similar to fread()
        
        char filepath_s[44] = {0};                                            // Combine the directory and filename for Output images
        char filename_s[] = "%02d_s.raw";
        strcat(filepath_s, argv[2]);
        strcat(filepath_s, filename_s);
        sprintf(filepath_s, filepath_s, (char)(object_id));
        
        if (!(file=fopen(filepath_s,"wb"))) {
            cout << "Error: unable to save file" << endl;
            exit(1);
        }
        
        fwrite(ImageOutput, sizeof(unsigned char), Height*Width*BytesPerPixel, file);
        fclose(file);
        printf("Smoothed image %d is successfully saved: %s\n", object_id, filepath_s);
    }
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif