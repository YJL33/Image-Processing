//
//  MinMeanDistClassifier.cpp
//  HW2_P1.2
//
//  From feature array obtained from previous step, we can apply a "min distance classifier"
//  (We already know each array's dimension.)
//
//  Input: [class A (grass) mean-features array].................. 2-D array obtained from 1.1
//         [class B (straw) mean-features array].................. 2-D array obtained from 1.1
//         [unknown image features array]......................... 2-D array obtained from 1.1
//
//  For convenience, we convert the feature array into matrix.
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef MIN_DISTANCE_CLASSIFIER
#define MIN_DISTANCE_CLASSIFIER

using namespace cv;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE * file;
    int VectorLen = 25, unknownFileCount = 24, labeledFileCount = 36;         // Define the variables
    
    // argv[1] = "/Users/YJLee/Desktop/unknown_all.array";
    // argv[2] = "/Users/YJLee/Desktop/grass_all.array";
    // argv[3] = "/Users/YJLee/Desktop/straw_all.array";
    
    // Check for proper syntax
    if (argc < 4){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [class A (grass) mean-features] [class B (straw) mean-features] [unknown images' features]" << endl;
        return 0;
    }
    
    // Read all the features by fread(ptr,size,count,fp)
    double unknownFeatures[VectorLen][unknownFileCount], grassFeatures[VectorLen][labeledFileCount], strawFeatures[VectorLen][labeledFileCount];
    if (!(file=fopen(argv[1],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(unknownFeatures, sizeof(double), VectorLen*unknownFileCount, file);
    fclose(file);

    if (!(file=fopen(argv[2],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(grassFeatures, sizeof(double), VectorLen*labeledFileCount, file);
    fclose(file);
    
    if (!(file=fopen(argv[3],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(strawFeatures, sizeof(double), VectorLen*labeledFileCount, file);
    fclose(file);
   
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Convert the unknown features array into matrix
    Mat unknown_MatT, unknown_Mat = Mat(25, 24, CV_64FC1, unknownFeatures);
    transpose(unknown_Mat, unknown_MatT);
    cout<<"unknown sample: \n"<<unknown_MatT<<endl<<endl;
    
    // Get class mean and covariance matrix
    Mat grass_MatT, grass_Mat = Mat(25, 36, CV_64FC1, grassFeatures), straw_MatT, straw_Mat = Mat(25, 36, CV_64FC1, strawFeatures);
    transpose(grass_Mat, grass_MatT), transpose(straw_Mat, straw_MatT);
    
    Mat cov_g, inv_cov_g, mean_g, cov_s, inv_cov_s, mean_s;
    cv::calcCovarMatrix(grass_MatT, cov_g, mean_g, CV_COVAR_NORMAL+CV_COVAR_ROWS), cv::calcCovarMatrix(straw_MatT, cov_s, mean_s, CV_COVAR_NORMAL+CV_COVAR_ROWS);;
    cov_g = cov_g/(grass_MatT.rows-1), cov_s = cov_s/(straw_MatT.rows-1);
    
    cout<<"grass mean: \n"<<mean_g<<endl<<endl;
    cout<<"straw mean: \n"<<mean_s<<endl<<endl;
    
    // use DECOMP_SVD for accuracy
    invert(cov_g, inv_cov_g, DECOMP_SVD), invert(cov_s, inv_cov_s, DECOMP_SVD);
    
    // Calculate Mahalanobis distance from 24 unknown image features to grass mean and straw mean, and then compare the distance one by one.
    printf("compare Mahalanobis distance for each unknown sample:\n (distance to grass mean, distance to straw mean) = (a, b)\n\n");
    for (int i=0; i<unknownFileCount; i++) {
        
        Mat sample = (unknown_MatT(Range(i,i+1), Range::all()));
        
        //printf("unknown_%02d:\n", (i+1));
        //cout<< sample << endl<< endl;
        //printf("Mahalanobis distance between unknown_%02d and grass mean: %f\n", (i+1), Mahalanobis(sample, mean_g, inv_cov_g));
        //printf("Mahalanobis distance between unknown_%02d and straw mean: %f\n", (i+1), Mahalanobis(sample, mean_s, inv_cov_s));

        if (Mahalanobis(sample, mean_g, inv_cov_g) > Mahalanobis(sample, mean_s, inv_cov_s)) {
            printf("unknown_%02d.raw is %s, (%.2f > %.2f)\n", (i+1), "straw", Mahalanobis(sample, mean_g, inv_cov_g), Mahalanobis(sample, mean_s, inv_cov_s));
        }
        else {
            printf("unknown_%02d.raw is %s, (%.2f < %.2f)\n", (i+1), "grass", Mahalanobis(sample, mean_g, inv_cov_g), Mahalanobis(sample, mean_s, inv_cov_s));
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif