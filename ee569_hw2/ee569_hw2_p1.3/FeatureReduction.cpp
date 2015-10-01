//
//  FeatureReduction.cpp
//  HW2_P1.3
//
//  This code applys PCA and LDA to reduce the number of features,
//  based on unadjusted feature array obtained from 1.1.
//
//  Input: [unknown image's features array]....................... 2-D array obtained from 1.1
//         [class A (grass) features array]....................... 2-D array obtained from 1.1
//         [class B (straw) features array]....................... 2-D array obtained from 1.1
//
//  Algorithm of PCA:
//  step 1. Convert the unknown samples' feature array into Matrices.
//  step 2. Apply PCA for unlabeled group, reduce the feature into single one.
//
//  Verify PCA: (use diagonal covariance matrix of unknown samples)
//  step 1. Adjust the scale of each feature into [0,1] for unknown sample's feature array.
//  step 2. Get diagonal covariance matrix, and compare with eigenvector
//
//  Algorithm of LDA:
//  step 1. Adjust the scale of each feature into [0,1], for both labeled arrays.
//  step 2. Get diagonal covariance matrix, and compare both diag. covariance matrice.
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#ifndef FEATURE_REDUCTION
#define FEATURE_REDUCTION

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
    if (argc < 4)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [class A (grass) mean-features] [class B (straw) mean-features] [unknown images' features]" << endl;
        return 0;
    }
    
    // Read all the features by fread(ptr,size,count,fp)
    double unknownFeats[NumOfFeats][unknownFileCount], grassFeats[NumOfFeats][labeledFileCount], strawFeats[NumOfFeats][labeledFileCount];
    if (!(file=fopen(argv[1],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(unknownFeats, sizeof(double), NumOfFeats*unknownFileCount, file);
    fclose(file);

    if (!(file=fopen(argv[2],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(grassFeats, sizeof(double), NumOfFeats*labeledFileCount, file);
    fclose(file);
    
    if (!(file=fopen(argv[3],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(strawFeats, sizeof(double), NumOfFeats*labeledFileCount, file);
    fclose(file);
   
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Part 1 - PCA
    printf("*****************PCA*****************\n");
    
    // Step 1. Convert these features arrays into matrices
    Mat unknown_Mat = Mat(NumOfFeats, unknownFileCount, CV_64FC1, unknownFeats), unknown_MatT;
    transpose(unknown_Mat, unknown_MatT);
    
    // Debug:
    // cout<<"unknown samples: \n"<<unknown_MatT<<endl<<endl;

    // Step 2. Perform PCA analysis
    PCA pca_unknown(unknown_MatT, Mat(), CV_PCA_DATA_AS_ROW, 1);
    
    Mat pca_unknown_mean = pca_unknown.mean.clone();
    Mat pca_unknown_eigenvalues = pca_unknown.eigenvalues.clone();
    Mat pca_unknown_eigenvectors_g = pca_unknown.eigenvectors.clone();
    
    cout<<"PCA mean: \n"<<pca_unknown_mean<<endl<<endl;
    cout<<"PCA eigen-value: \n"<<pca_unknown_eigenvalues<<endl<<endl;
    cout<<"PCA eigen-vector: \n"<<pca_unknown_eigenvectors_g<<endl<<endl;
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Part 2 - Verify PCA results
    printf("*****************Verify PCA*****************\n ");
    
    // Step 1. Adjust the scale of 25 features
    double unknownFeatsRescale[NumOfFeats][unknownFileCount], grassFeatsRescale[NumOfFeats][labeledFileCount], strawFeatsRescale[NumOfFeats][labeledFileCount];
    double FeatMax[NumOfFeats], FeatMin[NumOfFeats];
    
    for (int ft = 0; ft < NumOfFeats; ft++) {
        FeatMax[ft] = 0;
        FeatMin[ft] = 1000000;
        for (int fc = 0; fc < labeledFileCount; fc++) {
            if ( FeatMax[ft] < max(grassFeats[ft][fc], strawFeats[ft][fc])) {
                FeatMax[ft] = max(grassFeats[ft][fc], strawFeats[ft][fc]);
            }
            if ( FeatMin[ft] > min(grassFeats[ft][fc], strawFeats[ft][fc])) {
                FeatMin[ft] = max(grassFeats[ft][fc], strawFeats[ft][fc]);
            }
        }
        for (int fc = 0; fc < unknownFileCount; fc++) {
            if ( FeatMax[ft] < unknownFeats[ft][fc]) {
                FeatMax[ft] = unknownFeats[ft][fc];
            }
            if ( FeatMin[ft] > unknownFeats[ft][fc]) {
                FeatMin[ft] = unknownFeats[ft][fc];
            }
        }
    }
    
    for (int ft=0; ft< NumOfFeats; ft++) {
        for (int fc = 0; fc < labeledFileCount; fc++) {
            grassFeatsRescale[ft][fc] = (grassFeats[ft][fc]-FeatMin[ft])/FeatMax[ft];
            strawFeatsRescale[ft][fc] = (strawFeats[ft][fc]-FeatMin[ft])/FeatMax[ft];
        }
        for (int fc = 0; fc < unknownFileCount; fc++) {
            unknownFeatsRescale[ft][fc] = (unknownFeats[ft][fc]-FeatMin[ft])/FeatMax[ft];
        }
    }
    // Convert the rescaled array into matrix
    Mat unknown_MatR = Mat(NumOfFeats, unknownFileCount, CV_64FC1, unknownFeatsRescale), unknown_MatRT;
    transpose(unknown_MatR, unknown_MatRT);
   
    // Step 2. Calculate the covariance matrix of unknown samples
    Mat cov_ukR, inv_cov_ukR, mean_ukR;
    cv::calcCovarMatrix(unknown_MatRT, cov_ukR, mean_ukR, CV_COVAR_NORMAL+CV_COVAR_ROWS);
    
    Mat diag_cov_uk = cov_ukR.diag(0), diag_cov_ukT;
    transpose(diag_cov_uk, diag_cov_ukT);
    
    cout<<"\nunknown samples' diagnonal covariance matrix: \n"<<diag_cov_ukT<<endl;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Part 3 - LDA
    printf("\n*****************LDA*****************\n");
    
    // Step 1. Convert the rescaled arrays into matrices
    Mat grass_MatR = Mat(NumOfFeats, labeledFileCount, CV_64FC1, grassFeatsRescale), grass_MatRT;
    Mat straw_MatR = Mat(NumOfFeats, labeledFileCount, CV_64FC1, strawFeatsRescale), straw_MatRT;

    transpose(grass_MatR, grass_MatRT);
    transpose(straw_MatR, straw_MatRT);
    
    // Step 2. Calculate the "inter-feature" variance in both labeled groups (diagonal covariance matrix)
    Mat cov_g, inv_cov_g, mean_g;
    cv::calcCovarMatrix(grass_MatRT, cov_g, mean_g, CV_COVAR_NORMAL+CV_COVAR_ROWS);
    //cov_g = cov_g/(grass_MatRT.rows-1);
    
    Mat cov_s, inv_cov_s, mean_s;
    cv::calcCovarMatrix(straw_MatRT, cov_s, mean_s, CV_COVAR_NORMAL+CV_COVAR_ROWS);
    //cov_s = cov_s/(straw_MatRT.rows-1);
    
    // Debug:
    // cout<<"labeled grass mean: \n"<<mean_g<<endl<<endl;
    // cout<<"labeled straw mean: \n"<<mean_s<<endl<<endl;
    // cout<<"25x25 grass cov: \n"<<cov_g<<endl<<endl;
    // cout<<"25x25 straw cov: \n"<<cov_s<<endl<<endl;
    
     Mat diag_cov_g = cov_g.diag(0), diag_cov_s = cov_s.diag(0), diag_cov_gT, diag_cov_sT;
     transpose(diag_cov_g, diag_cov_gT), transpose(diag_cov_s, diag_cov_sT);
    
     cout<<"\ngrass diagnonal covariance matrix: \n"<<diag_cov_gT<<endl;
     cout<<"\nstraw diagonal covariance matrix: \n"<<diag_cov_sT<<endl;
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif