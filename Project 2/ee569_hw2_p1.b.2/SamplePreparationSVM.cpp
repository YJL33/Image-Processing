//
//  SVM.cpp
//  HW2_P1.b.2
//
//  Establish four sets of training/testing data for SVM binary classifier, which is provided by libsvm.
//  https://www.csie.ntu.edu.tw/~cjlin/libsvm/
//
//  ("svm-train" and "svm-predict" are applied)
//
//  Input: [Positive training image samples]..................... 2-D 25x36 array obtained from 1.b.0
//         [Negative training image samples]..................... 2-D 25x36 array obtained from 1.b.0
//         [Test samples]........................................ 2-D 25x48 array obtained from 1.b.0, required labeling (modifying the code)
//
//  Output [Training data] ...................................... required format for libsvm
//         [Testing data] ....................................... required format for libsvm
//
//
//  According to requirement of libSVM,
//  Rescale and Convert the Test/Training sample's feature arrays into desired format:
//
//  [label] [feature1]:[value1] [feature2]:[value2] ... (sample 1)
//  [label] [feature1]:[value1] [feature2]:[value2] ... (sample 2)
//
//   e.g.  +1 1:0.708333 2:1 3:1 4:-0.320755 5:-0.105023 6:-1 7:1 8:-0.419847 9:-1 10:-0.225806 12:1 13:-1
//         -1 1:0.583333 2:-1 3:0.333333 4:-0.603774 5:1 6:-1 7:1 8:0.358779 9:-1 10:-0.483871 12:-1 13:1
//
//
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef SVM
#define SVM

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE * file;
    int NumOfFeats = 25, TestFileCount = 48, TrainFileCount = 36;         // Define the variables
    double TestFeatsRescale[NumOfFeats][TestFileCount], PositiveFeatsRescale[NumOfFeats][TrainFileCount], NegativeFeatsRescale[NumOfFeats][TrainFileCount];
    
    // For example:
    // argv[1] = "/Users/YJLee/Desktop/Grass_train.array";
    // argv[2] = "/Users/YJLee/Desktop/Non-Grass_train.array";
    // argv[3] = "/Users/YJLee/Desktop/Test.array";
    
    // Check for proper syntax
    if (argc < 6)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [Positive samples] [Negative samples] [Test samples] [output training images samples] [output test images samples]" << endl;
        return 0;
    }
    
    // Read all the features by fread(ptr,size,count,fp)
    double TestFeats[NumOfFeats][TestFileCount], PositiveFeats[NumOfFeats][TrainFileCount], NegativeFeats[NumOfFeats][TrainFileCount];
    if (!(file=fopen(argv[3],"rb")))
    {
        cout << "Error: unable to open file 1" <<endl;
        exit(1);
    }
    fread(TestFeats, sizeof(double), NumOfFeats*TestFileCount, file);
    fclose(file);

    if (!(file=fopen(argv[1],"rb")))
    {
        cout << "Error: unable to open file 2" <<endl;
        exit(1);
    }
    fread(PositiveFeats, sizeof(double), NumOfFeats*TrainFileCount, file);
    fclose(file);
    
    if (!(file=fopen(argv[2],"rb")))
    {
        cout << "Error: unable to open file 3" <<endl;
        exit(1);
    }
    fread(NegativeFeats, sizeof(double), NumOfFeats*TrainFileCount, file);
    fclose(file);
    // Debug:
    //for (int fc=0; fc<48; fc++){
    //    printf(" \n\n tst feats: %d \n ", fc);
    //    for (int ft=0;ft<NumOfFeats; ft++)  {
    //        printf("%02f, ", TestFeats[ft][fc]);
    //}
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Step 0. Adjust the scale
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
    // Step 1. Convert the training matrix into format for libSVM
    if (!(file=fopen(argv[4],"wb"))) {
        cout << "Error: unable to save file 1" << endl;
        exit(1);
    }
    for (int fc=0; fc<TrainFileCount; fc++) {
        printf(  "+1");
        for (int ft=0; ft<NumOfFeats; ft++) {
            printf(  " %d:%f", (ft+1), PositiveFeatsRescale[ft][fc]);
        }
        printf(  "\n");
    }
    for (int fc=0; fc<TrainFileCount; fc++) {
        printf(  "-1");
        for (int ft=0; ft<NumOfFeats; ft++) {
            printf(  " %d:%f", (ft+1), NegativeFeatsRescale[ft][fc]);
        }
        printf(  "\n");
    }
    fclose(file);
    
    if (!(file=fopen(argv[5],"wb"))) {
        cout << "Error: unable to save file 2" << endl;
        exit(1);
    }
    
    // Label test data  **********************************************************
    printf("Reminder: did you modifiy the label?");
    for (int fc=0; fc<TestFileCount; fc++) {
        if (36 <= fc && fc < 48) {
            printf(  "+1");
        }
        else {
            printf(  "-1");
        }
        for (int ft=0; ft<NumOfFeats; ft++) {
            printf(  " %d:%f", (ft+1), TestFeatsRescale[ft][fc]);
        }
        printf(  "\n");
    }
    fclose(file);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\nFiles are ready for svm analysis\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif