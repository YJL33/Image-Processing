//
//  applyCanny.cpp
//  HW2_P2.b.1
//
//  Apply Canny detector to find out edge and contour
//
//
//  Input: [input image]......................................... single-channel 8-bit input image
//         [output image]........................................ output edge map; it has the same size and type as
//         [low threshold].......................................
//         [high threshold]......................................
//
//
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef APPLY_CANNY
#define APPLY_CANNY

using namespace cv;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE * file;
    
    // argv[1] = "/Users/YJLee/Desktop/Test.array";
    // argv[2] = "/Users/YJLee/Desktop/Grass_train.array";
    // argv[3] = "/Users/YJLee/Desktop/Non-Grass_train.array";
    
    // Check for proper syntax
    if (argc < 4)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [Test images samples] [class A (Positive) samples] [class B (Negative) samples] " << endl;
        return 0;
    }
    
    // Read all the features by fread(ptr,size,count,fp)
    double TestFeats[NumOfFeats][TestFileCount], PositiveFeats[NumOfFeats][TrainFileCount], NegativeFeats[NumOfFeats][TrainFileCount];
    if (!(file=fopen(argv[1],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(TestFeats, sizeof(double), NumOfFeats*TestFileCount, file);
    fclose(file);

    if (!(file=fopen(argv[2],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(PositiveFeats, sizeof(double), NumOfFeats*TrainFileCount, file);
    fclose(file);
    
    if (!(file=fopen(argv[3],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(NegativeFeats, sizeof(double), NumOfFeats*TrainFileCount, file);
    fclose(file);
        // Debug:
    //for (int fc=0; fc<48; fc++){
    //    printf(" \n\n tst feats: %d \n ", fc);
    //    for (int ft=0;ft<NumOfFeats; ft++)  {
    //        printf("%02f, ", TestFeats[ft][fc]);
    //    }
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Apply Canny and get eigenvectors
    PCA pca_Train(TrainMatRescaledTranspose, Mat(), CV_PCA_DATA_AS_ROW, NumOfReducedFeats);

     // Print out results
    printf("\nTotal %d Positive results in Test data are: ", count_test);
    for (int i=0; i< TestFileCount; i++) {
        if (PCA_classify_test[i] == true) {
            printf("%d, ", (i+1));
        }
    }
    printf("\n\n Total %d Positive results in Training data are: ", count_train);
    for (int i=0; i< 2*TrainFileCount; i++) {
        if (PCA_classify_train[i] == true) {
            printf("%d, ", (i+1));
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif