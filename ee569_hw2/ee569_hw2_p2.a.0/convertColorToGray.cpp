//
//  HW2.b.0
//
//  Convert RGB to Gray
//
//  Input: Raw RGB square image, desired path of gray image, (input size)
//  Output: Gray square image
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef CONVERT_GRAY
#define CONVERT_GRAY

using namespace std;

int main(int argc, const char * argv[]) {
    clock_t begin = clock();
    FILE *file;
    int BytesPerPixel=3, Height = 321, Width = 481;                              // Define the variables
    float convert_ratio[3] = {0.2989, 0.5870, 0.1140};                          // RGB coefficient for converting
    
    //argv[1] = "/Users/YJLee/Desktop/*.raw";
    //argv[2] = "/Users/YJLee/Desktop/*_gray.raw";
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 3] [Width = 481] [Height = 321]" << endl;
        return 0;
    }
    
    // Check if size is specified
    if (argc >= 4){
        BytesPerPixel = atoi(argv[3]);
        Width = atoi(argv[4]);
        Height = atoi(argv[5]);
    }
    printf("Size of image: %d x %d\n", Width, Height);

    // Read the RGB image contents by fread(ptr,size,count,fp)
    unsigned char Imagedata[Height][Width][BytesPerPixel];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Height*Width*BytesPerPixel, file);
    fclose(file);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    unsigned char ImageGray[Height][Width][1];
    
    for (int y =0; y < Height; y++) {
        for (int x =0; x < Width; x++) {
            double sum=0;
            for (int ch=0; ch < BytesPerPixel; ch++) {
                sum += convert_ratio[ch]*Imagedata[y][x][ch];
            }
            ImageGray[y][x][0] = sum;
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // Save the output_array into output Gray image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(ImageGray, sizeof(unsigned char), Height*Width*1, file);
    fclose(file);
    cout << "Gray image is successfully saved" <<endl;
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif