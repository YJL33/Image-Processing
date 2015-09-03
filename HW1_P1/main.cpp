//
//  main.cpp
//  HW1_P1
//
//  Image Resizing via Bilinear Interpolation
//  To resize a raw image, from 512x512 to 650x650 according to the homework assignment.
//
//  Created by Yun-Jun Lee on 8/29/15.
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
    int Size = 512;
    int new_Size = 650;
    int BytesPerPixel;
    
    cout << "Argument count: " << argc << endl;
    
    // argv[1] = "/Users/YJLee/Desktop/the_starry_night.raw"
    // argv[2] = "/Users/YJLee/Desktop/the_starry_night_2.raw"
    
    // Check for proper syntax
    
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 3] [Size = 512]" << endl;
        return 0;
    }
    
    // Check if image is grayscale or color
    if (argc < 4){
        BytesPerPixel = 3; // default is color image
    }
    else {
        BytesPerPixel = atoi(argv[3]);
        // Check if size is specified
    }
    
    if (argc >= 5){
        Size = atoi(argv[4]);
        new_Size = atoi(argv[5]);
        }
    
    // Determine the resizing ratio
    
    double ratio = ((((double)Size)-1) / (((double)new_Size)-1));
    
    cout << "bytes per pixel: " << BytesPerPixel << endl;
    cout << "original size: " << Size <<endl;
    cout << "target size: " << new_Size <<endl;
    cout << "original/target ratio: " << ratio <<endl;
    
    // Read the image contents by fread(ptr,size,count,fp)
    
	unsigned char Imagedata[Size][Size][BytesPerPixel];
    
    if (!(file=fopen(argv[1],"rt"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    else {
        cout << "Image successfully loaded" <<endl;
    }
    
    fread(Imagedata, sizeof(unsigned char), Size*Size*BytesPerPixel, file);
    fclose(file);

    // Resizing via bilinear interpolation
    // The idea is to squeeze the size of output_array as the input_array.
    // According to the "squeezed position", calculate the element of each point.
    
    unsigned char ImageOutput[new_Size][new_Size][BytesPerPixel];
    
    for (int x = 0; x < new_Size; x++) {
        for (int  y= 0; y < new_Size; y++) {
            for(int channel = 0; channel < BytesPerPixel; channel++){
                
                // The weight of neighbor points
                
                double a;
                if ((int)floor(ratio*y) == (int)ceil(ratio*y)) {
                    a = 1;
                } else {
                    a = (int)ceil(ratio*y) - ratio*y;
                }
                double b;
                if ((int)floor(ratio*x) == (int)ceil(ratio*x)) {
                    b = 1;
                } else {
                    b = (int)ceil(ratio*x) - ratio*x;
                }
                
                // Debug only:
                // cout << "(a,b)= (" << a << ","<< b << ")" << endl;
                
                double result_1 = round(a*b*Imagedata[(int)floor(ratio*x)][(int)floor(ratio*y)][channel]);
                double result_2 = round((1.0-a)*b*(Imagedata[(int)floor(ratio*x)][(int)ceil(ratio*y)][channel]));
                double result_3 = round(a*(1.0-b)*Imagedata[(int)ceil(ratio*x)][(int)floor(ratio*y)][channel]);
                double result_4 = round((1.0-a)*(1.0-b)*Imagedata[(int)ceil(ratio*x)][(int)ceil(ratio*y)][channel]);

                double result_all = round(result_1 + result_2 + result_3 + result_4);
                
                ImageOutput[x][y][channel] = (unsigned char)result_all;
                
                // Debug only:
                //cout << "(1,2,3,4,sum) = (" << result_1 << " " << result_2 << " "<< result_3 << " "<< result_4 << " "<< result_all << ")"<<endl;
                //cout<< "output_array[" << x << "][" << y << "]["<< channel << "]:" << round(ImageOutput[x][y][channel]) << endl;
            }
        }
    }
    
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    
    FILE *new_file;
    if (!(new_file=fopen(argv[2],"wt"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(ImageOutput, sizeof(unsigned char), new_Size*new_Size*BytesPerPixel, new_file);
    fclose(new_file);
    cout << "Scaled image successfully saved" <<endl;

    return 0;
}