//
//  DecisionTree_(for test_ideal2 characters).cpp (modified from 1.a.2)
//  HW4_P1.b.2
//
//  Use Minimum distance classifier to classify the unknown image
// (Sample space: S P E D L I M T 0 1 2 3 6 8 9 4 5 7)
//  Feature array extracted from 1.a.1 is required.
//
//  Input: [features array]......................... 2-D 12x18 array obtained from 1.1
//         directory of unknown image
//
//  Min-dist-classifier:
//  step 1.   Extract the feature from the unknown image.
//  step 2.   Apply minimum distance classifier on geometry features.
//  step 3.   Classify it.
//
//  Difference b/w 1.a.2
//  1. The image size is different
//  2. The unknown characters is now only 12
//
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef MIN_DISTANCE_CLASSIFIER
#define MIN_DISTANCE_CLASSIFIER

const double PI  =3.141592653589793238463;

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int Height = 245, Width = 224;                     // Define the variables
    int NumOfCharacter = 18;
    int NumOfUnknownCharacter = 12;
    int NumberOfFeatures = 12;
    
    // argv[1] = "/Users/YJLee/Desktop/Feat.array"
    // argv[2] = "/Users/YJLee/Desktop/test2/";
    
    if (argc < 3){                                                 // Check for proper syntax
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name  [Feature array]  [directory where the unknown character's image are]" << endl;
        return 0;
    }
    
    double CharacterFeatures[NumberOfFeatures][NumOfCharacter];    // Read training image's features
    if (!(file=fopen(argv[1],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(CharacterFeatures, sizeof(double), NumberOfFeatures*NumOfCharacter, file);
    fclose(file);
    
    unsigned char Imagedata[Height][Width];                               // Imagedata
    
    for (int ch = 0; ch < NumOfUnknownCharacter; ch++) {        // use "for loop" to handle all image iteratively   ***** IMPORTANT: this loop lasts through step 2. *****
        
        char filepath[44] = {0};
        char filename[] = "%02d.raw";                    // Combine the directory and filename for Input images
        strcat(filepath, argv[2]);
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
        
        // Step 1.

        double Feature[NumberOfFeatures][NumOfCharacter];                 // Initialize Features of unknown image
        for (int ch=0; ch<NumOfCharacter; ch++) {
            for (int ft=0; ft<NumberOfFeatures; ft++) {
                Feature[ft][ch] = 0;
            }
        }
        
        
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
                if (((Window[0] == Window[3]) && (Window[1] == Window[2])) && (Window[0] != Window[1]) && ((Window[0] == 0) || (Window[1] == 0))) {
                    Qd++;
                }
            }
        }
        
        Feature[7][ch] = 0.25*(Qa - Qc + (2*Qd));                 // EulerNumber (4-connectivity)
        printf("Q1: %.0f, Q3: %.0f, Qd: %.0f, Euler # (4-connectivity) = %.1f\n", Qa, Qc, Qd, Feature[7][ch] );
        
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
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Step 2. Calculate the distance
        
        if (Feature[7][ch] == -1) {                // Euler number = -1
            printf("\n==============Guess: 8==============\n");
        }
        else if (Feature[7][ch] == 0) {            // Euler number = 0
            if (Feature[11][ch] > 0.6) {           // Enclosed area are mostly distributed low
                printf("\n==============Guess: 6==============\n");
            }
            else
            {
                if ( (Feature[4][ch] > 0.6) && ((pow((0.5-Feature[10][ch]),2) + pow((0.5-Feature[11][ch]),2)) < 0.005)) {       // Enclosed area are bigger than pixel area
                    printf("\n==============Guess: 0==============\n");
                }
                else
                {
                    double dist = 65536;            // Calculate the distance
                    int character_guess[4] = {1,3,14,15};      // P, D, 9, 4
                    char character_candidates[4] = {'P', 'D', '9', '4'};
                    int best_guess=19;
                    for (int k=0; k<4; ++k) {
                        double dist_temp = 0;
                        for (int ft=0; ft<NumberOfFeatures; ++ft) {
                            dist_temp += pow((CharacterFeatures[ft][character_guess[k]] - Feature[ft][ch]), 2);
                        }
                        printf("dist to %d_s.raw: %f\n", k+1, dist_temp);
                        if (dist > dist_temp) {
                            dist = dist_temp;
                            best_guess = k;
                        }
                    }
                    printf("\n==============Guess: %c==============\n", character_candidates[best_guess]);
                }
            }
        }
        else if (Feature[7][ch] == 1) {                // Euler number = 1
            if (Feature[0][ch] > 0.8) {                // Normalized area (actual pixel/boundary box)
                printf("\n==============Guess: I==============\n");
            }
            else
            {
                if (Feature[6][ch] > 0.9) {            // This character is wide
                    printf("\n==============Guess: M==============\n");
                }
                else if (Feature[6][ch] < 0.5) {       // This character is thin
                    printf("\n==============Guess: 1==============\n");
                }
                else {
                    if (Feature[8][ch] < -2) {          // The character is distributed more in left (E, L)
                        double dist = 65536;            // Calculate the distance
                        int character_guess[2] = {2,4};      // E, L
                        char character_candidates[2] = {'E', 'L'};
                        int best_guess=19;
                        for (int k=0; k<2; ++k) {
                            double dist_temp = 0;
                            for (int ft=0; ft<NumberOfFeatures; ++ft) {
                                dist_temp += pow((CharacterFeatures[ft][character_guess[k]] - Feature[ft][ch]), 2);
                            }
                            printf("dist to %d_s.raw: %f\n", k+1, dist_temp);
                            if (dist > dist_temp) {
                                dist = dist_temp;
                                best_guess = k;
                            }
                        }
                        printf("\n==============Guess: %c==============\n", character_candidates[best_guess]);
                    }
                    else if (Feature[8][ch] > 1.4) {     // The character is distributed more in rignt (3)
                        printf("\n==============Guess: 3==============\n");
                    }
                    else
                    {
                        if (Feature[9][ch] > 1) {     // The character is distributed more in bottom (2)
                            printf("\n==============Guess: 2==============\n");
                        }
                        else if (Feature[9][ch] < -2.5) {     // The character is distributed more in top (T, 7)
                            double dist = 65536;              // Calculate the distance
                            int character_guess[2] = {7,17};      // T, 7
                            char character_candidates[2] = {'T', '7'};
                            int best_guess=19;
                            for (int k=0; k<2; ++k) {
                                double dist_temp = 0;
                                for (int ft=0; ft<NumberOfFeatures; ++ft) {
                                    dist_temp += pow((CharacterFeatures[ft][character_guess[k]] - Feature[ft][ch]), 2);
                                }
                                printf("dist to %d_s.raw: %f\n", k+1, dist_temp);
                                if (dist > dist_temp) {
                                    dist = dist_temp;
                                    best_guess = k;
                                }
                            }
                            printf("\n==============Guess: %c==============\n", character_candidates[best_guess]);
                        }
                        else {
                            double dist = 65536;                  // Calculate the distance
                            int character_guess[2] = {0,16};      // S, 5
                            char character_candidates[2] = {'S', '5'};
                            int best_guess=19;
                            for (int k=0; k<2; ++k) {
                                double dist_temp = 0;
                                for (int ft=5; ft<9; ++ft) {      // Focus in some feats.
                                    dist_temp += (CharacterFeatures[ft][character_guess[k]] - Feature[ft][ch]);
                                }
                                printf("dist to %d_s.raw: %f\n", k+1, dist_temp);
                                if (pow(dist,2) > pow(dist_temp,2)) {
                                    dist = dist_temp;
                                    best_guess = k;
                                }
                            }
                            printf("\n==============Guess: %c==============\n", character_candidates[best_guess]);
                        }
                    }
                }
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif
    


