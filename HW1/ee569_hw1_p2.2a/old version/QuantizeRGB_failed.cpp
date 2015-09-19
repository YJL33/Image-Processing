//
//  main.cpp
//  ee569_hw1_p2.2
//
//  Image Filtering - Creating Oil Painting Effect
//  Step 1. Quantizing original 24-bit RGB raw image (barn.raw is defaulted)
//
//  Based on color level, separate each channel into quantized parts in equal number.
//  Then assign new RGB values for each of them, based on average RGB value of each part.
//
//  Input:
//  Raw image; path of desired quantized image; (quantize number); (height of image); (width of image);
//
//  Output:
//  enhanced raw image, argv[2];
//
//  Created by Yun-Jun Lee on 9/7/15.
//  Copyright (c) 2015 USC. All rights reserved.
//
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#ifndef QUANTIZE_RAWIMG
#define QUANTIZE_RAWIMG

using namespace std;

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int ColorSpan = 256, BytesPerPixel = 3, width = 580, height = 247;    // Default height/width value (barn.raw)
    int NumberOfPixels = height*width;
    int QuantizeNumber;
    unsigned char NewRGBLv[BytesPerPixel*QuantizeNumber];                 // quantized color level for each channel

    // argv[1] = "/Users/YJLee/Desktop/barn.raw"          // argv[1] = "/Users/YJLee/Desktop/coliseum.raw"
    // argv[2] = "/Users/YJLee/Desktop/barn512.raw"       // argv[2] = "/Users/YJLee/Desktop/coliseum512.raw"
    // argv[3] = 380                                      // argv[3] = 580
    // argv[4] = 275                                      // argv[4] = 247
    // argv[5] = 4

    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image64.raw (quantize number) (input_image's_height) (input_image's_width)" << endl;
        return 0;
    }
    if (argc >= 5) {
        width = (int)atoi(argv[3]);
        height = (int)atoi(argv[4]);
    }
    if (argc < 6) {
        QuantizeNumber = 4;                                // Default = 4
    }
    else {
        QuantizeNumber = (int)atoi(argv[5]);
    }
    // printf("quantize number: %d, image width: %d , image height: %d\n", QuantizeNumber, width, height);
    // Read the image contents by fread(ptr,Size,count,fp)
    unsigned char Imagedata[height][width][BytesPerPixel];
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), height*width*BytesPerPixel, file);
    fclose(file);
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Divide each channel into of parts based on quantize number, then calculate each part's average color intensity.
    unsigned char ImageQuantize[height][width][BytesPerPixel];
    int bin_count[BytesPerPixel*QuantizeNumber];                // Separate all pixels' components into corresponding quantized parts of "bins".
    int sum[BytesPerPixel*QuantizeNumber];                      // Sum of all value of each bin.
    int bin_id[BytesPerPixel];                                  // Bin counter, if current bin is full, fill next.
    
    for (int ch=0; ch<BytesPerPixel; ch++) {
        for (int lv=0; lv<ColorSpan; lv++) {
            for (int y=0; y<height; y++) {
                for (int x=0; x<width; x++) {
                    if ((round(Imagedata[y][x][ch])) == lv){                                            // Fill from 0 to 255
                        cout << lv << endl;;
                        if (bin_count[ch+BytesPerPixel*bin_id[ch]]<=(NumberOfPixels/QuantizeNumber)) {  // If the bin is not full
                            ImageQuantize[y][x][ch]=bin_id[ch];
                            sum[ch+BytesPerPixel*bin_id[ch]]+=lv;
                            bin_count[ch+BytesPerPixel*bin_id[ch]]++;
                        }
                        else {                                                                          // If the bin is full
                            bin_id[ch]++;
                            ImageQuantize[y][x][ch]=bin_id[ch];
                            sum[ch+BytesPerPixel*bin_id[ch]]+=lv;
                            bin_count[ch+BytesPerPixel*bin_id[ch]]++;
                        }
                    }
                }
            }
        }
    }
    
    cout << sum[0] << "," << sum[1] << "," << sum[2] << "," << sum[3] << "," << sum[4] << "," << sum[5] << endl;
    
    for (int ch=0; ch<BytesPerPixel; ch++) {
        for (int qt=0; qt<QuantizeNumber; qt++) {
            NewRGBLv[ch+qt*BytesPerPixel] = (unsigned char)round((double)sum[ch+BytesPerPixel*qt]/(double)bin_count[ch+BytesPerPixel*qt]);
        }
    }
    printf("Quantize R: %d, %d, %d, %d\n", (NewRGBLv[0]),(NewRGBLv[1]),(NewRGBLv[2]),(NewRGBLv[3]));
    printf("Quantize G: %d, %d, %d, %d\n", (NewRGBLv[4]),(NewRGBLv[5]),(NewRGBLv[6]),(NewRGBLv[7]));
    printf("Quantize B: %d, %d, %d, %d\n", (NewRGBLv[8]),(NewRGBLv[9]),(NewRGBLv[10]),(NewRGBLv[11]));
    
    // Assign new RGB values (only different quantized levels) to each pixel.
    for (int x=0; x<width; x++) {
        for (int y=0; y<height; y++) {
            for (int ch=0; ch<BytesPerPixel; ch++) {
                ImageQuantize[y][x][ch] = NewRGBLv[ch+ImageQuantize[y][x][ch]*BytesPerPixel];
                }
            }
        }
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save img file" << endl;
        exit(1);
    }
    fwrite(ImageQuantize, sizeof(unsigned char), (height)*(width)*BytesPerPixel, file);
    fclose(file);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif