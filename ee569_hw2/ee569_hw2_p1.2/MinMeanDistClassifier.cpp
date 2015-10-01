//
//  MinMeanDistClassifier.cpp
//  HW2_P1.2
//
//  OpenCV is required. From feature array obtained from previous step (1.1), we can apply a "min distance classifier"
//  (We already know each array's dimension, but it's not yet necessary to adjust these feature's scale.)
//
//  Input: [unknown image features array]......................... 2-D array obtained from 1.1
//         [class A (grass) mean-features array].................. 2-D array obtained from 1.1
//         [class B (straw) mean-features array].................. 2-D array obtained from 1.1
//
//  Algorithm:
//  step 1. For convenience, we read the array(step 0.) and convert the feature array into matrix.
//  step 2. Apply calcCovarMatrix() on labeled samples, to get covariance matrix and class-mean, which are needed for next step.
//  step 3. Apply Mahalanobis() on unknown samples, and compare the m.distance to both group for each unknown sample.
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
    int NumOfFeats = 25, unknownFileCount = 24, labeledFileCount = 36;         // Define the variables
    
    // argv[1] = "/Users/YJLee/Desktop/unknown_all.array";
    // argv[2] = "/Users/YJLee/Desktop/grass_all.array";
    // argv[3] = "/Users/YJLee/Desktop/straw_all.array";
    
    // Check for proper syntax
    if (argc < 4){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [class A (grass) mean-features] [class B (straw) mean-features] [unknown images' features]" << endl;
        return 0;
    }
    else {
        printf("Number of Features: %d\nNumber of unknown samples: %d\nNumber of labeled samples in each class: %d\n\n", NumOfFeats, unknownFileCount, labeledFileCount);
    }
    
    // Step 0. Read all the features by fread(ptr,size,count,fp)
    double unknownFeatures[NumOfFeats][unknownFileCount], grassFeatures[NumOfFeats][labeledFileCount], strawFeatures[NumOfFeats][labeledFileCount];
    if (!(file=fopen(argv[1],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(unknownFeatures, sizeof(double), NumOfFeats*unknownFileCount, file);
    fclose(file);

    if (!(file=fopen(argv[2],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(grassFeatures, sizeof(double), NumOfFeats*labeledFileCount, file);
    fclose(file);
    
    if (!(file=fopen(argv[3],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(strawFeatures, sizeof(double), NumOfFeats*labeledFileCount, file);
    fclose(file);
   
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Step 1. Convert these features arrays into matrices
    Mat unknown_Mat = Mat(NumOfFeats, unknownFileCount, CV_64FC1, unknownFeatures), unknown_MatT;
    Mat grass_Mat = Mat(NumOfFeats, labeledFileCount, CV_64FC1, grassFeatures), grass_MatT;
    Mat straw_Mat = Mat(NumOfFeats, labeledFileCount, CV_64FC1, strawFeatures), straw_MatT;
    
    transpose(unknown_Mat, unknown_MatT);
    transpose(grass_Mat, grass_MatT);
    transpose(straw_Mat, straw_MatT);
    
    // Debug:
    // cout<<"unknown samples: \n"<<unknown_MatT<<endl<<endl;
    
    // Step 2. Get class covariance matrix and mean for both labeled groups
    Mat cov_g, inv_cov_g, mean_g;
    cv::calcCovarMatrix(grass_MatT, cov_g, mean_g, CV_COVAR_NORMAL+CV_COVAR_ROWS);
    cov_g = cov_g/(grass_MatT.rows-1);

    Mat cov_s, inv_cov_s, mean_s;
    cv::calcCovarMatrix(straw_MatT, cov_s, mean_s, CV_COVAR_NORMAL+CV_COVAR_ROWS);
    cov_s = cov_s/(straw_MatT.rows-1);

    // Debug:
    // cout<<"25 grass mean: \n"<<mean_g<<endl<<endl;
    // cout<<"25 straw mean: \n"<<mean_s<<endl<<endl;
    // cout<<"25x25 grass cov: \n"<<cov_g<<endl<<endl;
    // cout<<"25x25 straw cov: \n"<<cov_s<<endl<<endl;
    
    // Step 3. Calculate M-distance from 24 unknown image features to grass mean and straw mean, and then compare the distance one by one.
    printf("compare M-distance for each unknown sample:\n (distance to grass mean, distance to straw mean) = (a, b)\n\n");
    
    // Get inverse covariance matrix (this is needed to calculate M-distance) with DECOMP_SVD for accuracy.
    // Further detail: http://goo.gl/pmDJgB
    
    invert(cov_g, inv_cov_g, DECOMP_SVD), invert(cov_s, inv_cov_s, DECOMP_SVD);
    
    // For each unknown sample point, calculate and compare the M-distance
    for (int i=0; i<unknownFileCount; i++) {
        
        // Calculate
        Mat sample = (unknown_MatT(Range(i,i+1), Range::all()));
        double dist_to_grass = Mahalanobis(sample, mean_g, inv_cov_g);
        double dist_to_straw = Mahalanobis(sample, mean_s, inv_cov_s);
        
        // Compare
        if ( dist_to_grass > dist_to_straw) {
            printf("unknown_%02d.raw is %s, (%.2f > %.2f)\n", (i+1), "straw", dist_to_grass, dist_to_straw);
        }
        else {
            printf("unknown_%02d.raw is %s, (%.2f < %.2f)\n", (i+1), "grass", dist_to_grass, dist_to_straw);
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif