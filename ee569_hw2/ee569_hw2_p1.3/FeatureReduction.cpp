//
//  FeatureReduction.cpp
//  HW2_P1.3
//
//  This code applys PCA and LDA to reduce the number of features, and using the new feature for classification.
//  based on unadjusted feature array obtained from 1.1.
//
//  Input: [unknown image samples]....................... 2-D 25x24 array obtained from 1.1
//         [class A (grass) samples]..................... 2-D 25x36 array obtained from 1.1
//         [class B (straw) samples]..................... 2-D 25x36 array obtained from 1.1
//
//  Steps:
//  0. Rescale and Convert the unknown sample's feature arrays into Matrices
//
//  (PCA part)
//  1. Apply PCA for unlabeled samples, get mean, eigenvector and eigenvalue.
//  2. Project all samples into PCA eigenvector, then reduce the feature matrices into 1-D matrices.
//  3. Apply Mahalanobis binary classifier to label data
//
//  (LDA part)
//  4. "Relabel" labeled samples, and then apply LDA to get eigenvector.
//  5. Reduce all sample matrices into 1-D matrices.
//  6. Use LDA results to classify labeled data
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
    double unknownFeatsRescale[NumOfFeats][unknownFileCount], grassFeatsRescale[NumOfFeats][labeledFileCount], strawFeatsRescale[NumOfFeats][labeledFileCount];
    
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
    // Step 0. Adjust the scale of 25 features

    double FeatMax[NumOfFeats], FeatMin[NumOfFeats];
    for (int ft = 0; ft < NumOfFeats; ft++) {
        FeatMax[ft] = 0;
        FeatMin[ft] = 1000000;
        for (int fc = 0; fc < labeledFileCount; fc++) {
            if ( FeatMax[ft] < max(grassFeats[ft][fc], strawFeats[ft][fc])) {
                FeatMax[ft] = max(grassFeats[ft][fc], strawFeats[ft][fc]);
            }
            if ( FeatMin[ft] > min(grassFeats[ft][fc], strawFeats[ft][fc])) {
                FeatMin[ft] = min(grassFeats[ft][fc], strawFeats[ft][fc]);
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
    
    // Make these array into Matrices
    Mat unknownMatRescaled = Mat(NumOfFeats, unknownFileCount, CV_64FC1, unknownFeatsRescale);   // 24 columns -> filecount, 25 rows -> features
    Mat grassMatRescaled = Mat(NumOfFeats, labeledFileCount, CV_64FC1, grassFeatsRescale);
    Mat strawMatRescaled = Mat(NumOfFeats, labeledFileCount, CV_64FC1, strawFeatsRescale);
    
    Mat unknownMatRescaledTranspose, grassMatRescaledTranspose, strawMatRescaledTranspose;    // 25 columns -> features, 24 rows -> filecount
    transpose(unknownMatRescaled, unknownMatRescaledTranspose);
    transpose(grassMatRescaled, grassMatRescaledTranspose);
    transpose(strawMatRescaled, strawMatRescaledTranspose);
    
    // Step 1. Perform PCA analysis
    
    // Apply PCA and get eigenvectors
    PCA pca_unknown(unknownMatRescaledTranspose, Mat(), CV_PCA_DATA_AS_ROW, 1);     
    
    Mat pca_unknown_mean = pca_unknown.mean.clone();
    Mat pca_unknown_eigenvectors = pca_unknown.eigenvectors.clone();
    Mat pca_unknown_eigenvalues = pca_unknown.eigenvalues.clone();

    printf("*****************PCA*****************\n");
    cout<<"PCA mean: \n"<<pca_unknown_mean<<endl<<endl;
    cout<<"PCA eigen-vector: \n"<<pca_unknown_eigenvectors<<endl<<endl;
    cout<<"PCA eigen-value: \n"<<pca_unknown_eigenvalues<<endl<<endl;
    
    // Step 2. Project the samples on eigenvectors, reduce the samples into 1-D array
    double grass1DPCA[labeledFileCount], straw1DPCA[labeledFileCount], unknown1DPCA[unknownFileCount];
    
    for (int i=0; i<labeledFileCount; i++) {
        Mat sample_grass = (grassMatRescaledTranspose(Range(i,i+1), Range::all()));
        Mat sample_straw = (strawMatRescaledTranspose(Range(i,i+1), Range::all()));
        grass1DPCA[i] = sample_grass.dot(pca_unknown_eigenvectors);
        straw1DPCA[i] = sample_straw.dot(pca_unknown_eigenvectors);
    }
    for (int i=0; i<labeledFileCount; i++) {
        Mat sample_unknown = (grassMatRescaledTranspose(Range(i,i+1), Range::all()));
        unknown1DPCA[i] = sample_unknown.dot(pca_unknown_eigenvectors);
    }
    // Convert to Matrices
    Mat unknown1DPCAMat = Mat(1, unknownFileCount, CV_64FC1, unknown1DPCA);
    Mat grass1DPCAMat = Mat(1, labeledFileCount, CV_64FC1, grass1DPCA);
    Mat straw1DPCAMat = Mat(1, labeledFileCount, CV_64FC1, straw1DPCA);
    
    Mat covUk1DPCA, inv_covUk1DPCA, meanUk1DPCA;
    cv::calcCovarMatrix(unknown1DPCAMat, covUk1DPCA, meanUk1DPCA, CV_COVAR_NORMAL+CV_COVAR_COLS);

    Mat covGrass1DPCA, inv_covGrass1DPCA, meanGrass1DPCA;
    cv::calcCovarMatrix(grass1DPCAMat, covGrass1DPCA, meanGrass1DPCA, CV_COVAR_NORMAL+CV_COVAR_COLS);
    
    Mat covStraw1DPCA, inv_covStraw1DPCA, meanStraw1DPCA;
    cv::calcCovarMatrix(straw1DPCAMat, covStraw1DPCA, meanStraw1DPCA, CV_COVAR_NORMAL+CV_COVAR_COLS);
    
    invert(covUk1DPCA, inv_covUk1DPCA, DECOMP_SVD);
    invert(covGrass1DPCA, inv_covGrass1DPCA, DECOMP_SVD);
    invert(covStraw1DPCA, inv_covStraw1DPCA, DECOMP_SVD);
    printf("\n*****************Using PCA chosen features to classify labeled samples*****************\n");
    
    // Step 3. Classify labeled samples
    
    // Calculate the Mahalanobis Distance and compare
    for (int i=0; i<unknownFileCount; i++) {
        // Calculate
        Mat sample_unknown = (unknown1DPCAMat.col(i));
        // cout << sample_unknown << endl << endl << meanGrass1DPCA << endl << endl << inv_covUk1DPCA << endl;
        double dist_to_grass = Mahalanobis(sample_unknown, meanGrass1DPCA, inv_covGrass1DPCA);
        double dist_to_straw = Mahalanobis(sample_unknown, meanStraw1DPCA, inv_covStraw1DPCA);
        
        // Compare
        if ( dist_to_grass > dist_to_straw) {
            printf("unknown_%02d.raw is %s, (%.2f > %.2f)\n", (i+1), "straw", dist_to_grass, dist_to_straw);
        }
        else {
            printf("unknown_%02d.raw is %s, (%.2f < %.2f)\n", (i+1), "grass", dist_to_grass, dist_to_straw);
        }
    }

    // Step 4. Perform LDA analysis
    
    // Combine two labeled sample-sets and convert into Matrix
    double labeledFeatsRescale[NumOfFeats][labeledFileCount*2];
    for (int fc=0; fc<labeledFileCount; fc++) {
        for (int ft=0; ft<NumOfFeats; ft++) {
            labeledFeatsRescale[ft][fc] = grassFeatsRescale[ft][fc];
            labeledFeatsRescale[ft][fc+labeledFileCount] = strawFeatsRescale[ft][fc];
        }
    }
    Mat labeledMatRescaled = Mat(NumOfFeats, labeledFileCount*2, CV_64FC1, labeledFeatsRescale), labeledMatRescaledTranspose;
    transpose(labeledMatRescaled, labeledMatRescaledTranspose);
    
    // Label these samples
    vector<int>labels;
    for(int i=0;i<labeledMatRescaledTranspose.rows;i++)
    {
        if(i<labeledMatRescaledTranspose.rows/2)
        {
            labels.push_back(0);              // Grass
        }
        else
        {
            labels.push_back(1);              // Straw
        }
    }
    
    // Apply LDA and get eigenvectors
    LDA lda=LDA(labeledMatRescaledTranspose,labels);
    Mat lda_labeled_eigenvectors=lda.eigenvectors().clone(), lda_labeled_eigenvectorsTranspose;
    Mat lda_labeled_eigenvalue = lda.eigenvalues().clone();
    transpose(lda_labeled_eigenvectors, lda_labeled_eigenvectorsTranspose);
    
    printf("\n*****************LDA*****************\n");
    
    cout<<"LDA eigen-vector: \n"<< lda_labeled_eigenvectorsTranspose << endl << endl;
    cout<<"LDA eigen-value: \n" << lda_labeled_eigenvalue << endl << endl;
    
    // Step 5. Project the data on eigenvectors, reduce the samples into 1-D array
    double grass1DLDA[labeledFileCount], straw1DLDA[labeledFileCount], unknown1DLDA[unknownFileCount];
    
    for (int i=0; i<labeledFileCount; i++) {
        Mat sample_grass = (grassMatRescaledTranspose(Range(i,i+1), Range::all()));
        Mat sample_straw = (strawMatRescaledTranspose(Range(i,i+1), Range::all()));
        grass1DLDA[i] = sample_grass.dot(lda_labeled_eigenvectorsTranspose);
        straw1DLDA[i] = sample_straw.dot(lda_labeled_eigenvectorsTranspose);
    }
    for (int i=0; i<labeledFileCount; i++) {
        Mat sample_unknown = (grassMatRescaledTranspose(Range(i,i+1), Range::all()));
        unknown1DLDA[i] = sample_unknown.dot(lda_labeled_eigenvectorsTranspose);
    }
    
    // Convert to Matrices
    Mat unknown1DLDAMat = Mat(1, unknownFileCount, CV_64FC1, unknown1DLDA);
    Mat grass1DLDAMat = Mat(1, labeledFileCount, CV_64FC1, grass1DLDA);
    Mat straw1DLDAMat = Mat(1, labeledFileCount, CV_64FC1, straw1DLDA);
    
    Mat covUk1DLDA, inv_covUk1DLDA, meanUk1DLDA;
    cv::calcCovarMatrix(unknown1DLDAMat, covUk1DLDA, meanUk1DLDA, CV_COVAR_NORMAL+CV_COVAR_COLS);
    
    Mat covGrass1DLDA, inv_covGrass1DLDA, meanGrass1DLDA;
    cv::calcCovarMatrix(grass1DLDAMat, covGrass1DLDA, meanGrass1DLDA, CV_COVAR_NORMAL+CV_COVAR_COLS);
    
    Mat covStraw1DLDA, inv_covStraw1DLDA, meanStraw1DLDA;
    cv::calcCovarMatrix(straw1DLDAMat, covStraw1DLDA, meanStraw1DLDA, CV_COVAR_NORMAL+CV_COVAR_COLS);
    
    invert(covUk1DLDA, inv_covUk1DLDA, DECOMP_SVD);
    invert(covGrass1DLDA, inv_covGrass1DLDA, DECOMP_SVD);
    invert(covStraw1DLDA, inv_covStraw1DLDA, DECOMP_SVD);
    printf("\n*****************Using LDA chosen features to classify labeled samples*****************\n");

    // Step 6. Using LDA results to classify labeled and unknown samples
    
    // Calculate the Mahalanobis Distance and compare
    for (int i=0; i<unknownFileCount; i++) {
        // Calculate
        Mat sample_unknown = (unknown1DLDAMat.col(i));
        // cout << sample_unknown << endl << endl << meanGrass1DPCA << endl << endl << inv_covUk1DPCA << endl;
        double dist_to_grass = Mahalanobis(sample_unknown, meanGrass1DLDA, inv_covGrass1DLDA);
        double dist_to_straw = Mahalanobis(sample_unknown, meanStraw1DLDA, inv_covStraw1DLDA);
        
        // Compare
        if ( dist_to_grass < dist_to_straw) {
            printf("unknown_%02d.raw is %s, (%.2f < %.2f)\n", (i+1), "grass", dist_to_grass, dist_to_straw);
        }
        else {
            printf("unknown_%02d.raw is %s, (%.2f > %.2f)\n", (i+1), "straw", dist_to_grass, dist_to_straw);
        }
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif