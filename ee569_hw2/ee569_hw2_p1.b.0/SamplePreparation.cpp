//
//  SamplePreparation.cpp
//  HW2_P1.b.0
//
//
//  Input: [training Negative sample set 1] .......... 2-D 25x12 array obtained from 1.1
//         [training Negative sample set 2] .......... 2-D 25x12 array obtained from 1.1
//         [training Negative sample set 3] .......... 2-D 25x12 array obtained from 1.1
//         [output 1]
//         [testing sample set 1] .................... 2-D 25x12 array obtained from 1.1 (if necessary)
//         [testing sample set 2] .................... 2-D 25x12 array obtained from 1.1 (if necessary)
//         [testing sample set 3] .................... 2-D 25x12 array obtained from 1.1 (if necessary)
//         [testing sample set 4] .................... 2-D 25x12 array obtained from 1.1 (if necessary)
//         [outout 2]
//
//  Output: [training Negative sample set] ........... 2-D 25x36 array obtained from 1.1
//          [testing sample set] ..................... 2-D 25x48 array obtained from 1.1 (if necessary)
//
//  Here is the reason of preparation:
//
//     For example, "Grass vs. Non-grass" => use grass on input 1
//
//     For PCA,
//     we mix input 1~4 into one 25x72 array for PCA in step 1, get mean and eigenvectors,
//     then mix input 2~4 (unknown part),
//     then use PCA result to reduce all remaining sample sets (1, 2~4, 5~8) from 25xN into 3xN,
//     and finally apply M-distance classifier.
//
//     For LDA,
//     we mix input 1~4 together as 25x72 array,
//     then "relabel" them for LDA in step 2, get eigenvectors,
//     then use LDA result to reduce sample set (1, 2~4, 5~8) from 25xN into 3xN,
//     and finally apply M-distance classifier.
//
//     Since we're going to to binary classification, we need different combinations (different 2~4) for each classification.
//
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef SAMPLE_PREPARATION
#define SAMPLE_PREPARATION

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE * file;
    int NumOfFeats = 25, labeledFileCount = 12;         // Define the variables
    
    // For example, "Grass vs. Non-grass"
    
    // argv[1] = "/Users/YJLee/Desktop/leather_train_N.array";
    // argv[2] = "/Users/YJLee/Desktop/sand_train_N.array";
    // argv[3] = "/Users/YJLee/Desktop/straw_train_N.array";
    // argv[4] = "/Users/YJLee/Desktop/non_xxx_train.array";  ******* output
    
    // argv[5] = "/Users/YJLee/Desktop/grass_test.array";
    // argv[6] = "/Users/YJLee/Desktop/leather_test.array";
    // argv[7] = "/Users/YJLee/Desktop/sand_test.array";
    // argv[8] = "/Users/YJLee/Desktop/straw_test.array";
    // argv[9] = "/Users/YJLee/Desktop/Test.array";     ******* output
    
    // Check for proper syntax
    if (argc < 4)
    {
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [Negative training sample set]x3 [output training Negative sample set] ([testing sample set]x4 [output testing sample set])" << endl;
        return 0;
    }
    
    // Read all the features by fread(ptr,size,count,fp)
    double NegativeSamples1[NumOfFeats][labeledFileCount];
    double NegativeSamples2[NumOfFeats][labeledFileCount];
    double NegativeSamples3[NumOfFeats][labeledFileCount];
    
    double TestSamples1[NumOfFeats][labeledFileCount];
    double TestSamples2[NumOfFeats][labeledFileCount];
    double TestSamples3[NumOfFeats][labeledFileCount];
    double TestSamples4[NumOfFeats][labeledFileCount];
    
    if (!(file=fopen(argv[1],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(NegativeSamples1, sizeof(double), NumOfFeats*(labeledFileCount), file);
    fclose(file);
    
    if (!(file=fopen(argv[2],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(NegativeSamples2, sizeof(double), NumOfFeats*(labeledFileCount), file);
    fclose(file);

    if (!(file=fopen(argv[3],"rb")))
    {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(NegativeSamples3, sizeof(double), NumOfFeats*(labeledFileCount), file);
    fclose(file);
    
    if (argc == 10) {
        if (!(file=fopen(argv[5],"rb")))
        {
            cout << "Error: unable to open file" <<endl;
            exit(1);
        }
        fread(TestSamples1, sizeof(double), NumOfFeats*(labeledFileCount), file);
        fclose(file);
        
        if (!(file=fopen(argv[6],"rb")))
        {
            cout << "Error: unable to open file" <<endl;
            exit(1);
        }
        fread(TestSamples2, sizeof(double), NumOfFeats*(labeledFileCount), file);
        fclose(file);
        
        if (!(file=fopen(argv[7],"rb")))
        {
            cout << "Error: unable to open file" <<endl;
            exit(1);
        }
        fread(TestSamples3, sizeof(double), NumOfFeats*(labeledFileCount), file);
        fclose(file);
        
        if (!(file=fopen(argv[8],"rb")))
        {
            cout << "Error: unable to open file" <<endl;
            exit(1);
        }
        fread(TestSamples4, sizeof(double), NumOfFeats*(labeledFileCount), file);
        fclose(file);
    }
    /////
    // Debug:
    //for (int fc=0; fc<12; fc++){
    //    printf(" \n\n tst feats: %d \n ", fc);
    //    for (int ft=0;ft<NumOfFeats; ft++)  {
    //        printf("%02f, ", TestSamples4[ft][fc]);
    //      }
    //}

    // Combine 3 groups of different negative sample array into one array
    double NegativeSamples[NumOfFeats][labeledFileCount*3];
    for (int fc=0; fc<(labeledFileCount); fc++ ) {
        for (int ft=0; ft<NumOfFeats; ft++) {
            NegativeSamples[ft][fc] = NegativeSamples1[ft][fc];
            NegativeSamples[ft][fc+labeledFileCount] = NegativeSamples2[ft][fc];
            NegativeSamples[ft][fc+2*labeledFileCount] = NegativeSamples3[ft][fc];
        }
    }
    
    // Combine 4 groups of different test sample array into one array
    double TestSamples[NumOfFeats][labeledFileCount*4];
    if (argc == 10) {
        for (int ft=0; ft<NumOfFeats; ft++) {
            for (int fc=0; fc<labeledFileCount; fc++ ) {
                TestSamples[ft][fc] = TestSamples1[ft][fc];
                TestSamples[ft][fc+labeledFileCount*1] = TestSamples2[ft][fc];
                TestSamples[ft][fc+labeledFileCount*2] = TestSamples3[ft][fc];
                TestSamples[ft][fc+labeledFileCount*3] = TestSamples4[ft][fc];
            }
        }
    }
    
 
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    if (!(file=fopen(argv[4],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(NegativeSamples, sizeof(double), NumOfFeats*(labeledFileCount*3), file);
    fclose(file);
    cout << "\n\nAll Negative sample-sets successfully saved" <<endl;
    
    if (argc == 10) {
        if (!(file=fopen(argv[9],"wb"))) {
            cout << "Error: unable to save file" << endl;
            exit(1);
        }
        
        fwrite(TestSamples, sizeof(double), NumOfFeats*(labeledFileCount*4), file);
        fclose(file);
        cout << "\n\nAll Test sample sets successfully saved" <<endl;
    }
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\n\n elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif