//
//  FeatureExtraction.cpp
//  HW4_P1.a.1
//
//  Extract the features from multiple files, and output the feature array.
//  The output will be 2-D array: [features (10)] * [number of samples (18)]
//
//  **** Execute once, get feature arrays of all samples in such class. *****
//
//  Input: [the full description of sets of files]...................... e.g. "../part_a/unknown_%02d.raw"
//         [output "feature array"]..................................... 2-D array: [features] * [number of samples (18)]
//
//  In order to deal with multiple files, following steps are wrapped in a loop.
//  Features are extracted from following functions:
//  1. Normalized area: real area/boundary box(BB) area
//  2. Normalized perimeter: real perimeter/BB perimeter
//  3~4. Spatial Moments: The (m,n)th moment of the x-y joint probability, (m,n) = (1,0) (0,1)
//  5. Enclosed Area: enclosed area/real area
//  6. Circularity: A*p^2/4PI
//  7. Aspect ratio: BB width/BB height
//  8. Euler Numbers (4-connectivity): Apply Gray's Algorithm
//  9. Horizontal symmetry: top part - bottom part
//  10. Vertical symmetry: left part - right part
//  11~12. Enclosed center: similar to 3~4
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef FEATURE_EXTRACTION
#define FEATURE_EXTRACTION

const double PI  =3.141592653589793238463;

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int Height = 245, Width = 224;                     // Define the variables
    int NumOfCharacter = 18;
    int NumberOfFeatures = 12;
    
    double Feature[NumberOfFeatures][NumOfCharacter];
    for (int ch=0; ch<NumOfCharacter; ch++) {
        for (int ft=0; ft<NumberOfFeatures; ft++) {
            Feature[ft][ch] = 0;
        }
    }
    
    // argv[1] = "/Users/YJLee/Desktop/ch/%02d.raw";
    // argv[2] = "/Users/YJLee/Desktop/Feat.array"
    
    cout << "Example of directory of input image name: [/Users/YJLee/Desktop/ch/]" << endl;

    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [directory where the character's image is] [Feature array]" << endl;
        return 0;
    }
    
    unsigned char Imagedata[Height][Width];                               // Imagedata
    
    for (int ch = 0; ch < NumOfCharacter; ch++) {        // use "for loop" to handle all image iteratively
                                                         //***** IMPORTANT: this loop lasts through step 2. *****
        char filepath[44] = {0};
        char filename[] = "%02d_s.raw";                    // Combine the directory and filename for Input images
        strcat(filepath, argv[1]);
        strcat(filepath, filename);
        sprintf(filepath, filepath, (char)(ch+1));
        
        cout << "\nExtracting image feature from:  \n" << filepath << " \n" << endl;
        
        if (!(file=fopen(filepath,"rb")))
        {
            cout << "Error: unable to open file" <<endl;
            exit(1);
        }
        fread(Imagedata, sizeof(unsigned char), Height*Width, file);      // Read the image contents by fread(ptr,size,count,fp)
        fclose(file);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////

        //  1. Normalized area: real area/boundary box(BB) area
        //  2. Normalized perimeter: real perimeter/BB perimeter
        
        int min_x=Width, max_x=0, min_y=Height, max_y=0;                  // Initialize the boundary of each object (character)
        int char_pixel_count=0, char_peri_count=0;
        
        for (int y=0; y<Height; ++y) {
            for (int x=0; x<Width; ++x) {
                
                if ((y > 0 && Imagedata[y-1][x] != Imagedata[y][x])) {    // Check whether this pixel is same with its neighbor or not => if not same, here is a boundary.
                    char_peri_count++;
                }
                if ((y+1 < Height && Imagedata[y+1][x] != Imagedata[y][x])) {
                    char_peri_count++;
                }
                if ((x > 0 && Imagedata[y][x-1] != Imagedata[y][x])) {
                    char_peri_count++;
                }
                if ((x+1 < Width && Imagedata[y][x+1] != Imagedata[y][x])) {
                    char_peri_count++;
                }
                
                if (Imagedata[y][x] == 0)
                {
                    char_pixel_count++;                                   // Find the "exact area" occupied by this character
                    if (x < min_x) {                                      // Find the "boundary box area" of this character
                        min_x = x;
                    };
                    if (x > max_x) {
                        max_x = x;
                    }
                    if (y < min_y) {
                        min_y = y;
                    };
                    if (y > max_y) {
                        max_y = y;
                    }
                }
            }
        }
        
        char_peri_count = char_peri_count/2;
        int char_height = (max_y-min_y+1), char_width = (max_x-min_x+1);
        int char_area = char_height*char_width, char_perimeter = 2*(char_height + char_width);
        
        printf("The boundary box: area = %dx%d = %d, perimeter = %d, \nThe character's actual area = %d, actual perimeter = %d\n", char_width, char_height, char_area, char_perimeter, char_pixel_count, char_peri_count);
        
        Feature[0][ch] = (double)(char_pixel_count)/(double)(char_area);
        Feature[1][ch] = (double)(char_peri_count)/(double)(char_perimeter);
        printf("Normalized Area: %.3f,\nNormalized Perimeter: %.3f\n", Feature[0][ch], Feature[1][ch]);
        
        // 3, 4 Spatial Moments: The (m,n)th moment of the joint probability
        
        double centroid_x=0, centroid_y=0;

        for (int j=0; j<=char_height; ++j) {
            for (int i=0; i<=char_width; ++i) {
                if (Imagedata[min_y+j][min_x+i] == 0) {
                    centroid_x = centroid_x + (i)*(1/(double)(char_area));                 // (m,n) = (1,0)
                    centroid_y = centroid_y + (j)*(1/(double)(char_area));                 // (m,n) = (0,1)
                }
            }
        }
        printf("Centroid: (%f, %f)\n", centroid_x, centroid_y);
        
        Feature[2][ch] = double(centroid_x/(char_width-1));                // Centroid deviation from midpoint in x direction
        Feature[3][ch] = double(centroid_y/(char_height-1));               // Centroid deviation from midpoint in y direction
        printf("Centroid deviation in x direction: %.3f, \nCentroid deviation in y direction: %.3f\n", Feature[2][ch], Feature[3][ch]);
        
        //  5. Enclosed Area: enclosed area/real area
        // 11~12. Enclosed center
        int enclosed_pixel_count=0;
        double enclosed_center_x=0, enclosed_center_y=0;
        for (int y=min_y; y<max_y; ++y) {                                         // Find out enclosed area
            for (int x=min_x; x<max_x; ++x) {
                if (Imagedata[y][x] == 254) {
                    enclosed_pixel_count++;
                }
            }
        }
        for (int y=min_y; y<max_y; ++y) {                                         // Find out enclosed area
            for (int x=min_x; x<max_x; ++x) {
                if (Imagedata[y][x] == 254) {
                    enclosed_center_x = enclosed_center_x + (x-min_x)*(1/(double)(enclosed_pixel_count));      // (m,n) = (1,0)
                    enclosed_center_y = enclosed_center_y + (y-min_y)*(1/(double)(enclosed_pixel_count));      // (m,n) = (0,1)
                }
            }
        }

        //printf("Enclosed point count = %d\n", enclosed_pixel_count);
        Feature[4][ch] = (double)(enclosed_pixel_count)/(double)(char_pixel_count);
        Feature[10][ch] = double(enclosed_center_x/(char_width-1));                // Enclosed area center deviation in x direction
        Feature[11][ch] = double(enclosed_center_y/(char_height-1));               // Enclosed area center deviation in y direction

        printf("Normalized enclosed area = %.3f\n", Feature[4][ch]);
        printf("Enclosed center deviate in x direction: %.3f\nEnclosed center deviate in y direction: %.3f\n", Feature[10][ch], Feature[11][ch]);
        
        //  6. Circularity: A*p^2/4PI
        //  7. Aspect ratio: BB width/BB height
        Feature[5][ch] = (4*PI*Feature[0][ch])/pow(Feature[1][ch],2);
        Feature[6][ch] = (double)(char_width)/(double)(char_height) ;
        printf("Circularity = %.3f\nAspect Ratio = %.3f\n", Feature[5][ch] , Feature[6][ch] );
        
        //  8. Euler Number
        int Window[4] = {0};
        double Qa = 0, Qc = 0, Qd = 0;
        for (int y=0; y<Height-1; ++y) {
            for (int x=0; x<Width-1; ++x) {
                int pixel_count = 0;
                int sum = 0;
                for (int j = y; j <= y+1; ++j){                                // Apply Gray Algorithm
                    for (int i = x; i <= x+1; ++i){
                        Window[pixel_count] = Imagedata[j][i];
                        pixel_count++;
                        if (Imagedata[j][i] == 0) {
                            sum++;
                        }
                    }
                }
                if (sum == 1) {                                                // Count Q1, Q3, and Qd
                    Qa++;
                }
                if (sum == 3) {
                    Qc++;
                }
                if (((Window[0] == Window[3]) && (Window[1] == Window[2])) && (Window[0] != Window[1]) ) {
                    Qd++;
                }
            }
        }

        Feature[7][ch] = 0.25*(Qa - Qc + (2*Qd));                 // EulerNumber (4-connectivity)
        printf("Euler # (4-connectivity) = %.1f\n", Feature[7][ch] );
        
        //  9. Horizontal symmetry: top part - bottom part
        //  10. Vertical symmetry: left part - right part
        
        double horizontal_diff=0, vertical_diff=0;
        int horizontal_diff_count=0, vertical_diff_count=0;
        
        for (int j=min_y; j<=max_y; ++j) {
            for (int i=min_x; i<centroid_x+min_x; ++i) {
                horizontal_diff_count++;
                horizontal_diff = horizontal_diff + ((Imagedata[j][i] - Imagedata[j][max_x-(i-min_x)])/32);
            }
        }
        
        for (int i=min_x; i<=max_x; ++i) {
            for (int j=min_y; j<centroid_y+min_y; ++j) {
                vertical_diff_count++;
                vertical_diff = vertical_diff + ((Imagedata[j][i] - Imagedata[max_y-(i-min_y)][i])/32);
            }
        }
        
        Feature[8][ch] = (horizontal_diff)/horizontal_diff_count;
        Feature[9][ch] = (vertical_diff)/vertical_diff_count;
        
        printf("Horizontal symmetry = %.3f,\nVertical symmetry = %.3f\n", Feature[8][ch], Feature[9][ch]);

    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    for (int ch=0; ch<NumOfCharacter; ch++) {
        for (int ft=0; ft<NumberOfFeatures; ft++) {
            printf(" %.03f, ", Feature[ft][ch]);
        }
        cout << endl;
    }
    
    
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(Feature, sizeof(double), NumberOfFeatures*NumOfCharacter, file);
    fclose(file);
    cout << "\n\nAll Feature vector arrays successfully saved" <<endl;

    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif