//
//  FeatureReduction_ver2.cpp
//  HW2_P1.b.1
//
//  Apply PCA to reduce the number of features into 3,
//        LDA to reduce the number of features into 1, as reference
//
//  And use the new feature for classification, based on unadjusted feature array obtained from 1.b.0.
//
//
//  Input: [Test samples]........................................ 2-D 25x48 array obtained from 1.b.0
//         [Positive training image samples]..................... 2-D 25x36 array obtained from 1.b.0
//         [Negative training image samples]..................... 2-D 25x36 array obtained from 1.b.0
//
//
//  Steps:
//  0. Rescale and Convert the Test sample's feature arrays into Matrices
//
//  (PCA part)
//  1. Apply PCA, get mean, eigenvector and eigenvalue.
//  2. Project all samples into PCA eigenvector, then reduce the feature matrices into 1-D matrices.
//  3. Apply Mahalanobis binary classifier to label data
//
//  (As reference, LDA part reduce into 1 feature)
//  4. "Relabel" Train samples, and then apply LDA to get eigenvector.
//  5. Reduce all sample matrices into 1-D matrices.
//  6. Use LDA results to classify Train data
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

#ifndef FEATURE_REDUCTION_V2
#define FEATURE_REDUCTION_V2

using namespace cv;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE * file;
    int NumOfFeats = 25, TestFileCount = 48, TrainFileCount = 36;         // Define the variables
    int NumOfReducedFeats = 3;
    double TestFeatsRescale[NumOfFeats][TestFileCount], PositiveFeatsRescale[NumOfFeats][TrainFileCount], NegativeFeatsRescale[NumOfFeats][TrainFileCount];
    
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
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Step 0. Adjust the scale of 25 features
    double FeatMax[NumOfFeats], FeatMin[NumOfFeats];
    for (int ft = 0; ft < NumOfFeats; ft++) {
        FeatMax[ft] = 0;
        FeatMin[ft] = 1000000;
        for (int fc = 0; fc < TrainFileCount; fc++) {
            if ( FeatMax[ft] < max(PositiveFeats[ft][fc], NegativeFeats[ft][fc])) {
                FeatMax[ft] = max(PositiveFeats[ft][fc], NegativeFeats[ft][fc]);
            }
            if ( FeatMin[ft] > min(PositiveFeats[ft][fc], NegativeFeats[ft][fc])) {
                FeatMin[ft] = min(PositiveFeats[ft][fc], NegativeFeats[ft][fc]);
            }
        }
     }
    
    for (int ft=0; ft< NumOfFeats; ft++) {
        for (int fc = 0; fc < TrainFileCount; fc++) {
            PositiveFeatsRescale[ft][fc] = (PositiveFeats[ft][fc]-FeatMin[ft])/FeatMax[ft];
            NegativeFeatsRescale[ft][fc] = (NegativeFeats[ft][fc]-FeatMin[ft])/FeatMax[ft];
        }
        for (int fc = 0; fc < TestFileCount; fc++) {
            TestFeatsRescale[ft][fc] = (TestFeats[ft][fc]-FeatMin[ft])/FeatMax[ft];
        }
    }
    
    // Combine Training Samples into 25x72 array
    double TrainFeatsRescale[NumOfFeats][TrainFileCount*2];
    for (int fc=0; fc < TrainFileCount; fc++) {
        for (int ft =0; ft < NumOfFeats; ft++) {
            TrainFeatsRescale[ft][fc] = PositiveFeatsRescale[ft][fc];
            TrainFeatsRescale[ft][fc+TrainFileCount] = NegativeFeatsRescale[ft][fc];
        }
    }
    
    // Make these array into Matrices
    Mat TestMatRescaled = Mat(NumOfFeats, TestFileCount, CV_64FC1, TestFeatsRescale);   // columns -> filecount, rows -> features
    Mat TrainMatRescaled = Mat(NumOfFeats, (TrainFileCount*2), CV_64FC1, TrainFeatsRescale);
    Mat PositiveMatRescaled = Mat(NumOfFeats, TrainFileCount, CV_64FC1, PositiveFeatsRescale);
    Mat NegativeMatRescaled = Mat(NumOfFeats, TrainFileCount, CV_64FC1, NegativeFeatsRescale);
    
    Mat TestMatRescaledTranspose, TrainMatRescaledTranspose, PositiveMatRescaledTranspose, NegativeMatRescaledTranspose;    // columns -> features, rows -> filecount
    transpose(TestMatRescaled, TestMatRescaledTranspose);
    transpose(TrainMatRescaled, TrainMatRescaledTranspose);
    transpose(PositiveMatRescaled, PositiveMatRescaledTranspose);
    transpose(NegativeMatRescaled, NegativeMatRescaledTranspose);
    
    // Step 1. Perform PCA analysis
    
    // Apply PCA and get eigenvectors
    PCA pca_Train(TrainMatRescaledTranspose, Mat(), CV_PCA_DATA_AS_ROW, NumOfReducedFeats);
    
    Mat pca_Train_mean = pca_Train.mean.clone();
    Mat pca_Train_eigenvectors = pca_Train.eigenvectors.clone();
    Mat pca_Train_eigenvalues = pca_Train.eigenvalues.clone();

    printf("*****************PCA*****************\n");
    cout<<"PCA mean: \n"<<pca_Train_mean<<endl<<endl;
    cout<<"PCA eigen-vector: \n"<<pca_Train_eigenvectors<<endl<<endl;
    cout<<"PCA eigen-value: \n"<<pca_Train_eigenvalues<<endl<<endl;
    
    // Step 2. Project the samples on eigenvectors, reduce the samples into an one dimentional array including 3 features
    double Positive1DPCA[NumOfReducedFeats][TrainFileCount], Negative1DPCA[NumOfReducedFeats][TrainFileCount];
    double Train1DPCA[NumOfReducedFeats][TrainFileCount*2], Test1DPCA[NumOfReducedFeats][TestFileCount];
    
    for (int ft=0; ft<NumOfReducedFeats; ft ++) {
        for (int i=0; i<TrainFileCount; i++) {
            Mat sample_Positive = (PositiveMatRescaledTranspose(Range(i,i+1), Range::all()));
            Mat sample_Negative = (NegativeMatRescaledTranspose(Range(i,i+1), Range::all()));
            Positive1DPCA[ft][i] = sample_Positive.dot(pca_Train_eigenvectors.row(ft));
            Negative1DPCA[ft][i] = sample_Negative.dot(pca_Train_eigenvectors.row(ft));
        }
        for (int i=0; i<TestFileCount; i++) {
            Mat sample_Test = (TestMatRescaledTranspose(Range(i,i+1), Range::all()));
            Test1DPCA[ft][i] = sample_Test.dot(pca_Train_eigenvectors.row(ft));
        }
        for (int i=0; i<TrainFileCount*2; i++) {
            Mat sample_Train = (TrainMatRescaledTranspose(Range(i,i+1), Range::all()));
            Train1DPCA[ft][i] = sample_Train.dot(pca_Train_eigenvectors.row(ft));
        }
    }
    // Convert to Matrices
    Mat Test1DPCAMat = Mat(NumOfReducedFeats, TestFileCount, CV_64FC1, Test1DPCA);
    Mat Train1DPCAMat = Mat(NumOfReducedFeats, TrainFileCount*2, CV_64FC1, Train1DPCA);
    Mat Positive1DPCAMat = Mat(NumOfReducedFeats, TrainFileCount, CV_64FC1, Positive1DPCA);
    Mat Negative1DPCAMat = Mat(NumOfReducedFeats, TrainFileCount, CV_64FC1, Negative1DPCA);
    
    Mat covUk1DPCA, inv_covUk1DPCA, meanUk1DPCA;
    cv::calcCovarMatrix(Test1DPCAMat, covUk1DPCA, meanUk1DPCA, CV_COVAR_NORMAL+CV_COVAR_COLS);

    Mat covPositive1DPCA, inv_covPositive1DPCA, meanPositive1DPCA;
    cv::calcCovarMatrix(Positive1DPCAMat, covPositive1DPCA, meanPositive1DPCA, CV_COVAR_NORMAL+CV_COVAR_COLS);
    
    Mat covNegative1DPCA, inv_covNegative1DPCA, meanNegative1DPCA;
    cv::calcCovarMatrix(Negative1DPCAMat, covNegative1DPCA, meanNegative1DPCA, CV_COVAR_NORMAL+CV_COVAR_COLS);
    
    invert(covUk1DPCA, inv_covUk1DPCA, DECOMP_SVD);
    invert(covPositive1DPCA, inv_covPositive1DPCA, DECOMP_SVD);
    invert(covNegative1DPCA, inv_covNegative1DPCA, DECOMP_SVD);
    
    // Step 3. Classify samples
    
    // Calculate the Mahalanobis Distance and compare
    printf("\n*****************Using PCA chosen features to classify Test/Training samples*****************\n");
    bool PCA_classify_test[TestFileCount], PCA_classify_train[TrainFileCount*2];
    int count_test=0, count_train=0;

    for (int i=0; i<TestFileCount; i++) {
        // Calculate
        Mat sample_Test = (Test1DPCAMat.col(i));
        // cout << sample_Test << endl << endl << meanPositive1DPCA << endl << endl << inv_covUk1DPCA << endl;
        double dist_Test_to_Positive = Mahalanobis(sample_Test, meanPositive1DPCA, inv_covPositive1DPCA);
        double dist_Test_to_Negative = Mahalanobis(sample_Test, meanNegative1DPCA, inv_covNegative1DPCA);
        
        // Compare
        if ( dist_Test_to_Positive < dist_Test_to_Negative) {
            PCA_classify_test[i]=1;
            count_test++;
        }
        else {
            PCA_classify_test[i]=0;
        }
    }
    for (int i=0; i<TrainFileCount*2; i++) {
        Mat sample_Train = (Train1DPCAMat.col(i));

        double dist_Train_to_Positive = Mahalanobis(sample_Train, meanPositive1DPCA, inv_covPositive1DPCA);
        double dist_Train_to_Negative = Mahalanobis(sample_Train, meanNegative1DPCA, inv_covNegative1DPCA);
        
        // Compare
        if ( dist_Train_to_Positive < dist_Train_to_Negative) {
            PCA_classify_train[i]=1;
            count_train++;
        }
        else {
            //printf("%d is negative\n", (i+1));
            PCA_classify_train[i]=0;
        }
    }
    
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
    // Step 4. Perform LDA analysis
    
    // Label these samples
    vector<int>labels;
    for (int fc=0 ; fc < TrainMatRescaledTranspose.rows; fc++) {
        if(fc<TrainMatRescaledTranspose.rows/2) {
            labels.push_back(0);              // Positive
        }
        else {
            labels.push_back(1);              // Negative
        }
    }
    
    // Apply LDA and get eigenvectors
    LDA lda=LDA(TrainMatRescaledTranspose,labels);
    Mat lda_Train_eigenvectors=lda.eigenvectors().clone(), lda_Train_eigenvectorsTranspose;
    Mat lda_Train_eigenvalue = lda.eigenvalues().clone();
    transpose(lda_Train_eigenvectors, lda_Train_eigenvectorsTranspose);
    
    printf("\n\n\n*****************LDA*****************\n");
    
    cout<<"LDA eigen-vector: \n"<< lda_Train_eigenvectorsTranspose << endl << endl;
    cout<<"LDA eigen-value: \n" << lda_Train_eigenvalue << endl << endl;
    
    // Step 5. Project the data on eigenvectors, reduce the samples into 1-D array
    double Positive1DLDA[TrainFileCount], Negative1DLDA[TrainFileCount], Test1DLDA[TestFileCount];
    
    for (int i=0; i<TrainFileCount; i++) {
        Mat sample_Positive = (PositiveMatRescaledTranspose(Range(i,i+1), Range::all()));
        Mat sample_Negative = (NegativeMatRescaledTranspose(Range(i,i+1), Range::all()));
        Positive1DLDA[i] = sample_Positive.dot(lda_Train_eigenvectorsTranspose);
        Negative1DLDA[i] = sample_Negative.dot(lda_Train_eigenvectorsTranspose);
    }
    for (int i=0; i<TestFileCount; i++) {
        Mat sample_Test = (TestMatRescaledTranspose(Range(i,i+1), Range::all()));
        Test1DLDA[i] = sample_Test.dot(lda_Train_eigenvectorsTranspose);
    }
    
    // Convert to Matrices
    Mat Test1DLDAMat = Mat(1, TestFileCount, CV_64FC1, Test1DLDA);
    Mat Positive1DLDAMat = Mat(1, TrainFileCount, CV_64FC1, Positive1DLDA);
    Mat Negative1DLDAMat = Mat(1, TrainFileCount, CV_64FC1, Negative1DLDA);
    
    Mat covUk1DLDA, inv_covUk1DLDA, meanUk1DLDA;
    cv::calcCovarMatrix(Test1DLDAMat, covUk1DLDA, meanUk1DLDA, CV_COVAR_NORMAL+CV_COVAR_COLS);
    
    Mat covPositive1DLDA, inv_covPositive1DLDA, meanPositive1DLDA;
    cv::calcCovarMatrix(Positive1DLDAMat, covPositive1DLDA, meanPositive1DLDA, CV_COVAR_NORMAL+CV_COVAR_COLS);
    
    Mat covNegative1DLDA, inv_covNegative1DLDA, meanNegative1DLDA;
    cv::calcCovarMatrix(Negative1DLDAMat, covNegative1DLDA, meanNegative1DLDA, CV_COVAR_NORMAL+CV_COVAR_COLS);
    
    invert(covUk1DLDA, inv_covUk1DLDA, DECOMP_SVD);
    invert(covPositive1DLDA, inv_covPositive1DLDA, DECOMP_SVD);
    invert(covNegative1DLDA, inv_covNegative1DLDA, DECOMP_SVD);
    printf("\n*****************Using LDA chosen features to classify Train samples*****************\n");

    // Step 6. Using LDA results to classify Train and Test samples
    
    // Calculate the Mahalanobis Distance and compare
    bool LDA[TestFileCount];
    int count_LDA=0;
    for (int i=0; i<TestFileCount; i++) {
        // Calculate
        Mat sample_Test = (Test1DLDAMat.col(i));
        // cout << sample_Test << endl << endl << meanPositive1DPCA << endl << endl << inv_covUk1DPCA << endl;
        double dist_to_Positive = Mahalanobis(sample_Test, meanPositive1DLDA, inv_covPositive1DLDA);
        double dist_to_Negative = Mahalanobis(sample_Test, meanNegative1DLDA, inv_covNegative1DLDA);
        
        // Compare
        if ( dist_to_Positive < dist_to_Negative) {
            LDA[i]=1;
            count_LDA++;
        }
        else {
            LDA[i]=0;
        }
    }
    printf("Total %d Positive results are: ", count_LDA);
    for (int i=0; i< TestFileCount; i++) {
        if (LDA[i] == true) {
            printf("%d, ", (i+1));
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif